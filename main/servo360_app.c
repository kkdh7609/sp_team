#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define SV360_DEV_NAME "/dev/servo_dev"

// mode 0 = turn 90 degrees, mode 1 = turn 180 degrees, mode 2 = turn -90 degrees
int turn_servo360(int mode){
  int fd;
  char turn_msg;

  fd = open(SV360_DEV_NAME, O_WRONLY);

  if(fd < 0){
    perror("[SERVO360] Failed to open..\n");
    return -1;
  }

  if(mode == 0)
    turn_msg = '0';
  else if(mode == 1)
    turn_msg = '1';
  else if(mode == 2)
    turn_msg = '2';
  else{
    printf("[SERVO360] Wrong mode!\n");
    close(fd);
    return -1;
  }

  if(write(fd, &turn_msg, 1) < 0){
    perror("[SERVO] Failed to write\n");
    close(fd);
    return -1;
  }
  
  close(fd);
  return 0;
}
