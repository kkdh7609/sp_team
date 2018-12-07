#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

int main(void){
  int fd;
  char c[20];
  while(1){
  fd = open("/dev/dht_dev", O_RDONLY);
  read(fd, c, 20);
  if(c[0] == '0')  printf("xxxxxxxx\n");
  else printf("%s\n", c);
  sleep(1);

  close(fd);
  }
}
