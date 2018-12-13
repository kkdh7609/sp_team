#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define GPIO_360 18
#define DEV_NAME "SERVO360_dev"
#define DEV_NUM 240

MODULE_LICENSE("GPL");

int servo_open(struct inode *pinode, struct file *pfile){
  printk("[SERVO360] Open servo_dev\n");
  if(gpio_request(GPIO_360, "servo360") != 0){          // check pin is used
    printk(KERN_ALERT "[SERVO360]Already being used");
    return -1;
  }
  gpio_direction_output(GPIO_360, 0);
  return 0;
}

int servo_close(struct inode *pinode, struct file *pfile){
  printk("[SERVO360] Close servo_dev\n");
  gpio_free(GPIO_360);            // Release the pin

  return 0;
}

void turn_servo(int mode){     // mode  0 => turn 90 degree, mode 1 => turn -90 degree
  int i;

  if(mode == 0){        // Turn 90 degrees
    for(i = 0; i < 15; i++){
      gpio_set_value(GPIO_360, 1);
      usleep_range(1300, 1300);
      gpio_set_value(GPIO_360, 0);
      mdelay(20);
    }
  }

  else if(mode == 1){       // Turn -90 degrees
    for(i=0; i < 15; i++){
      gpio_set_value(GPIO_360, 1);
      usleep_range(1700, 1700);
      gpio_set_value(GPIO_360, 0);
      mdelay(20);
    }
  }
}

ssize_t servo_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset){
  char get_msg;          // inputed by user

  if(copy_from_user(&get_msg, buffer, length) < 0){
    printk(KERN_ALERT "[SERVO360] Write error\n");
    return -1;
  }

  if(get_msg == '0'){      // Turn 90 degrees
    turn_servo(0);
    printk("[SERVO360] Turn 90 degrees\n");
  }

  else if(get_msg == '1'){    // Turn 180 degress
    turn_servo(0);
    msleep(800);
    turn_servo(0);
    printk("[SERVO360] Turn 180 degrees\n");
  }

  else if(get_msg == '2'){      // Turn -90 degrees
    turn_servo(1);
    printk("[SERVO360] Turn -90 degrees\n");
  }
  
  return 0;
}

struct file_operations fop = {
  .owner = THIS_MODULE,
  .open = servo_open,
  .write = servo_write,
  .release = servo_close,
};

int __init servo_init(void){
  printk("[SERVO360] Initialize SERVO_dev\n");
  register_chrdev(DEV_NUM, DEV_NAME, &fop);
  return 0;
}

void __exit servo_exit(void){
  printk("[SERVO360] Exit SERVO_dev\n");
  unregister_chrdev(DEV_NUM, DEV_NAME);
}

module_init(servo_init);
module_exit(servo_exit);
