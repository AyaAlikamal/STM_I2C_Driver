#include "Slave_code.h"

void main(void){
   begin(MY_ADDRESS);  //set as master to send data
   setClock(12);   //set clock of i2c as 12Mhz
   RCC_AHB1ENR  |= (1<<3);
   GPIOD_MODER =0XFFFFFFFF;
 while(1){
      onReceive(event);
}
void event(int num){
    int bytes =availableBytes();
    while(1 <bytes)
    {
      uint8_t c= Read();
    }
    if (c ==1 )
     GPIOD_ODR|=(1<<0);
     else{  GPIOD_ODR &=~ (1<<0);}
 
 }
}