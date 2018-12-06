#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUT1_DEV_NAME "/dev/button_dev"
#define BUT2_DEV_NAME "/dev/button_dev_2"

int status_button(void);
