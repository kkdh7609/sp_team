#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUT1_DEV_NAME "/dev/button_dev"
#define BUT2_DEV_NAME "/dev/button_dev_2"

// output 0 = no change, output 1 = button 1 is pressed, output 2 = button 2 is pressed, output 3 = button1, button2 are pressed
int status_button(){
  int fd1, fd2;
  char buf;
  int changes[2] = {0, 0};

  fd1 = open(BUT1_DEV_NAME, O_RDONLY);
  if(fd1 == -1){
    printf("[BUTTON1] Open error!\n");
    return -1;
  }

  if(read(fd1, &buf, 1) < 0){
    printf("[BUTTON1] Read error!\n");
    close(fd1);
    return -1;
  }

  if(buf == '1'){
    close(fd1);
    changes[0] = 1;
  }

  else{
    close(fd1);
  }

  fd2 = open(BUT2_DEV_NAME, O_RDONLY);
  if(fd2 == -1){
    printf("[BUTTON2] Open error!\n");
    return -1;
  }

  if(read(fd2, &buf, 1) < 0){
    printf("[BUTTON2] Read error!\n");
    close(fd2);
    return -1;
  }

  if(buf == '1'){
    close(fd2);
    changes[1] = 1;
  }

  else{
    close(fd2);
  }

  return(changes[0] + changes[1]*2);
}

