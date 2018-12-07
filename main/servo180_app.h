#include <stdio.h>
#include <unistd.h>
#include <fctnl.h>

#define SV180_DEV_NAME_1 "/dev/servo180_dev"
#define SV180_DEV_NAME_2 "/dev/servo180_dev_2"

int init_servo180(int turn_a, int tura_b);
int turn_servo180(int mode, int cur_state1, int cur_state2);
