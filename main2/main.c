#include "gas_app.h"
#include "soil_app.h"
#include "3led_app.h"
#include <time.h>
#include <pthread.h>

static int gas_stat = 0;
static int soil_stat = 0;

void *get_gas(void *p){
  pid_t pid;
  pthread_t tid;
  int check;
  char *ch = (char*)p;

  pid = getpid();
  tid = pthread_self();

  while(1){
    check = status_gas();
    if(check == -1){
       printf("[MAIN] Error in gas\n");
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

  pid = getpid();
  tid = pthread_self();

  while(1){
    check = status_soil();
    if(check == -1){
       printf("[MAIN] Error in soil\n");
       continue;
    }
    soil_stat = check;
    sleep(1);
  }
}

void *setting_led(void *p){
  pid_t pid;
  pthread_t tid;
  int mode;
  char* ch = (char*)p;

  pid = getpid();
  tid = pthread_self();

  while(1){
    mode = (2 * soil_stat) + gas_stat;
    if(set_led(mode) < 0){
      printf("[MAIN] set led error\n");
    }
    sleep(1);
  }
}
int main(void){
  pthread_t p_thread[2];
  int thread_id;
  int status;
  char gas_thread[] = "gas_thread";
  char soil_thread[] = "soil_thread";
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

  setting_led((void*)main_thread);

  pthread_join(p_thread[0], (void **)&status);
  pthread_join(p_thread[1], (void **)&status);
  return 0;
}
