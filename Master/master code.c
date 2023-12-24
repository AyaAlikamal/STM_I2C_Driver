#include "master_code.h"

/*
Description: begin function to set all settings of i2c.
Parameters: addr: the address of the connected Slave.
Return statues: this function is void doesn't return any data
*/
void begin(uint8_t addr){
          // Enable the I2C CLOCK and GPIO CLOCK
          RCC_APB1ENR |= (1<<21);  // enable I2C CLOCK
          RCC_AHB1ENR |= (1<<1);  // Enable GPIOB CLOCK

          // Configure the I2C PINs for ALternate Functions
          //PB8 and PB9 are connected to I2C1_SCL and I2C1_SDA
          GPIOB_MODER |= (2<<16) | (2<<18);  // Bits (17:16)= 1:0 --> Alternate Function for Pin PB8
                                            // Bits (19:18)= 1:0 --> Alternate Function for Pin PB9
          GPIOB_OTYPER |= (1<<8) | (1<<9);  //  Bit8=1, Bit9=1  output open drain
          GPIOB_OSPEEDR |= (3<<16) | (3<<18);  // Bits (17:16)= 1:1 --> Very High Speed for PIN PB8; Bits (19:18)= 1:1 --> Very High Speed for PIN PB9
          GPIOB_PUPDR |= (1<<16) | (1<<18);  // Bits (17:16)= 0:1 --> Pull up for PIN PB8; Bits (19:18)= 0:1 --> pull up for PIN PB9
          GPIOB_AFRH |= (4<<0) | (4<<4);  // Bits (3:2:1:0) = 0:1:0:0 --> AF4 for pin PB8;  Bits (7:6:5:4) = 0:1:0:0 --> AF4 for pin PB9


            // Reset the I2C
            I2C1_CR1 |= (1<<15); //I2C Peripheral under reset state
            I2C1_CR1 &= ~(1<<15); //I2C Peripheral not under reset
            
            // Program the peripheral input clock in I2C_CR2 Register in order to generate correct timings
            I2C1_CR2 |= (PCLK1Frequency <<0);  // PCLK1 FREQUENCY in MHz

            // Configure the clock control registers
            I2C1_CCR = 210<<0;

            // Configure the rise time register
            I2C1_TRISE = 43;  // I2C1_TRISE=((Tr(SCL))/(Tp(CLK1)))+1 = (1000 ns/(1/42*10^6))+1 =43

            // Program the I2C_CR1 register to enable the peripheral
            I2C1_CR1 |= (1<<0);  // Enable I2c
            /*Initialize the I2C module and join the I2C bus. If
              the address is -1 (define it as a preprocessor macro)
              it joins the bus as a master, otherwise join as a
              slave.*/
            if (addr == -1){
            // -1 = 0xFF
            //join I2C as a master mode
            /******Generate a start condition**********/
            I2C1_CR1 |= (1<<10);  // Enable the ACK
            I2C1_CR1 |= (1<<8);  // Generate START
            while (!(I2C1_SR1 & (1<<0)));  // Wait for SB bit to set to 1, This indicates that the start condition is generated

            /**********Send the Slave Address to the DR Register*****************/
            I2C1_DR = addr;  //  send the address
            while (!(I2C1_SR1 & (1<<1)));  // while ADDR=0 address not recieved but when ADDR=1 the address recieved
            temp = I2C1_SR1 | I2C1_SR2;  // read SR1 and SR2 to clear the ADDR bit

        }
       else{
        //I2C default is a slave unless you generate a start condition to join as a master
            I2C1_OAR1 = addr;//own address interface
            I2C1_CR1 |= (1<<10);  // Enable the ACK ,indicate that a byte is received
        }
}
/*
Description: this funtion Release the I2C bus.
Parameters: void no parameter needed.
Return statues: this function is void doesn't return any data
*/
void end(void)  {

           I2C1_CR1 |= (1<<9);//stop generation
            // Wait until the STOP condition is complete
           while (I2C1_SR2 & (1<<0)); //Cleared by hardware after detecting a Stop condition on the bus

           // Clear the STOP bit
           I2C1_CR1 &= ~(1<<9);
  }

/*
Description: this funtion Request bytes from device of addr.
Parameters: addr: the address of the required Slave.
            quantity: the data you want to send.
Return statues: this function is void doesn't return any data.
*/
 void  requestFrom(uint8_t addr,uint8_t quantity){
            I2C1_CR1 |= (1<<10);  // Enable the ACK
            I2C1_CR1 |= (1<<8);  // Generate start codition
            while (!(I2C1_SR1 & (1<<0)));// wait until the start condition is generated

            I2C1_DR = addr;  //  send the address
                while (!(I2C1_SR1 & (1<<1)));  // while ADDR=0 address not recieved but when ADDR=1 the address recieved
            temp = I2C1_SR1 | I2C1_SR2;   // read SR1 and SR2 to clear the ADDR bit
           availableBytes =  quantity;
  }
/*
Description: this funtion Begin enqueueing up the data to transmit to the device given by addr.
Parameters: addr: the address of the connected Slave.
Return statues: this function is void doesn't return any data
*/

