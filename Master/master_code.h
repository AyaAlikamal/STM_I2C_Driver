#ifndef MASTER_CODE_H_
#define MASTER_CODE_H_

//..................Macros Section..................//
#define MY_ADDRESS -1 // Define the address as a preprocessor macro as master
#define I2C_TIMEOUT 10000 // Define your desired timeout
#define I2C_MAX_FREQ 400000 // Maximum I2C frequency (400 kHz in this example)
#define I2C_MIN_FREQ 10000  // Minimum I2C frequency (10 kHz in this example)
#define RCC_AHB1ENR 0x40023830                  //registers addresses defination
#define RCC_APB1ENR 0x40023840
#define GPIOB_MODER 0x40020400
#define GPIOB_OTYPER 0x40020404
#define GPIOB_OSPEEDR 0x40020408
#define GPIOB_PUPDR 0x4002040C
#define GPIOB_AFRH 0x40020424
#define I2C1_CR1 0x40005400
#define I2C1_CR2 0x40005404
#define I2C1_CCR 0x4000541C
#define I2C1_TRISE 0x40005420
#define I2C1_SR1 0x40005414
#define I2C1_SR2 0x40005418
#define I2C1_OAR1 0x40005408
#define I2C1_DR 0x40005410
//..................Data types Section..................//
typedef unsigned char uint8_t;         //define unsigned char as new type uint8_t
typedef unsigned int  uint16_t;        //define unsigned int as new type uint16_t
typedef unsigned long uint32_t;        //define unsigned long as new type uint32_t
uint16_t temp ;                          //temp variable
uint8_t received_Byte  ;                 //temp variable
uint8_t availableBytes;                  //temp variable
uint8_t  PCLK1Frequency=12  ;            //12mhz
//..................Function Decleration Section..................//
void begin(uint8_t addr);                //begin function to set all settings of i2c
void end(void);                          //Release the I2C bus
void requestFrom(uint8_t addr,uint8_t quantity);    //Request bytes from device of addr.
void beginTransmision(uint8_t addr);              //Begin enqueueing up the data to transmit to the device given by addr.
void endTransmision();                            //Transmit the bytes that have been queued and end the transmission.
void Write(uint8_t dat);                          //Write data from peripheral to controller.
uint8_t Read (uint8_t addr );                      //Reads a byte that was transmitted from a peripheral to the controller.
void setClock(uint8_t freq);                        //Modify the peripheral clock frequency (define the maximum and minimum limits as a preprocessor macro).
void enableNoiseFilter (uint8_t en, uint8_t filt_type); //Provide capability of enabling either the analog filter or digital filter of the I2C module, if en is true
uint16_t availableBytes(void);                            //Returns the number of bytes available for retrieval.
void onReceive(void(*)(void));                             //Register a function to be called when a peripheral receives a transmission through the interrupt service routine (use function pointer).
void onRequest(void(*)(void));                              //Register a function to be called when the controller requests data through the interrupt service routine(use function pointer)

#endif