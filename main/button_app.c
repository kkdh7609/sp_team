#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUT_DEV_NAME "/dev/button_dev"

// output 0 = no change, output 1 = button 1 is pressed, output 2 = button 2 is pressed, output 3 = button1, button2 are pressed
int status_button(){
  int fd;
  char buf;

  fd = open(BUT_DEV_NAME, O_RDONLY);
  if(fd == -1){
    printf("[BUTTON] Open error!\n");
    return -1;
  }

  if(read(fd, &buf, 1) < 0){
    printf("[BUTTON] Read error!\n");
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

