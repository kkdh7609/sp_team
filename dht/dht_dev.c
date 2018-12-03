#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define GPIO_DHT 4
#define DEV_NAME "DHT_dev"
#define DEV_NUM 241

#define MAXTIMING 83 // 40 databits * 2 + response signal + pullup ready signal + first start to transmit bit
int dht_data[5] = {0, };

MODULE_LICENSE("GPL");

int dht_open(struct inode *pinode, struct file *pfile){
  printk("[DHT] Open dht_dev\n");
  if(gpio_request(GPIO_DHT, "dht")!=0{
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
  int pre_state = 1;
  int counter = 0;
  int i;
  int j = 0;

  // Initialize data
  dht_data[0] = 0;  // High humidity 8
  dht_data[1] = 0;  // Low humidity 8
  dht_data[2] = 0;  // High temp 8
  dht_data[3] = 0;  // Low temp 8
  dht_data[4] = 0;  // Parity bit

  // Low 18ms, High 20~40 us is start signal.
  gpio_direction_output(GPIO_DHT, 0);
  msleep(18);

  gpio_set_value(GPIO_DHT, 1);
  usleep_range(32);
  gpio_direction_input(GPIO_DHT);

  pre_state = gpio_get_value(GPIO_DHT);
  for (i = 0; i < MAXTIMINGS; i++){
    counter = 0;
    while( gpio_get_value(GPIO_DHT) == pre_state){
      counter++;
      usleep_range(1, 1);
      if ( counter == 199 ){    // something wrong!
        break;
      }
    }
    if(counter == 99){
      break;
    }

    pre_state = (pre_state + 1) % 2;   // if pre_state was 1 then set 0, prestate was 0 then set 1.

    // 1st low signal = response signal ( 80 us )
    // 2nd high signal = pulled ready to output ( 80 us )
    // 3th and odd order low signal = not used just for waiting ( 50 us )
    // if evne order high signal output for 26~28us it means bit data '0'
    // if even order high signal output for 70us it means bit data '1'
    if (i >= 4){
      

