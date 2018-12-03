#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <delay.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>

#define GPIO_BUT1 17
#define GPIO_BUT2 18
#define DEV_NAME "BUTTON_dev"
#define DEV_NUM 241

MODULE_LICENSE("GPL");

/*
* Button Handler
*/

static int button_irq1, button_irq2;
static bool is_pushed;
static int temp1, temp2;
static int pre_status = 0;
static int pre_check = 0;
static int clk_count = 0;

irqreturn_t button_interrupt(int irq, void *dev_id){
  if(pre_check == 0){
    pre_check = 1;
    clk_count++;
    printk("%d\n", clk_count);
  }
  msleep(10);
  printk("Hi\n");
  return IRQ_HANDLED;
}

irqreturn_t button_fall_interrupt(int irq, void *dev_id){
  printk("ffff\n");
  pre_status = 0;
  pre_check = 0;
  printk("Bye\n");
  msleep(10);

  return IRQ_HANDLED;
}

struct file_operations fop = {
};

int __init button_init(void){
  printk("[BUTTON] Initialize BUTTON_dev\n");
  is_pushed = false;
  register_chrdev(DEV_NUM, DEV_NAME, &fop);
  gpio_request(GPIO_BUT1, "button1");
  gpio_request(GPIO_BUT2, "button2");
  gpio_direction_output(GPIO_BUT1, 1);
  gpio_direction_input(GPIO_BUT1);
  gpio_direction_input(GPIO_BUT2);
  printk("%d\n", gpio_get_value(GPIO_BUT1));

  button_irq1 = gpio_to_irq(GPIO_BUT1);
  button_irq2 = gpio_to_irq(GPIO_BUT2);

  temp1 = request_irq(button_irq1, &button_interrupt, IRQF_TRIGGER_RISING, "button1", NULL);
  temp2 = request_irq(button_irq2, &button_fall_interrupt, IRQF_TRIGGER_FALLING, "button2", NULL);
  //printk("%d\n",temp);
  if(temp1<0){
    printk("Failed\n");
  }
  return 0;
}

void __exit button_exit(void){
  printk("[BUTTON] Exit BUTTON_dev\n");
  free_irq(button_irq1, NULL);
  free_irq(button_irq2, NULL);
  gpio_free(GPIO_BUT1);
  gpio_free(GPIO_BUT2);
  unregister_chrdev(DEV_NUM, DEV_NAME);
}

module_init(button_init);
module_exit(button_exit);

