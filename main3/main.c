#include "servo360_app.h"
#include <time.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT_NUM 55002

static int mode;

void *udp_receiver(void *p){
  pid_t pid;
  pthread_t tid;
  char* ch = (char*)p;
  int sock;
  int server_addr_size;

  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;

  char buff_rcv[6];

  pid = getpid();
  tid = pthread_self();

  sock= socket(PF_INET, SOCK_DGRAM, 0);

  if(sock < 0){
    printf("[MAIN] Socket error!\n");
    return 0;
  }

  memset(&client_addr, 0, size(client_addr));
  client_addr.sin_family = AF_INET;
  client_addr.sin_port = htons(PORT_NUM);
  client_addr.sin_Addr.s_addr = htonl(INADDR_ANY);

  if(bind(sock, (strcut sockaddr*)&client_addr, sizeof(client_addr))){
    printf("[MAIN] bind error!\n");
    return 0;
  }

  while(1){
    recvfrom(sock, buff_rcv, 6, 0, (struct sockaddr*)&server_addr, &server_addr_size);
    mode = (int)(buff_rcv[0] - '0');
    printf("%d\n", mode);
    sleep(1);
  }
}

int main(void){
  pthread_t p_thread;
  char name_thread[] = "udp_thread";
  int thread_id, status;
  int pre_status = 0;
  mode = 0;
  thread_id = pthread_create(&p_thread, NULL, udp_receiver, (void*)name_thread);

  if(thread_id < 0){
    perror("[MAIN] thread create error\n");
    return 0;
  }

  while(1){
    if(pre_status == mode){
      sleep(1);
      continue;
    }
    else if(mode > 5){
      pre_status = mode - 6;
    }
    else{
      pre_status = mode - 1;
    }
    if(turn_servo360(pre_status) < 0)
      printf("[MAIN] Error in servo360\n");
    sleep(1);
  }

  pthread_join(p_thread, (void **)&status);
  return 0;
}
