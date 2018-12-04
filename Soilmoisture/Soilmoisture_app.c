#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/kdev_t.h>

#define DEV_PATH "/dev/Soilmoisture_dev"

#define BUFFER_LENGTH 4

static char receive[BUFFER_LENGTH];

int main(int argc, char* argv[]) {
  int fd = 0;
  char buffer[BUFFER_LENGTH];

  if((fd = open(DEV_PATH, O_RDWR | O_NONBLOCK)) < 0 ) {
   printf("1Faile\n");
  }

  int Value = 0;

  printf("Soilmoisture Test Start\n");

  while(1) {

//   write(fd,buffer, strlen(buffer));

   usleep(1000 * 500);

   read(fd, receive, BUFFER_LENGTH);

   printf("%s\n", receive);
  }

  close(fd);

  return 0;
}
