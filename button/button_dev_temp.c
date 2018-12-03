#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/time.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>

#define GPIO_BUT1 19
#define GPIO_BUT2 26
#define DEV_NAME "BUTTON_dev"
#define DEV_NUM 239

MODULE_LICENSE("GPL");

/*
* Button Handler
*/

static int button_irq1, button_irq2;
static bool is_pushed;
static int temp1, temp2;
static struct timeval pre_time;
static int pre_status;

irqreturn_t button_interrupt(int irq, void *dev_id){
  struct timeval cur_time;
  unsigned long start_time, end_time;
  do_gettimeofday(&cur_time);
  start_time = (int)pre_time.tv_sec;
  end_time = (int)cur_time.tv_sec;

  if((end_time - start_time > 1)){ 
    printk("[BUTTON] Button Pressed\n");
    is_pushed = true;
  }
  do_gettimeofday(&pre_time);

  return IRQ_HANDLED;
}


irqreturn_t button_fall_interrupt(int irq, void *dev_id){
  struct timeval cur_time;
  unsigned long start_time, end_time;

  do_gettimeofday(&cur_time);
  start_time = (unsigned long)pre_time.tv_sec;
  end_time = (unsigned long)cur_time.tv_sec;

  unsigned long times = end_time - start_time;
  printk("%lu\n", times);

  if(end_time - start_time > 1){
    pre_status = 0;
    printk("[BUTTON] Button Released\n");
  }
 do_gettimeofday(&pre_time); 

  return IRQ_HANDLED;
}

int button_open(struct inode *pinode, struct file *pfile){
  printk("[BUTTON] Open button_dev\n");
  return 0;
}

int button_close(struct inode *pinode, struct file *pfile){
  printk("[BUTTON] Close servo_dev\n");
  return 0;
}

ssize_t button_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset){
  char buf;
  printk("[BUTTON] Read light dev\n");
  if(is_pushed){
    buf = '1';
    is_pushed = false;
  }
  else
    buf = '0';

  copy_to_user(buffer, &buf, 1);
  return 0;
}

struct file_operations fop = {
  .owner = THIS_MODULE,
  .open = button_open,
  .read = button_read,
  .release = button_close,
};

int __init button_init(void){
  printk("[BUTTON] Initialize BUTTON_dev\n");
  is_pushed = false;
  pre_status = 0;
  do_gettimeofday(&pre_time);
  register_chrdev(DEV_NUM, DEV_NAME, &fop);
  gpio_request(GPIO_BUT1, "button1");
  //gpio_request(GPIO_BUT2, "button2");
  gpio_direction_input(GPIO_BUT1);
  //gpio_direction_input(GPIO_BUT2);
  
  button_irq1 = gpio_to_irq(GPIO_BUT1);
  //button_irq2 = gpio_to_irq(GPIO_BUT2);

  temp1 = request_irq(button_irq1, &button_interrupt, IRQF_TRIGGER_RISING, "button1", NULL);
  //temp2 = request_irq(button_irq2, &button_fall_interrupt, IRQF_TRIGGER_FALLING, "button2", NULL);
  
  if(temp1<0){
    printk("Failed\n");
  }
  return 0;
}

void __exit button_exit(void){
  printk("[BUTTON] Exit BUTTON_dev\n");
  free_irq(button_irq1, NULL);
  //free_irq(button_irq2, NULL);
  gpio_free(GPIO_BUT1);
  //gpio_free(GPIO_BUT2);
  unregister_chrdev(DEV_NUM, DEV_NAME);
}

module_init(button_init);
module_exit(button_exit);

