#include "master_code.h"


void main(void){
   begin(MY_ADDRESS);  //set as master to send data
   setClock(12);   //set clock of i2c as 12Mhz

 while(1){
   beginTransmision(0x08); //begin transmision to address 8 in hex
   Write(1);   //send 1 as high level for led
   delay_ms(3000);  //delay 3 sec
   Write(0);   //send 1 as low level for led
   delay_ms(3000);  //delay 3 sec
   endTransmision(); //end transmision
}

}