#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define DEV_NAME "/dev/3led_dev"

int main(){
  int fd =open(DEV_NAME,O_WRONLY);
  char buf[2048] = "";
  if(fd==-1){
    fprintf(stderr,"open() error : %s\n",strerror(errno));
    exit(1);
  }
  while(1){
    printf("R : G : B\n");
    scanf("%s",buf);
    write(fd,buf,strlen(buf));
  }
}
