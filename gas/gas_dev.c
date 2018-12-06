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

#define DEV_NAME "GAS_dev"
#define GPIO_GAS 17
#define DEV_NUM 240

MODULE_LICENSE("GPL");

int gas_open(struct inode* pinode, struct file* pfile){
  printk("[GAS]Open Gas dev\n");
  if(gpio_request(GPIO_GAS, "gas")<0){
    printk("[GAS]GPIO ERROR\n");
    return -1;
  }
  if(gpio_direction_input(GPIO_GAS)<0){
    printk("[GAS]GPIO INPUT ERROR\n");
    return -1;
  }
  return 0;
}

int gas_close(struct inode *pinode, struct file *pfile){
  printk("[GAS]Release Gas dev\n");
  gpio_free(GPIO_GAS);
  return 0;
}

ssize_t gas_read(struct file* pfile, char __user* buffer, size_t length, loff_t* offset){
  char buf;
  printk("[GAS]Read Gas dev\n");
  gpio_get_value(GPIO_GAS)==1? buf='1' : (buf='0');
  printk("%c\n", buf);
  copy_to_user(buffer,&buf,1);
  
  return 0;
}

struct file_operations fop = {
  .owner = THIS_MODULE,
  .open = gas_open,
  .read = gas_read,
  .release = gas_close,
};

int __init gas_init(void){
  printk("[GAS]Init Gas dev\n");
  register_chrdev(DEV_NUM,DEV_NAME,&fop);
  return 0;
}

void __exit gas_exit(void){
  printk("[GAS]Exit Gastest dev\n");
  unregister_chrdev(DEV_NUM,DEV_NAME);
}

module_init(gas_init);
module_exit(gas_exit);

