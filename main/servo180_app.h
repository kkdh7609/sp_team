#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define SV180_DEV_NAME_1 "/dev/servo180_dev"

int init_servo180(void);
int turn_servo180(int cur_state1);
