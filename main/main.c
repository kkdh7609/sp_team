#include "light_app.h"
#include "servo360_app.h"
#include "button_app.h"
#include <time.h>
#include <stdbool.h>

int main(){
  int light_loc;
  int turn_mode;
  bool cur_status = false;
  int but_num;

  while(1){
    but_num = status_button();
      if(but_num < 0)
        printf("[MAIN] Error occured in button\n");
      else if(but_num == 1)
        cur_status = !cur_status;
    if(!cur_status){
      continue;
    }
    light_loc = get_light();
    if(light_loc == 2)
      turn_mode = 0;
    else if(light_loc == 3)
      turn_mode = 1;
    else if(light_loc == 4)
      turn_mode = 2;
    else if(light_loc == -1){            // ERROR OCCURED
      printf("[MAIN] Error occured in light sensors\n");
      continue;
    }
    else
      continue;

    if(turn_servo360(turn_mode) < 0){
      printf("[MAIN] Error occured in turn_servo360\n");
      continue;
    }

    sleep(2);
  }
}
