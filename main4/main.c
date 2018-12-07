#include "servo180_app.h"
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT_NUM  55003
 
int main(void){
  int sock;
  int add_server_size;
  int num_buff;
  int pre_status = 0;
  int err_ck;
  int cur_stat;
  struct sockaddr_in add_server;
  struct sockaddr_in add_client;

  char buff[6];
  sock = socket(PF_INET, SOCK_DGRAM, 0);

  if(sock < 0){
    printf("[MAIN] Socket error!\n");
    return 0;
  }

  memset(&add_client, 0, sizeof(add_client));
  add_client.sin_family = AF_INET;
  add_client.sin_port = htons(PORT_NUM);
  add_client.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(sock, (struct sockaddr*)&add_client, sizeof(add_client))){
    printf("[MAIN] bind error!\n");
    return 0;
  }

  while(1){
    recvfrom(sock, buff, 6, 0, (struct sockaddr*)&add_server, &add_server_size);
    num_buff = (int)(buff[0] - '0');
    if (pre_status != num_buff){
      if(num_buff == 2){
        if(init_servo180() < 0){
          printf("[MAIN] Init failed\n");
          continue;
        }
        cur_stat = 0;
      }
      else{
        if(turn_servo180(cur_stat) < 0){
          printf("[Main] turn failed\n");
          continue;
        }
        cur_stat = 1 - cur_stat;
      }
      pre_status = num_buff;
    }
    sleep(1);
  }
}
