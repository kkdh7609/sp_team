#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define LED_DEV_NAME "/dev/3led_dev"

int set_led(int mode);
