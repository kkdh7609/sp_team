#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define GPIO_DHT 13
#define DEV_NAME "DHT_dev"
#define DEV_NUM 239

#define DATANUM 40 // 40 databits
static int data[5] = {0, 0, 0, 0, 0}; // [0] = High gumidity 8bits, [1] = Low humidity 8bits, [2] = High temper 8bits, [3] = Low temper 8bits, [4] = Parity 8bit
static bool status_err;
MODULE_LICENSE("GPL");

int dht_open(struct inode *pinode, struct file *pfile){
  printk("[DHT] Open dht_dev\n");
  if(gpio_request(GPIO_DHT, "dht")!=0){
    printk("[DHT] Pin already being used");
    return -1;
  }
  return 0;
}

int dht_close(struct inode *pinode, struct file *pfile){
  printk("[DHT] Close dht_dev\n");
  gpio_free(GPIO_DHT);
  return 0;
}

void read_data(void){
  // 1st low signal = response signal ( 80 us )
  // 2nd high signal = pulled ready to output ( 80 us )
  // 3th and odd order low signal = not used. just for waiting ( 50 us )
  // if evne order high signal output for 26~28us it means bit data '0'
  // if even order high signal output for 70us it means bit data '1'
  int i;
  int temp_i;
  int u_times = 0;
  
  status_err = false;
  // Initialize data
  data[0] = 0;  // High humidity 8
  data[1] = 0;  // Low humidity 8
  data[2] = 0;  // High temp 8
  data[3] = 0;  // Low temp 8
  data[4] = 0;  // Parity bit
  gpio_direction_output(GPIO_DHT, 0);
  // Low 18ms, High 20~40 us is start signal.
  gpio_set_value(GPIO_DHT, 0);
  mdelay(18);

  gpio_set_value(GPIO_DHT, 1);
  udelay(30);

  gpio_direction_input(GPIO_DHT);
  // Respons signal is here
  while(u_times < 150){                       // if 150us is passed then something is wrong.
    if(gpio_get_value(GPIO_DHT) == 1)            // 'pulled ready to output' signal
      break;
    udelay(1);
    u_times++;
  }
  if(u_times == 150){                        // failed to read
    status_err = true;
    return;
  }
  u_times = 0;
  udelay(100);                        // pulled ready output signal = 80us, response signal = 50us.
  
  // Low Pulse is here
  
  for (i = 0; i < DATANUM; i++){            // get data 40 bits
    u_times = 0;
    while(u_times < 150){                   // if 150us is passed then something is wrong.
      if(gpio_get_value(GPIO_DHT) == 1)
        break;
      udelay(1);
      u_times++;
    }
    if(u_times == 150){                        // failed to read
      status_err = true;
      return;
    }
    u_times = 0;
    
    // Actually get data 
    while(gpio_get_value(GPIO_DHT) == 1){
      udelay(1);
      u_times++;

      if(u_times == 150){                      // failed to read
        status_err = true;
        return;
      }
    }
    temp_i = i/8;
    data[temp_i] = data[temp_i] << 1;
    if(u_times > 25){                      // bit data 0 - 26~28us high signal, bit data 1 - 70us high signal. But actually 0 - 13~14us, 1 - 40us because of delay.
      data[temp_i] = data[temp_i] + 1;
    }
  }
  
  if(data[4] == ((data[0] + data[1] + data[2] + data[3]) % 256)){             // parity check
    return;                                                                  // success to read
  }
  printk("4\n");
  status_err = true;                                                          // worng parity
  return;
}

int dht_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset){
  printk("[DHT] read dht dev\n");
  char buf[20];

  read_data();
  if(status_err){
    printk("[DHT] Reading failed\n");
    buf[0] = '-';
    copy_to_user(buffer, &buf[0], 1);
    return -1;
  }

  printk("%d, %d, %d, %d\n", data[0], data[1], data[2], data[3]);
  sprintf(buf, "%d,%d,%d,%d\n", data[0], data[1], data[2], data[3]);
  copy_to_user(buffer, buf, sizeof(buf));
  return 0;
}

struct file_operations fop = {
  .owner = THIS_MODULE,
  .open = dht_open,
  .read = dht_read,
  .release = dht_close,
};

int __init dht_init(void){
  printk("[DHT] Initialize DHT_dev\n");
  register_chrdev(DEV_NUM, DEV_NAME, &fop);

  return 0;
}

void __exit dht_exit(void){
  printk("[DHT] Exit DHT_dev\n");
  unregister_chrdev(DEV_NUM, DEV_NAME);
}

module_init(dht_init);
module_exit(dht_exit);