void beginTransmision(uint8_t addr) {
    // Send START condition
          I2C1_CR1 |= (1<<10);  // Enable the ACK
          I2C1_CR1 |= (1<<8);  // Generate start codition
          while (!(I2C1_SR1 & (1<<0)));// wait until the start condition is generated

    // Send slave address
          I2C1_DR = addr ;  // send the address
         while (!(I2C1_SR1 & (1<<1)));  // while ADDR=0 address not recieved but when ADDR=1 the address recieved
         temp = I2C1_SR1 | I2C1_SR2; //once read SR1 and SR2 clear the ADDR bit
}
/*
Description: this funtion Transmit the bytes that have been queued and end the transmission.
Parameters: void.
Return statues: this function is void doesn't return any data
*/
void endTransmision(){

         // wait for BTF bit to set
          while (!(I2C1_SR1 & (1<<2))); //waiting while BTF=0 but when BTF=1; Data byte transfer succeeded
          I2C1_CR1 |= (1<<9);// generate stop condition to end the transmsion

}
/*
Description: this funtion Write data from peripheral to controller.
Parameters: dat: the data required to be written in the DR Register.
Return statues: this function is void doesn't return any data
*/

void Write (uint8_t data){
  /**** STEPS FOLLOWED  ************
  //master transmitter
  1. Wait for the TXE (bit 7 in SR1) to set. This indicates that the DR is empty
  2. Send the DATA to the DR Register
  3. Wait for the BTF (bit 2 in SR1) to set. This indicates the end of LAST DATA transmission
  */
          while (!(I2C1_SR1 & (1<<7)));  // wait for TXE bit to set
          //while TXE=0, I2C1_DR is empty but when TXE=1 by the hardware, I2C1_DR is not empty

              I2C1_DR = data   ;
              // wait for BTF bit to set
          while (!(I2C1_SR1 & (1<<2))); //waiting while BTF=0 but when BTF=1; Data byte transfer succeeded

  }
/*
Description: this funtion Reads a byte that was transmitted from a peripheral to the controller.
Parameters: addr: the address of required register we want to read data from.
Return statues: this function is void doesn't return any data
*/
uint8_t Read (uint8_t addr ){
     /**** STEPS FOLLOWED  ************
          If only 1 BYTE needs to be Read
          a) Write the slave Address, and wait for the ADDR bit (bit 1 in SR1) to be set
          b) the Acknowledge disable is made during EV6 (before ADDR flag is cleared) and the STOP condition generation is made after EV6
          c) Wait for the RXNE (Receive Buffer not Empty) bit to set
          d) Read the data from the DR
          */

             uint8_t receivedData = 0;
                      I2C1_DR = addr;  //  send the address
                while (!(I2C1_SR1 & (1<<1)));  // wait for ADDR bit to set

            I2C1_CR1 &= ~(1<<10);  // clear the ACK bit
            temp = I2C1_SR1 | I2C1_SR2;  // read SR1 and SR2 to clear the ADDR bit.... EV6 condition
            I2C1_CR1 |= (1<<9);  // Stop I2C

            while (!(I2C1_SR1 & (1<<6)));  // wait for RxNE to set

            // Read the data from the DATA REGISTER
            receivedData = I2C1_DR;
        return receivedData;
     }
/*
Description: this funtion Modify the peripheral clock frequency (define the maximum and minimum limits as a preprocessor macro).
Parameters: freq: the required frequancy to data to be sent.
Return statues: this function is void doesn't return any data
*/
void setClock(uint8_t freq){
         if ( freq >=2 && freq<= 50){
            PCLK1Frequency = freq;
         }

  }

/*
Description: this funtion Enable the Noise filter.
Parameters: en: if 1 the digital filter is enabled if 0 the analog filter is enabled.
            filter_type: the type of filter required.
Return statues: this function is void doesn't return any data
*/

void enableNoiseFilter (uint8_t en, uint8_t filter_type) {
 if (!(I2C1_CR1 & (1 << 0))){//check if PE is disabled or not
    if (en) {
        if (filter_type == 0x01) {
            // Enable digital filter
            //set bits [0:3] of I2C1_FLTR to 0001;

        } else {
            // Enable analog filter
            //I2C1_FLTR |= (0<<4);
        }
    }
    }
}
/*
Description: this funtion check if there is a data recieved or not.
Parameters: void.
Return statues: return the number of bytes available for retrieval.
*/
uint16_t availableBytes(void) {
    uint16_t availableBytes = 0;

    // Check the Receive Data Register Not Empty (RxNE) flag
    if (I2C1_SR1 & (1<<6)) {
        availableBytes++;
    }

    // You might need additional checks or processing based on your implementation

    return availableBytes;
}
/*
Description: this funtion called when a peripheral receives a transmission through the interrupt service routine (use function pointer).
Parameters: void *function(void): pointer to function thtat will be excuted.
Return statues: void.
*/
void onReceive(void(*function)(void))
/*
1 - start condition detected
2 - ADDR == 1 & RxNE == 1
3 - execute function
4 - read DR
5 - stop generation
*/
{
 while(!(I2C1_SR1 & (1<<1)) & !(I2C1_SR1 & (1<<6))); // wait till ADDR == 1 & RxNE == 1
 handler = function ;         // handler pointer takes value of function
 handler();
 temp = I2C1_DR;     // read DR
 I2C1_CR1 |= (1<<9); // stop generation
}

/*
Description: this funtion called when controller requests data through the interrupt service routine(use function pointer).
Parameters: void *function(void): pointer to function thtat will be excuted.
Return statues: void.
*/
void onRequest(void(*function)(void))
/*
1 - start condition detected
2 - ADDR == 1 & Tx == 1
3 - execute function
4 - read DR
5 - stop generation
*/
{
 while(!(I2C1_SR1 & (1<<1)) & !(I2C1_SR1 & (1<<7))); // wait till ADDR == 1 & Tx == 1
 handler = function ;         // handler pointer takes value of function
 handler();
 temp = I2C1_DR;     // read DR
 I2C1_CR1 |= (1<<9); // stop generation
}