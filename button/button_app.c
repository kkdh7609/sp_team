#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

int main(){
  int fd,fd2;
  char buf;
    fd = open("/dev/button_dev", O_RDONLY);
    fd2 = open("/dev/button_dev_2", O_RDONLY);
  while(1){ 

    read(fd, &buf, 1);
    if(buf == '1')
      printf("button_pressed\n");
    buf='0';

    read(fd2, &buf, 1);
    if(buf == '1')
      printf("button2_pressed\n");
    buf='0';
  }
    close(fd);
    close(fd2);
}
