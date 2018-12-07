#include "gas_app.h"
#include "soil_app.h"
#include "3led_app.h"
#include "dht_app.h"
#include <time.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT_NUM 55001

static int gas_stat = 0;
static int soil_stat = 0;
static int cur_tem = 0;
static int is_on;

void *get_gas(void *p){
  pid_t pid;
  pthread_t tid;
  int check;
  char *ch = (char*)p;

  pid = getpid();
  tid = pthread_self();

  while(1){
    if(is_on == 0)
      continue;
    check = status_gas();
    if(check == -1){
       printf("[MAIN] Error in gas\n");
       sleep(1);
       continue;
    }
    gas_stat = check;
    sleep(1);
  }
}

void *get_soil(void *p){
  pid_t pid;
  pthread_t tid;
  int check;
  char *ch = (char*)p;
  int soil_mode;

  pid = getpid();
  tid = pthread_self();

  while(1){
    if(is_on == 0)
      continue;
    soil_mode = is_on;
    check = status_soil(soil_mode);
    if(check == -1){
       printf("[MAIN] Error in soil\n");
       continue;
    }
    soil_stat = check;
    sleep(1);
  }
}

void *getting_dht(void *p){
  pid_t pid;
  pthread_t tid;
  int check;
  char *ch = (char*)p;
  int *res;

  pid = getpid();
  tid = pthread_self();

  while(1){
    if(is_on == 0)
      continue;
    res = get_dht();
    if(res[0] == -1){
       printf("[MAIN] Error in gas\n");
       sleep(1);
       continue;
    }
    cur_tem = res[2];
    printf("%d\n", cur_tem);
    sleep(1);
  }
}

void *setting_led(void *p){
  pid_t pid;
  pthread_t tid;
  int mode;
  char* ch = (char*)p;
  int check_soil;
  int check_temp;

  while(1){
    sleep(1);
    if(is_on == 0){
      if(set_led(0) < 0){
        printf("[MAIN] set led error\n");
      }
      continue;
    }
    if((cur_tem >= 20) && (cur_tem < 28))
      check_temp = 0;
    else
      check_temp = 1;
    (soil_stat == is_on)? (check_soil = 1):(check_soil = 0);
    mode = (4 * check_soil)+ (2 * check_temp) + gas_stat;
    if(set_led(mode) < 0){
      printf("[MAIN] set led error\n");
    }
  }
}

void *udp_receiver(void *p){
  pid_t pid;
  pthread_t tid;
  int mode;
  char* ch = (char*)p;
  int sock;
  int server_addr_size;

  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;

  char buff_rcv[6];

  pid = getpid();
  tid = pthread_self();

  sock = socket(PF_INET, SOCK_DGRAM, 0);

  if(sock < 0){
    printf("[MAIN] Socket error!\n");
    return 0;
  }

  memset(&client_addr, 0, sizeof(client_addr));
  client_addr.sin_family = AF_INET;
  client_addr.sin_port = htons(PORT_NUM);
  client_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(sock, (struct sockaddr*)&client_addr, sizeof(client_addr))){
    printf("[MAIN] bind error!\n");
    return 0;
  }

  while(1){
    recvfrom(sock, buff_rcv, 6, 0, (struct sockaddr*)&server_addr, &server_addr_size);
    is_on = (int)(buff_rcv[0] - '0');
    printf("%d\n", is_on);
    sleep(1);
  }
}
int main(void){
  pthread_t p_thread[4];
  int thread_id;
  int status;
  char gas_thread[] = "gas_thread";
  char soil_thread[] = "soil_thread";
  char udp_thread[] = "udp_thread";
  char dht_thread[] = "dht_thread";
  char main_thread[] = "main_thread";

  thread_id = pthread_create(&p_thread[0], NULL, get_gas, (void*)gas_thread);

  if(thread_id < 0){
    perror("[MAIN] thread create error\n");
    return 0;
  }
  
  thread_id = pthread_create(&p_thread[1], NULL, get_soil, (void*)soil_thread);

  if(thread_id < 0){
    perror("[MAIN] thread create error\n");
    return 0;
  }

  
  thread_id = pthread_create(&p_thread[2], NULL, getting_dht, (void*)dht_thread);

  if(thread_id < 0){
    perror("[MAIN] thread create error\n");
    return 0;
  }

  thread_id = pthread_create(&p_thread[3], NULL, udp_receiver, (void*)udp_thread);

  if(thread_id < 0){
    perror("[MAIN] thread create error\n");
    return 0;
  }
 

  setting_led((void*)main_thread);

  pthread_join(p_thread[0], (void **)&status);
  pthread_join(p_thread[1], (void **)&status);
  pthread_join(p_thread[2], (void **)&status);
  pthread_join(p_thread[3], (void **)&status);

  return 0;
}
