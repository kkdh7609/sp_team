#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define SOIL_DEV_NAME "/dev/soil_dev"

// output
int status_soil(){
  int fd;
  char buf;

  fd = open(SOIL_DEV_NAME, O_RDONLY);
  if(fd == -1){
    printf("[SOIL] Open error!\n");
    return -1;
  }

  if(read(fd, &buf, 1) < 0){
    printf("[SOIL] Read error!\n");
    close(fd);
    return -1;
  }

  if(buf == '1'){
    close(fd);
    return 1;
  }

  else{
    close(fd);
    return 0;
  }
}
