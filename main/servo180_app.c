#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define SV180_DEV_NAME_1 "/dev/servo180_dev"

int init_servo180(){
  int fd1;
  char buf = '0';
  
  fd1 = open(SV180_DEV_NAME_1, O_RDWR);
  if(fd1 < 0){
    printf("[SERVO180_1] Failed to open device\n");
    return -1;
  }

  if(write(fd1, &buf, 1) < 0){
    printf("[SERVO180_1] Failed to write device\n");
    close(fd1);
    return -1;
  }
  close(fd1);
  return 0;
}
  

int turn_servo180(int cur_state1){
  int fd1;
  char buf;

  fd1 = open(SV180_DEV_NAME_1, O_RDWR);
  if(fd1 < 0){
    printf("[SERVO_180_1] Open error\n");
    return -1;
  }

  buf = (1 - cur_state1) + '0';
  if(write(fd1, &buf, 1) < 0){
    printf("[SERVO_180_1] Failed to write device\n");
    close(fd1);
    return -1;
  }
  close(fd1);

  return 0;
}
