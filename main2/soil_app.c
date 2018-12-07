#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define SOIL1_DEV_NAME "/dev/soil_dev"
#define SOIL2_DEV_NAME "/dev/soil_dev_2"

// output
int status_soil(int mode){
  int fd1, fd2;
  int result[2] = {0, 0};
  char buf;
  if((mode % 2) == 1){
    fd1 = open(SOIL1_DEV_NAME, O_RDONLY);
    if(fd1 == -1){
      printf("[SOIL1] Open error!\n");
      return -1;
    }
    if(read(fd1, &buf, 1) < 0){
      printf("[SOIL1] Read error!\n");
      close(fd1);
      return -1;
    }

    if(buf == '0'){
      close(fd1);
      result[0] = 1;
    }
    else{
      close(fd1);
    }
  }

  if( mode >= 2){
    fd2 = open(SOIL2_DEV_NAME, O_RDONLY);
    if(fd2 == -1){
      printf("[SOIL2] Open error!\n");
      return -1;
    }
    if(read(fd2, &buf, 1) < 0){
      printf("[SOIL1] Read error!\n");
      close(fd2);
      return -1;
    }

    if(buf == '0'){
      close(fd2);
      result[1] = 1;
    }
    else{
      close(fd2);
    }
  }

  return(result[0] + result[1]*2);
}
