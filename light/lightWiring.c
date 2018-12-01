#include <stdio.h>
#include <wiringPi.h>
#include <time.h>

int main(void)
{
  int k1;
  int k2;
  int k_prev =1;
  if(wiringPiSetup()==-1)
    return 1;

  pinMode(4,INPUT);
  pinMode(5,INPUT);
  while(1){
    k1=digitalRead(4);
    k2=digitalRead(5);
    printf("%d\n",k1);
    printf("%d\n",k2);
    printf("\n");
    sleep(2); 
  }
    return 0;
}
