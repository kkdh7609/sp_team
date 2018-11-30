#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

int main(void){
  int fd,fp;
  char k;

  fd = open("/dev/servo_dev", O_RDWR);

  if(fd < 0){
    perror("Failed to open the device..");
    return errno;
  }

  k = '2';
  fp = write(fd, &k, 1);
  close(fd);
  return 0;
}

