#include <stdio.h>
#include <wiringPi.h>
#include <time.h>
int main(void)
{
  int k;
  int k_prev =1;
  if(wiringPiSetup()==-1)
    return 1;

  pinMode(4,INPUT);
  while(1){
    k=digitalRead(4);
    printf("%d\n",k); 
  
  }
    return 0;
}
