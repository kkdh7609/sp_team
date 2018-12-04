#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define DEV_PATH "/dev/gastest_dev"

int main(){
  int fd,ret;
  char receive[10];
  fd = open(DEV_PATH, O_RDONLY);
  if(fd<0){
    perror("Open Failed");
    return errno;
  }
  
  while(1){
    ret = read(fd, receive, 10);
    if(ret<0){
      perror("Read Failed");
      return errno;
    }
    printf("%s\n", receive);
    sleep(1);
  }
}
