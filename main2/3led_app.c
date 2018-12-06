#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define LED_DEV_NAME "/dev/3led_dev"

// mode 0 rgb 000
// mode 1 rgb 001
// mode 2 rgb 010
// mode 3 rgb 011
// mode 4 rgb 100
// mode 5 rgb 101
// mode 6 rgb 110
// mode 7 rgb 111
int set_led(int mode){
  int fd;
  char buf[4];

  fd = open(LED_DEV_NAME, O_WRONLY);
  if(fd == -1){
    printf("[LED] Open error!\n");
    return -1;
  }

  if((mode % 2) == 1){
    buf[2] = '1';
    mode = mode - 1;
  }
  else{
    buf[2] = '0';
  }

  if((mode % 4) == 2){
    buf[1] = '1';
    mode = mode - 2;
  }
  else{
    buf[1] = '0';
  }

  buf[0] = (mode / 4) + '0';
  buf[3] = '\0';
  if(write(fd, buf, 4) < 0){
    printf("[LED] Write error!\n");
    close(fd);
    return -1;
  }
  printf("b %s b\n",buf);
  close(fd);
  return 0;
}
