#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define SV360_DEV_NAME "\dev\servo_dev"

int turn_servo360(int mode);
