#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define SV180_DEV_NAME_1 "/dev/servo180_dev"
#define SV180_DEV_NAME_2 "/dev/servo180_dev_2"

int init_servo180(int turn_a, int turn_b){
  int fd1, fd2;
  char buf = '0';
  
  if(turn_a == 1){
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
  }
  sleep(2);
  if(turn_b == 1){
    fd2 = open(SV180_DEV_NAME_2, O_RDWR);
    if(fd2 < 0){
      printf("[SERVO_180_2] Failed to open device\n");
      return -1;
    }
    
    if(write(fd2, &buf, 1) < 0){
      printf("[SERVO_180_2] Failed to write device\n");
      close(fd2);
      return -1;
    }
    close(fd2);
  }
  return 0;
}
  

int turn_servo180(int mode, int cur_state1, int cur_state2){
  int fd1, fd2;
  char buf;

  if((mode % 2) == 1){
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
  }
  sleep(1);
  
  if(mode >= 2){
    fd2 = open(SV180_DEV_NAME_2, O_RDWR);
    if(fd2 < 0){
      printf("[SERVO_180_2] Open error\n");
      return -1;
    }
    
    buf = (1 - cur_state2) + '0';
    if(write(fd2, &buf, 1) < 0){
      printf("[SERVO_180_2] Failed to write device\n");
      close(fd2);
      return -1;
    }
    close(fd2);
  }
  return 0;
}
