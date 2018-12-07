#include "light_app.h"
#include "servo180_app.h"
#include "button_app.h"
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT_1 55001
#define PORT_2 55002

static int is_on=0;
static char client1_ip[16];
static char client2_ip[16];
static int turn_mode;

void *udp_sender_btn(void *p){
  pid_t pid;
  pthread_t tid;
  char *ch = (char*)p;
  int sock;
  struct sockaddr_in client_addr;
  char send_msg[] = "0";

  pid = getpid();
  tid = pthread_self();

  sock = socket(PF_INET, SOCK_DGRAM, 0); 
  if(sock < 0){
    printf("[MAIN]Failed to setting socket!\n");
    return;
  }
  memset(&client_addr, 0, sizeof(client_addr));
  client_addr.sin_family = AF_INET;
  client_addr.sin_port = htons(PORT_1);
  client_addr.sin_addr.s_addr = inet_addr(client1_ip);

  while(1){
    if(is_on == 1)
      send_msg[0] = '1';
    else if(is_on == 2)
      send_msg[0] = '2';
    else if(is_on == 3)
      send_msg[0] =='3';
    else{
      send_msg[0] = '0';
    }
    sendto(sock, send_msg, strlen(send_msg) + 1, 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
    sleep(1);
  }
}

void *udp_sender_light(void *p){
  pid_t pid;
  pthread_t tid;
  char *ch = (char*)p;
  int sock;
  struct sockaddr_in client_addr;
  char send_msg[] = "0";

  pid = getpid();
  tid = pthread_self();

  sock = socket(PF_INET, SOCK_DGRAM, 0);
  if(sock < 0){
    printf("[MAIN]Failed to setting socket!\n");
    return;
  }
  memset(&client_addr, 0, sizeof(client_addr));
  client_addr.sin_family = AF_INET;
  client_addr.sin_port = htons(PORT_2);
  client_addr.sin_addr.s_addr = inet_addr(client2_ip);

  while(1){
    send_msg[0] = turn_mode + '0';
    sendto(sock, send_msg, strlen(send_msg) + 1, 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
    sleep(1);
  }
}

void *button_checker(void *p){
  pid_t pid;
  pthread_t tid;
  char *ch = (char*)p;
  int but_num;
  int but_status[2] = {0, 0};
  int temp_btn;
  pid = getpid();
  tid = pthread_self();

  temp_btn = status_button();
  sleep(1);
  while(1){
    but_num = status_button();
    printf("%d\n",but_num);
    if(but_num < 0){
      printf("[MAIN] Error occured in button\n");
      sleep(1);
      continue;
    }
    else if(but_num == 1)
      but_status[0] = (but_status[0] + 1) % 2;
    else if(but_num == 2)
      but_status[1] = (but_status[1] + 1) % 2;
    else if(but_num == 3){
      but_status[0] = (but_status[0] + 1) % 2;
      but_status[1] = (but_status[1] + 1) % 2;
    }
    if((but_status[0] == 0) && (but_status[1] == 0))
      is_on = 0;
    else if((but_status[0] == 1) && (but_status[1] ==  0))
      is_on = 1;
    else if((but_status[0] == 0) && (but_status[1] == 1))
      is_on = 2;
    else
      is_on = 3;

    printf("%d\n",is_on);
    sleep(1);
  }
}

void* servo_turning(void *p){
  pid_t pid;
  pthread_t tid;
  char *ch = (char*)p;
  int pre_stat = 0;
  int turn_a, turn_b;
  int cur_a = 0;
  int cur_b = 0;
  time_t pre_time;
  time_t now_time;
  time(&pre_time);

  while(1){
    if(is_on != pre_stat){
      if(pre_stat == 0){
        turn_a = (is_on % 2);
        turn_b = (int)(is_on >= 2);
        init_servo180(turn_a, turn_b);
        pre_stat = is_on;
        cur_a = 0;
        cur_b = 0;
      }
      else if(pre_stat == 1){
        turn_b = (int)(is_on >= 2);
        init_servo180(0, turn_b);
        pre_stat = is_on;
        cur_b = 0;
      }
      else if(pre_stat == 2){
        turn_a = (is_on % 2);
        init_servo180(turn_a, 0);
        pre_stat = is_on;
        cur_a = 0;
      }
    }
    time(&now_time);
    if((now_time - pre_time) >= 30){
      turn_servo180(is_on, cur_a, cur_b);
      cur_a = 1 - cur_a;
      cur_b = 1 - cur_b;
    }
    sleep(1);
  }
}

int main(){
  int light_loc;
  pthread_t p_thread[5];
  int thread_id;
  int status;
  char udp_btn[] = "udp_btn";
  char udp_light[] = "udp_light";
  char btn_st[] = "btn_status";
  char servo_thread[] = "servo_thread";

  scanf("%s",client1_ip);
  scanf("%s",client2_ip);

  thread_id = pthread_create(&p_thread[0], NULL, udp_sender_btn, (void*)udp_btn);
  if(thread_id < 0){
    perror("[MAIN] thread create error\n");
    return 0;
  }

  thread_id = pthread_create(&p_thread[1], NULL, udp_sender_light, (void*)udp_light);
  if(thread_id < 0){
    perror("[MAIN] thread create error\n");
    return 0;
  }

  thread_id = pthread_create(&p_thread[2], NULL, servo_turning, (void*)servo_thread);
  if(thread_id < 0){
    perror("[MAIN] thread create error\n");
    return 0;
  }

  thread_id = pthread_create(&p_thread[3], NULL, button_checker, (void*)btn_st);
  if(thread_id < 0){
    perror("[MAIN] thread create error\n");
    return 0;
  }

  is_on = 0;
  while(1){
    if(is_on == 0){
      turn_mode = 0;
      continue;
    }
    light_loc = get_light();

    // To avoid duplicate inputs
    if(light_loc == 2)
      turn_mode == 1? (turn_mode = 6) : (turn_mode = 1); 
    else if(light_loc == 3)
      turn_mode == 2? (turn_mode = 7) : (turn_mode = 2);
    else if(light_loc == 4)
      turn_mode == 3? (turn_mode = 8) : (turn_mode = 3);
    else if(light_loc == -1){            // ERROR OCCURED
      printf("[MAIN] Error occured in light sensors\n");
      continue;
    }
    else
      continue;

    sleep(2);
  }
  pthread_join(p_thread[0], (void**)&status);
  pthread_join(p_thread[1], (void**)&status);
  pthread_join(p_thread[2], (void**)&status);
  pthread_join(p_thread[3], (void**)&status);

  return 0;
}
