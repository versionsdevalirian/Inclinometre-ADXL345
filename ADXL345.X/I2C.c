
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h> 
#include "I2C.h"
#include <xc.h>


inline void I2C_wait(void) {
    while(PIR3bits.SSP1IF == 0);
    PIR3bits.SSP1IF = 0;
}

void I2C_start(void) {
    SSP1CON2bits.SEN = 1; 
    while (SSP1CON2bits.SEN); 
}

void I2C_repeated_start(void) {
//    I2C_wait();
//    I2C_SSPCON2 |= 0x02; // RSEN=1  -> initiate REPEATED START condition on SDA and SCL pins
    SSP1CON2bits.RSEN = 1;	while (SSP1CON2bits.RSEN);	//Wait for restart	

}

void NotAckI2C(void) {
	SSP1CON2bits.ACKDT = 1;			//Set for NotACk
	SSP1CON2bits.ACKEN = 1;
	while(SSP1CON2bits.ACKEN);		//wait for ACK to complete
	SSP1CON2bits.ACKDT = 0;			//Set for NotACk
}
void AckI2C(void) {
	SSP1CON2bits.ACKDT = 0;			//Set for ACk
	SSP1CON2bits.ACKEN = 1;
	while(SSP1CON2bits.ACKEN);		//wait for ACK to complete
}
void I2C_stop(void) {
    SSP1CON2bits.PEN = 1;
    while (SSP1CON2bits.PEN);
}
void I2C_select(uint8_t address, bool mode) {
    I2C_write(address << 1 | mode);
}

uint8_t I2C_read(uint8_t ack) {
    uint8_t temp;
    I2C_wait();
    SSP1CON2bits.RCEN = 1;      // enable receive mode for I2c
    while(SSP1STATbits.BF == 0);
    SSP1STATbits.BF = 0;
    SSP1CON2bits.ACKDT = (ack); // 0-- not ACK , 1-- ACK
    while(SSP1CON2bits.ACKEN);     // Send Acknowledgement
    temp = SSP1BUF; // load data from Buffer to the temp

    return temp;

    
    
    
    
    
}

void I2C_write(uint8_t data) {
    I2C_wait();
    SSP1BUF = data;  // load data into SSPBUF register
}

void I2C_send(uint8_t address, uint8_t data) {
    /**********************FUNCONA*****************************/

    unsigned char acknowledge = 0;
    I2C_start();
    SSP1BUF = (0x1d << 1) | I2C_MODE_WRITE;
    I2C_wait();
    acknowledge = SSP1CON2bits.ACKSTAT;
    while(acknowledge == 1){
        acknowledge = SSP1CON2bits.ACKSTAT;
    }
    I2C_wait(); 
    SSP1BUF = address;
    I2C_wait();
    
    acknowledge = 0;
    acknowledge = SSP1CON2bits.ACKSTAT;
    while(acknowledge == 1){
        acknowledge = SSP1CON2bits.ACKSTAT;
    }
//    I2C_wait();
    SSP1BUF = data ;
    I2C_wait();
    I2C_stop(); 
    /**********************FUNCONA*****************************/
   
}
 
void WaitAck(void) {
	int count = 2;
	while (SSP1CON2bits.ACKSTAT) {
		if (--count == 0) {
//            count = 1000;
            CLRWDT();
//			ErrorRam();
			break;
		}
	}
}
void WriteI2C(unsigned char byte) {
	//This function transmits the byte passed to the function
	//while (I2C1STATbits.TRSTAT);	//Wait for bus to be idle
	SSP1BUF = byte;					//Load byte to I2C1 Transmit buffer
	I2C_wait(); 
    WaitAck();
}
uint8_t I2C_receive(uint8_t address, uint8_t reg) {
    unsigned char acknowledge = 0;
    unsigned char byte = 0;
    unsigned char bytesNumber = 0;
    uint8_t temp;
    
    I2C_start();
    SSP1BUF = (address << 1) | I2C_MODE_WRITE;
    WaitAck();
  
    I2C_wait(); 
    SSP1BUF = reg;
    WaitAck();
    
 
//    I2C_stop(); 
    /***********primera operativa*/

    I2C_repeated_start();    

    SSP1BUF = (address << 1) | I2C_MODE_READ;
    SSP1CON2bits.RCEN = 1;			//Enable Master receive
	NOP();
    while(!SSP1STATbits.BF);
    temp = SSP1BUF;
    NotAckI2C();
    I2C_stop();
    
    
    
//    SSP1CON2bits.RCEN = 1;
//    SSP1CON2bits.ACKDT = 1;
//    SSP1CON2bits.ACKEN = 1;
//    while(SSP1STATbits.BF == 0);
//    temp = SSP1BUF;
////    I2C_wait();

    
    
//    acknowledge = SSP1CON2bits.ACKSTAT;
//    while(acknowledge == 1){
//        acknowledge = SSP1CON2bits.ACKSTAT;
//    }
//    SSP1CON2bits.RCEN = 1;
//    SSP1CON2bits.ACKDT = 1;
//    SSP1CON2bits.ACKEN = 1;
//    while(SSP1CON2bits.ACKEN);
//    temp = SSP1BUF;
//    I2C_stop();
//    
//    bytesNumber = 1;
////    for(byte = 0; byte < bytesNumber; byte++){
//            SSP1CON2bits.RCEN = 1;       /*!< Enable receive mode for I2C */
////            if(byte == (bytesNumber - 1))
////            {
//                SSP1CON2bits.ACKDT = 1;
////            }
////            else
////            {
////                SSP1CON2bits.ACKDT = 0;
////            }
//            while(SSP1STATbits.BF == 0);    /*!< Wait for reception to complete */
//            SSP1CON2bits.ACKEN = 1;
//            while(SSP1CON2bits.ACKEN);
//            SSP1STATbits.BF = 0;
//            temp = SSP1BUF;
//   
//                I2C_wait();
//
//    I2C_stop(); 

    
    return temp;
}