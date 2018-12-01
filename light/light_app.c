#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define DEV_NAME "/dev/light_dev"

int main(){

  int fd =0;
  fd = open(DEV_NAME,O_RDONLY);
  char buf[5]="";		//to have 4 light_inputs
  if(fd==-1){
    fprintf(stderr,"open() error : %s\n",strerror(errno));
    exit(1);
  }
  while(1){
    read(fd,buf,4);
    //light_1
    if(buf[0]=='1')
       printf("1\n");
    else if(buf[0]!='1')
       printf("0\n");
    //light_2
    if(buf[1]=='1')
      printf("2\n");
    else if(buf[1]!='1')
      printf("0\n");
    //light_3
    if(buf[2]=='1')
      printf("3\n");
    else if(buf[2]!='1')
      printf("0\n");
    //light_4
    if(buf[3]=='1')
      printf("4\n");
    else if(buf[3]!='1')
      printf("0\n");
    sleep(1);
  }

  printf("Light test start");

}
