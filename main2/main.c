#include "gas_app.h"
#include "soil_app.h"
#include "3led_app.h"
#include <time.h>

int main(void){
  while(1){
    if(status_soil() == 1){
      if(set_led(4) < 0)
        printf("[MAIN] error in set_led\n");
    }
    else{
      set_led(2);
    }
    sleep(1);
  }

  return 0;
}
