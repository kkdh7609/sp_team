#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define SV180_DEV_NAME "/dev/servo180_dev"

int init_servo180(){
  int fd1;
  char buf = '0';
  
  fd1 = open(SV180_DEV_NAME, O_RDWR);
  if(fd1 < 0){
    printf("[SERVO180] Failed to open device\n");
    return -1;
  }
  if(write(fd1, &buf, 1) < 0){
    printf("[SERVO180] Failed to write device\n");
    close(fd1);
    return -1;
  }
  close(fd1);
  return 0;
}


int turn_servo180(int current_arr){
  int fd1;
  char buf;

  fd1 = open(SV180_DEV_NAME, O_RDWR);
  if(fd1 < 0){
    printf("[SERVO_180] Open error\n");
    return -1;
  }

  buf = (1 - current_arr) + '0';
  if(write(fd1, &buf, 1) < 0){
    printf("[SERVO_180] Failed to write device\n");
    close(fd1);
    return -1;
  }
  close(fd1);
  return 0;
}
