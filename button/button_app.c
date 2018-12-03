#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

int main(){
  int fd;
  char buf;
  fd = open("/dev/button_dev", O_RDONLY);

  while(1){
    read(fd, &buf, 1);

    if(buf == '1')
      printf("button_pressed\n");
  }
}
