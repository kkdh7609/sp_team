#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/kdev_t.h>

#define DEV_PATH "/dev/ledtest_dev"
#define BUFFER_LENGTH 256
static char receive[BUFFER_LENGTH];

int main(int argc, char *argv[]){
	int fd=0;

	if((fd = open("/dev/gastest_dev", O_RDWR | O_NONBLOCK))<0){
		perror("open()");
		exit(1);
 	}

  printf("open sccess!\n");
  sleep(2);

  int ret = read(fd, receive, BUFFER_LENGTH);
  printf("%s\n", receive); 
  close(fd);
  return 0;
}  
