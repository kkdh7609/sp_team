#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define GAS_DEV_NAME "/dev/gas_dev"

// output 1 = Gas is detected, output 0 = Gas isn't detected
int status_gas(){
  int fd;
  char buf;

  fd = open(GAS_DEV_NAME, O_RDONLY);
  if(fd == -1){
    printf("[GAS] Open error\n");
    close(fd);
    return -1;
  }

  if(read(fd, &buf, 1) < 0){
    printf("[GAS] Read error!\n");
    close(fd);
    return -1;
  }

  if(buf == '1'){
    close(fd);
    return 1;
  }

  else{
    //printf("No GAS!\n");
    close(fd);
    return 0;
  }
}
