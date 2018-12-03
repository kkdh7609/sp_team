#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/time.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>

#define GPIO_BUT 19
#define DEV_NAME "BUTTON_dev"
#define DEV_NUM 239

MODULE_LICENSE("GPL");

/*
* Button Handler
*/

static int button_irq;
static bool is_pushed;
static int temp;
static struct timeval pre_time;

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
  do_gettimeofday(&pre_time);
  register_chrdev(DEV_NUM, DEV_NAME, &fop);
  gpio_request(GPIO_BUT, "button");
  gpio_direction_input(GPIO_BUT);
  
  button_irq = gpio_to_irq(GPIO_BUT);

  temp = request_irq(button_irq, &button_interrupt, IRQF_TRIGGER_RISING, "button", NULL);

  if(temp<0){
    printk("Failed\n");
  }
  return 0;
}

void __exit button_exit(void){
  printk("[BUTTON] Exit BUTTON_dev\n");
  free_irq(button_irq, NULL);
  gpio_free(GPIO_BUT);
  unregister_chrdev(DEV_NUM, DEV_NAME);
}

module_init(button_init);
module_exit(button_exit);

