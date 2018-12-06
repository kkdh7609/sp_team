#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define GPIO 4
#define DEV_NAME "Soilmoisture_dev"
#define DEV_NUM 241

MODULE_LICENSE("GPL");

int Soilmoisture_open(struct inode* pinode, struct file* pfile) {

  if( gpio_request(GPIO, "GPIO")) {
    printk(KERN_ALERT "GPIO Request Fail\n");
  }

  gpio_direction_input(GPIO);

  return 0;
}

ssize_t Soilmoisture_read(struct file* pfile, char __user* buffer, size_t length, loff_t* offset) {
  if(0 < gpio_get_value(GPIO)) {
   copy_to_user(buffer, "1", length);
  }

  else{
   copy_to_user(buffer, "0", length);
  }

  return 0;
}

int Soilmoisture_close(struct inode* pinode, struct file* pfile) {
  return 0;
}

struct file_operations fop = {
  .owner = THIS_MODULE,
  .open = Soilmoisture_open,
  .read = Soilmoisture_read,
  .release = Soilmoisture_close,
};

int __init Soilmoisture_init(void) {
  printk("[SOIL] init\n");
  register_chrdev(DEV_NUM, DEV_NAME, &fop);
  return 0;
}

void __exit Soilmoisture_exit(void) {
  unregister_chrdev(DEV_NUM, DEV_NAME);
}

module_init(Soilmoisture_init);
module_exit(Soilmoisture_exit);
