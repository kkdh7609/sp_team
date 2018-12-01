#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/cdev.h>


#define GPIO1 4
#define DEV_NAME "gastest_dev"
#define DEV_NUM 240

static char* msg = NULL;

MODULE_LICENSE("GPL");

int gastest_open(struct inode* pinode, struct file* pfile){
	printk(KERN_ALERT "OPEN gastest_dev\n");
	gpio_request(GPIO1, "GPIO1");
	gpio_direction_output(GPIO1,0);
	gpio_direction_input(GPIO1);
	if(gpio_get_value(GPIO1)==0){
		strcpy(msg,"NO GAS");
	}
	else if(gpio_get_value(GPIO1)==1){
		strcpy(msg,"GET GAS");
	}
	return 0;
}

ssize_t gastest_read (struct file* pfile, char  __user* buffer, size_t length, loff_t* offset){
	printk(KERN_ALERT "READ gastest_dev\n");
	if(strcmp(msg, "GET GAS")==0){
		copy_to_user(buffer,msg , length);
	}
	else if(strcmp(msg, "NO GAS")==0){
		copy_to_user(buffer,msg, length);
	}
	strcpy(msg, "\0");

	return 0;
		
}

int gastest_close(struct inode* pinode, struct file* pfile){
	printk(KERN_ALERT "RELEASE gastest_dev\n");
	return 0;
}

struct file_operations fop = {
	.owner = THIS_MODULE,
	.open = gastest_open,
	.read = gastest_read,
	.release = gastest_close,
};

int __init gastest_init(void) {
	printk(KERN_ALERT "INIT gastest\n");
	register_chrdev(DEV_NUM, DEV_NAME, &fop);
	msg = (char *)kmalloc(32, GFP_KERNEL);
	return 0;
}

void __exit gastest_exit(void) {
	printk(KERN_ALERT "EXIT gastest_dev\n");
	unregister_chrdev(DEV_NUM, DEV_NAME);
}

module_init(gastest_init);
module_exit(gastest_exit);

