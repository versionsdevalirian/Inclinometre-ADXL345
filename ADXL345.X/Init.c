#include <xc.h>
#include "Init.h"
#include "timer.h"
#include "spi.h"
#include "uart.h"
#include "I2C.h"

void InitPinOut(void){
    
     /** TRISx registers */ 
    TRISA = 0xF0; // A7-A6 are inputs, A5-A0 are outputs
    TRISB = 0x30; // B4, B5 are inputs.
    TRISC = 0xFE; // C0 is an output, all others are inputs.
    /** ANSELx registers */
    ANSELA = 0x00; // 0x30 previously. 10
    ANSELB = 0x00; //
    ANSELC = 0x00; // Turn off analog signals on Port C.

    /** ODx registers */ 
    ODCONA = 0x00;
    ODCONB = 0x00;
    ODCONC = 0x00;

    // PPS Unlock sequence
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    INTCONbits.GIE = 0; 
    PPSLOCKbits.PPSLOCKED = 0x00;

    // Set the PPS for I2C pins over here until MCC can do this for us when the driver is released
    SSP1DATPPS = 0x13; //RC4 input is SDA1 (Page-235).
    SSP1CLKPPS = 0x14; //RC3 input is SCL1 (Page-235).
    RC3PPS = 0x15; //RC3 output is SCL1 (Page-239).
    RC4PPS = 0x14; //RC4 output is SDA1 (Page-239).
    RC5PPS = 0x10; //RC5 as TX
    //rc6 as input rx
    RXPPS = 0x16;
    
    // PPS lock sequence
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x01;
    INTCONbits.GIE = 1; 
    INTCONbits.PEIE = 1;
}

void Init345(){
    
    I2C_start();
    WriteI2C((0x1D << 1 )| 0x00); NOP();
    I2C_wait();
    WriteI2C(DATA_FORMAT); NOP(); 
    WriteI2C(0x3B); NOP();
    I2C_stop();
    I2C_start();
    WriteI2C((0x1D << 1 )| 0x00); NOP();
    I2C_wait();
    WriteI2C(POWER_CTL); NOP();
    WriteI2C(0x08); NOP();
    I2C_stop();
    
}

void Avg(int x, int y, int z){
    I2C_start();
    WriteI2C((0x1D << 1 )| 0x00); NOP();
    I2C_wait();
    WriteI2C(0x1E); NOP(); 
    WriteI2C(x); NOP();
    I2C_stop();
    
    I2C_start();
    WriteI2C((0x1D << 1 )| 0x00); NOP();
    I2C_wait();
    WriteI2C(0x1F); NOP();
    WriteI2C(y); NOP();
    I2C_stop();
    
    I2C_start();
    WriteI2C((0x1D << 1 )| 0x00); NOP();
    I2C_wait();
    WriteI2C(0x20); NOP();
    WriteI2C(z); NOP();
    I2C_stop();
}