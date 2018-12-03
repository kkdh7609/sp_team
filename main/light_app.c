#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define LIGHT_DEV_NAME "/dev/light_dev"

int get_light(void){

  int fd = 0;
  char buf[5] = "";          // get where is a light
  fd = open(LIGHT_DEV_NAME, O_RDONLY);

  if(fd==-1){
    fprintf(stderr,"[light]Open error : %s\n", strerror(errno));
    return -1;
  }

  read(fd, buf, 4);    // get status

  if(buf[0]=='1'){
    close(fd);
    return 1;
  }

  else if(buf[1]=='1'){
    close(fd);
    return 2;
  }

  else if(buf[2]=='1'){
    close(fd);
    return 3;
  }

  else if(buf[3]=='1'){
    close(fd);
    return 4;
  }

  close(fd);
  return 0;
}
