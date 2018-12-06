#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
//if receive dev_num randomly, need
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#define GPIO_red 10		//RED
#define GPIO_green 9		//GREEN
#define GPIO_blue 11		//BLUE
#define DEV_NAME "LED_dev"
//#define DEV_NUM 242		//if recieve dev_num randomly, dosen't need 

MODULE_LICENSE("GPL");
int major;
struct cdev my_cdev;
dev_t devno;
static char *msg=NULL;

module_param(major,int,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
MODULE_PARM_DESC(major,"major");

int led_open(struct inode *pinode, struct file *pfile){
  printk("OPEN 3LED_dev\n");
  gpio_request_one(GPIO_red,1,"GPIO_red");
  gpio_request_one(GPIO_green,1,"GPIO_green");
  gpio_request_one(GPIO_blue,1,"GPIO_blue");

  //return 0 if success, returns negative if fail
  if(gpio_direction_output(GPIO_red,1)!=0) printk("Err GPIO_red\n");
  if(gpio_direction_output(GPIO_green,1)!=0) printk("Err GPIO_green\n");
  if(gpio_direction_output(GPIO_blue,1)!=0) printk("Err GPIO_blue\n");
  
  return 0;
}

int led_close(struct inode* pinode, struct file *pfile){
  printk("Release LED_dev");
  //unlock the GPIO which were used.

  return 0;
}
// gets the input like 101,110 
ssize_t led_write(struct file* pfile, const char __user* buffer, size_t length, loff_t* offset){
  char msg[4];
  
  if(copy_from_user(msg,buffer,length)<0){
    printk("LED_dev write error\n");
    return -1;
  }

  msg[0]=='1' ? gpio_set_value(GPIO_red,0) : gpio_set_value(GPIO_red,1);
  msg[1]=='1' ? gpio_set_value(GPIO_green,0) : gpio_set_value(GPIO_green,1);
  msg[2]=='1' ? gpio_set_value(GPIO_blue,0) : gpio_set_value(GPIO_blue,1);
  printk("msg : %c %c %c\n",msg[0],msg[1],msg[2]);
  return length;
}

struct file_operations fop={
  .owner = THIS_MODULE,
  .open = led_open,
  .write = led_write,
  .release = led_close,
};
/*
int __init led_init(void){
  printk("Init LED_dev\nsudo mknod -m 666 /dev/3LED_dev c 242 0\n");
  register_chrdev(DEV_NUM,DEV_NAME,&fop);
  return 0;
}

void __exit led_exit(void){
  printk("Exit LED_dev\n");
  unregister_chrdev(DEV_NUM,DEV_NAME);
}
*/
// device number receiving randomly

int __init led_init(void){
  int err;
  if(major){
    devno=MKDEV(major,0);
    register_chrdev_region(devno,1,"LED drv");
  }else{
    alloc_chrdev_region(&devno,0,1,"LED drv");
    major = MAJOR(devno);
  }

  cdev_init(&my_cdev,&fop);
  my_cdev.owner = THIS_MODULE;
  err = cdev_add(&my_cdev,devno,1);

  if(err<0){
    printk("Device add error\n");
    return -1;
  }

  printk("INIT LED_dev\nsudo mknod -m 666 /dev/3led_dev c %d 0\n", MAJOR(devno));

  msg=(char *)kmalloc(32,GFP_KERNEL);
  if(msg!=NULL)
    printk("malloc allocator address: 0x%p\n",msg);

  return 0;
}

void __exit led_exit(void){
  printk(KERN_ALERT "EXIT LED_dev\n");
  if(msg){
    kfree(msg);
  }
  gpio_free(GPIO_red);
  gpio_free(GPIO_green);
  gpio_free(GPIO_blue);

  unregister_chrdev_region(devno,1);
  cdev_del(&my_cdev);
}

module_init(led_init);
module_exit(led_exit);
