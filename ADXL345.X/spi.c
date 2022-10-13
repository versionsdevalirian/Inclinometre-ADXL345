#include <xc.h>
#include "spi.h"
#include "uart.h"
#define CS_ACC			LATBbits.LATB2
#define _XTAL_FREQ 32000000 // Fosc  frequency for _delay()  library
#define I2C1BAUD 100 // The I2C-1 BAUD rate.
void WriteSpi(unsigned char Address, unsigned char data) {
	int foo = SSP1BUF;
	CS_ACC = 0;
    SSP1CON1bits.WCOL = 0;    PIR3bits.SSP1IF = 0;

	SSP1BUF = (((0x3f & (Address))));  
    while (!PIR3bits.SSP1IF); // Wait for Rx buffer empty

    PIR3bits.SSP1IF = 0;
    SSP1BUF = (((data))); 
    while (!PIR3bits.SSP1IF); // Wait for Rx buffer empty

    PIR3bits.SSP1IF = 0;
	CS_ACC = 1;
}
int  ReadSpi(unsigned char Address) {

    int foo = SSP1BUF;
	CS_ACC = 0;
    SSP1CON1bits.WCOL = 0;    PIR3bits.SSP1IF = 0;

	SSP1BUF =   (0x80 | Address);  
    while (!SSP1STATbits.BF); // Wait for Rx buffer empty

    PIR3bits.SSP1IF = 0;
    SSP1BUF = (((0))); 
    while (!SSP1STATbits.BF); // Wait for Rx buffer empty

    PIR3bits.SSP1IF = 0;
    
    CS_ACC = 1;

	return SSP1BUF;
}


void SPIInit (void){
    // I2C Mode Configuration.
     // Enable MSSP module.
     SSP1CON1 = 0b10110000;
//     SSP1CON1 = 0b00101000;

     // Slew Rate Control.
     SSP1STAT = 0x40; // This is correct for 100KHz BAUD - Slew rate control disabled.//80

     // Configure ACKSTAT, ACKDT, RCEN, PEN, RSEN and SEN - Normally these are cleared.
     SSP1CON2 = 0x00;

     // SBCDE, BOEN, SCIE, DHEN, and AHEN are all disabled, SDAHT is 100ns.
     SSP1CON3 = 0x00;

     // I2C BAUD Rate Setup.
     SSP1ADD = 0x00;//0x0E(_XTAL_FREQ/(4 * I2C1BAUD))-1; // 100KHz BAUD. Previously 0x4F

     // SCL/SDA PIN DIRECTION SETUP
     TRISC = 0x18;
     // SCL/SDA PIN DIRECTION SETUP
     TRISB = 0x10; 
     // clear the master interrupt flag
     PIR3bits.SSP1IF = 0;
     // enable the master interrupt
//     PIE3bits.SSP1IE = 1; // 0 Temporarily disables, set to 1 to re-enable.
}

    
 
