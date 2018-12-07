#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define SV180_DEV_NAME "/dev/servo180_dev"

int init_servo180(void);
int turn_servo180(int current_arr);
