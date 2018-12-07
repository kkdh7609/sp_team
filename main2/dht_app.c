#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

#define DHT_NAME "/dev/dht_dev"
static int dht_result[4];

int* get_dht(){
  int fd, i;
  char buffer[20];
  char *ptr;
  int j = 0;

  fd = open(DHT_NAME, O_RDONLY);
  for(i=0; i<10; i++){
    read(fd, buffer, 20);
    if(buffer[0] == '-')
      continue;
    else{
      ptr = strtok(buffer, ",");
      while(ptr != NULL){
        dht_result[j] = atoi(ptr);
        ptr = strtok(NULL,",");
        j++;
      }
      close(fd);
      return dht_result;
    }
    sleep(1);
  }
  close(fd);
  dht_result[0] = -1;
  return dht_result;
}
