#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/cdev.h>


#define GPIO_light1 23
#define GPIO_light2 24
#define GPIO_light3 25
#define GPIO_light4 12
#define DEV_NAME "light_dev"


int major;
struct cdev my_cdev;
dev_t devno;
static char *msg=NULL;
module_param(major,int,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
MODULE_PARM_DESC(major,"major");

MODULE_LICENSE("GPL");

int light_open(struct inode *pinode, struct file *pfile){
  printk("Open Light dev\n");
  gpio_request(GPIO_light1,"light1 received");
  gpio_request(GPIO_light2,"light2 received");
  gpio_request(GPIO_light3,"light3 received");
  gpio_request(GPIO_light4,"light4 received");
  gpio_direction_input(GPIO_light1);
  gpio_direction_input(GPIO_light2);
  gpio_direction_input(GPIO_light3);
  gpio_direction_input(GPIO_light4);
  return 0;
}

int light_close(struct inode *pinode, struct file *pfile){
  printk("Release Light dev\n");
  gpio_free(GPIO_light1);
  gpio_free(GPIO_light2);
  gpio_free(GPIO_light3);
  gpio_free(GPIO_light4);
  return 0;
}

ssize_t light_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset){
  printk("Read Light dev\n");
  char *buf;
  buf = kmalloc(4,GFP_KERNEL);
  gpio_get_value(GPIO_light1)==1? buf[0]='1' : (buf[0] ='0');
  gpio_get_value(GPIO_light2)==1? buf[1]='1' : (buf[1] ='0');
  gpio_get_value(GPIO_light3)==1? buf[2]='1' : (buf[2] ='0');
  gpio_get_value(GPIO_light4)==1? buf[3]='1' : (buf[3] ='0');
  printk("%c %c %c %c\n",buf[0],buf[1],buf[2],buf[3]);
  copy_to_user(buffer,buf,4);
  return 0;
}

struct file_operations fop = {
  .owner = THIS_MODULE,
  .open = light_open,
  .read = light_read,
  .release = light_close,
};

int __init light_init(void){
  int err;
  if(major){
    devno=MKDEV(major,0);
    register_chrdev_region(devno,1,"Light drv");
  }
  else{
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
	
  printk("Init light\nsudo mknod -m 666 /dev/light_dev c %d 0\n",MAJOR(devno));
  msg = (char *)kmalloc(32,GFP_KERNEL);
  if(msg!=NULL)
    printk("malloc allocator address: 0x%p\n",msg);
  
  return 0;
}

void __exit light_exit(void){
  printk("Exit light\n");
  if(msg){
    kfree(msg);
  }
  unregister_chrdev_region(devno,1);
  cdev_del(&my_cdev);

}

module_init(light_init);
module_exit(light_exit);
