#include <stdio.h>
#include <unistd.h>
#include <fctnl.h>

#define SV180_DEV_NAME_1 "/dev/servo180_dev"
#define SV180_DEV_NAME_2 "/dev/servo180_dev_2"

int turn_servo180(int mode){
  int fd1, fd2;
  char buf;

  if(
}
