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
  gpio_request(GPIO_light1,"light received");
  gpio_direction_input(GPIO_light1);
  return 0;
}

int light_close(struct inode *pinode, struct file *pfile){
  printk("Release Light dev\n");
  gpio_free(GPIO_light1);
  return 0;
}

ssize_t light_read(struct file *pfile, char __user *buffer, size_t length){
  if(gpio_get_value(GPIO_light1)==0){

  }
  else
  {

  }
  return 0;
}

struct file_operations fop = {
  .owner = THIS_MODULE,
  .open = light_open,
  .read = light_read,
  .release = ligth_close,
};

int __init light_init(void){
  int err;
  if(major){
    devno=MKDEV(major,0);
    register_chrdev_region(devno,1,"Light drv");
  }
  else{
    alloc_chrdev_region(&devno,1,"LED drv");
    malloc = MAJOR(devno);
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
