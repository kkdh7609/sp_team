#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

int main(void){
  int fd;
  char c;
  while(1){
  fd = open("/dev/dht_dev", O_RDONLY);
  read(fd, &c, 1);
  if(c == '1')  printf("abcabc\n");
  if(c == '0')  printf("xxxxxxxx\n");
  sleep(1);

  close(fd);
  }
}
