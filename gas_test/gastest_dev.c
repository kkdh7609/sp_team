#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/delay.h>

#define DEV_NAME "gastest_dev"
#define GPIO1 17
#define DEV_NUM 240

MODULE_LICENSE("GPL");

int gastest_open(struct inode* pinode, struct file* pfile){
  printk("Open Gastest dev\n");
  if(gpio_request(GPIO1, "GPIO1")<0){
    printk(KERN_ALERT "GPIO ERROR\n");
    return -1;
  }
  if(gpio_direction_input(GPIO1)<0){
    printk(KERN_ALERT "GPIO INPUT ERROR\n");
    return -1;
  }
  return 0;
}

int gastest_close(struct inode *pinode, struct file *pfile){
  printk("Release Gastest dev\n");
  gpio_free(GPIO1);
  return 0;
}

ssize_t gastest_read(struct file* pfile, char __user* buffer, size_t length, loff_t* offset){
  printk("Read Gastest dev\n");
  char buf;
  gpio_get_value(GPIO1)==1? buf='1' : (buf='0');
  printk("%c\n", buf);
  if(buf=='1'){
    copy_to_user(buffer,"GET GAS",8);
  }
  if(buf=='0'){
    copy_to_user(buffer,"NO GAS", 7);
  }
  return 0;
}

struct file_operations fop = {
  .owner = THIS_MODULE,
  .open = gastest_open,
  .read = gastest_read,
  .release = gastest_close,
};

int __init gastest_init(void){
  printk("Init Gastest dev\n");
  register_chrdev(DEV_NUM,DEV_NAME,&fop);
  return 0;
}

void __exit gastest_exit(void){
  printk("Exit Gastest dev\n");
  unregister_chrdev(DEV_NUM,DEV_NAME);
}

module_init(gastest_init);
module_exit(gastest_exit);

