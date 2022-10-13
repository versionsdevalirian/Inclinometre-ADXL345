////
//// PIC16F18855 Configuration Bit Settings
//#pragma config FEXTOSC = OFF // External Oscillator mode selection bits (Oscillator not enabled)
//#pragma config RSTOSC = HFINT32 // Power-up default value for COSC bits (HFINTOSC with OSCFRQ= 32 MHz and CDIV = 1:1)
//#pragma config CLKOUTEN = OFF // Clock Out Enable bit (CLKOUT function is disabled; i/o or oscillator function on OSC2)
//#pragma config CSWEN = ON // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
//#pragma config FCMEN = ON // Fail-Safe Clock Monitor Enable bit (FSCM timer enabled)
//// CONFIG2
//#pragma config MCLRE = ON // Master Clear Enable bit (MCLR pin is Master Clear function)
//#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
//#pragma config LPBOREN = OFF // Low-Power BOR enable bit (ULPBOR disabled)
//#pragma config BOREN = OFF // Brown-out reset enable bits (Brown-out Reset Enabled, SBOREN bit is ignored)
//#pragma config BORV = LO // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (VBOR) set to 1.9V on LF, and 2.45V on F Devices)
//#pragma config ZCD = OFF // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR.)
//#pragma config PPS1WAY = OFF // Peripheral Pin Select one-way control (The PPSLOCK bit can be cleared and set only once in software)
//#pragma config STVREN = ON // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a reset)
//// CONFIG3
//#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
//#pragma config WDTE = OFF // WDT operating mode (WDT enabled regardless of sleep; SWDTEN ignored)
//#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
//#pragma config WDTCCS = SC // WDT input clock selector (Software Control)
//// CONFIG4
//#pragma config WRT = OFF // UserNVM self-write protection bits (Write protection off)
//#pragma config SCANE = available// Scanner Enable bit (Scanner module is available for use)
//#pragma config LVP = ON // Low Voltage Programming Enable bit (Low Voltage programming enabled. MCLR/Vpp pin function is MCLR.)
//// CONFIG5
//#pragma config CP = OFF // UserNVM Program memory code protection bit (UserNVM code protection disabled)
//#pragma config CPD = OFF // DataNVM code protection bit (DataNVM code protection disabled)
//
//
//
//
//
//

// CONFIG1
#pragma config FEXTOSC = OFF    // External Oscillator mode selection bits->Oscillator not enabled
#pragma config RSTOSC = HFINT1    // Power-up default value for COSC bits->HFINTOSC (1MHz)
#pragma config CLKOUTEN = OFF    // Clock Out Enable bit->CLKOUT function is disabled; i/o or oscillator function on OSC2
#pragma config CSWEN = ON    // Clock Switch Enable bit->Writing to NOSC and NDIV is allowed
#pragma config FCMEN = ON    // Fail-Safe Clock Monitor Enable bit->FSCM timer enabled

// CONFIG2
#pragma config MCLRE = ON    // Master Clear Enable bit->MCLR pin is Master Clear function
#pragma config PWRTE = OFF    // Power-up Timer Enable bit->PWRT disabled
#pragma config LPBOREN = OFF    // Low-Power BOR enable bit->ULPBOR disabled
#pragma config BOREN = ON    // Brown-out reset enable bits->Brown-out Reset Enabled, SBOREN bit is ignored
#pragma config BORV = LO    // Brown-out Reset Voltage Selection->Brown-out Reset Voltage (VBOR) set to 1.9V on LF, and 2.45V on F Devices
#pragma config ZCD = OFF    // Zero-cross detect disable->Zero-cross detect circuit is disabled at POR.
#pragma config PPS1WAY = ON    // Peripheral Pin Select one-way control->The PPSLOCK bit can be cleared and set only once in software
#pragma config STVREN = ON    // Stack Overflow/Underflow Reset Enable bit->Stack Overflow or Underflow will cause a reset
#pragma config DEBUG = OFF    // Background Debugger->Background Debugger disabled

// CONFIG3
#pragma config WDTCPS = WDTCPS_31    // WDT Period Select bits->Divider ratio 1:65536; software control of WDTPS
#pragma config WDTE = OFF    // WDT operating mode->WDT Disabled, SWDTEN is ignored
#pragma config WDTCWS = WDTCWS_7    // WDT Window Select bits->window always open (100%); software control; keyed access not required
#pragma config WDTCCS = SC    // WDT input clock selector->Software Control

// CONFIG4
#pragma config WRT = OFF    // UserNVM self-write protection bits->Write protection off
#pragma config SCANE = available    // Scanner Enable bit->Scanner module is available for use
#pragma config LVP = ON    // Low Voltage Programming Enable bit->Low Voltage programming enabled. MCLR/Vpp pin function is MCLR.

// CONFIG5
#pragma config CP = OFF    // UserNVM Program memory code protection bit->Program Memory code protection disabled
#pragma config CPD = OFF    // DataNVM code protection bit->Data EEPROM code protection disabled


#define CSS LATBbits.LATB2
//#define SDO LATBbits.LATB5
#define _XTAL_FREQ 32000000 // Fosc  frequency for _delay()  library
#define I2C1BAUD 400000 // The I2C-1 BAUD rate.
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h> 
#include "spi.h"
#include "Init.h"
#include "timer.h"
#include "uart.h"
#include "I2C.h"
/******* Function Definitions ********/
void initBoard(); // Initialize the board.
void initMSSP1_I2C(); // Initialize MASTER SYNCHRONOUS SERIAL PORT (MSSP) in I2C mode.
int valorAngle(uint8_t eix);

void main(){
    static int T;
    static int Estat, res = 0;
    static int Xant,Yant,Zant, Zmostra,Xmostra,Ymostra,i,r, j, contadorInit,cal, xcal,ycal,zcal,Xoriginal, Yoriginal, Zoriginal,Za,Xa,Ya = 0;
    static signed int x ,y, z;
     static double accX,accY,accZ; 
    static int x0,x1, y0, y1, z0, z1;
    static float roll ;
    static char tmp[30];
    InitPinOut();
    initMSSP1_I2C(); 
    TiInitTimer(); SiInit(0);
    CSS = 1; // START returned. 

    while(1){
        CLRWDT(); 
        switch(Estat){
            case 0:
                T = TiGetTimer(); Estat =1;
                break;
            case 1:
                if(TiGetTics(T)>100){
                    Init345(); Estat = 24; TiResetTics(T);SiPuts("Inicialitzant....\n");Avg(0,0,0);
                }
                break;
            case 24:
                if(TiGetTics(T)>30){
                    Estat = 25; TiResetTics(T);cal = 1;
                }
                break;
            case 25:
                Avg(3,3,2);
                Estat = 2;TiResetTics(T);      
                break;
            case 2: 
                CLRWDT();
                x0 = valorAngle(DATAX0);
                x1 = valorAngle(DATAX1);
                y0 = valorAngle(DATAY0);
                y1 = valorAngle(DATAY1);
                z0 = valorAngle(DATAZ0);
                z1 = valorAngle(DATAZ1);

                x = (x0 | (x1 << 8));
                y = (y0 | (y1 << 8));
                z = (z0 | (z1 << 8));
                Ya = Ymostra;
                Ymostra = ALFA*y + U_ALFA*Yant;
                Yant = Ymostra;
                Xa = Xmostra;
                Xmostra = ALFA*x + U_ALFA*Xant;
                Xant = Xmostra;
                Za = Zmostra;
                Zmostra = ALFA*z + U_ALFA*Zant;
                Zant = Zmostra;
                if(contadorInit == 69) Estat = 3;
                else Estat = 22; 
                break;      
            case 22:
//                if(Xmostra == Xa && Ymostra == Ya){
                    contadorInit++; 
//                }else{
//                    contadorInit = 0;  
//                }
                if(contadorInit == 50){
                    xcal = Xant; ycal = Yant;SiPuts("Inicialitzat!\n"); Estat = 3; contadorInit = 69;/*  sprintf(tmp, "Eix X i Y: %d %d\n",Xmostra,Ymostra); SiPuts(tmp);*/
                }else{
                    Estat = 2; 
                }
                break;
            case 3:
//                if((Xoriginal != Xant) || (Yoriginal != Yant)){
                if((fabs(Xmostra - xcal) >= 10) || fabs(Ymostra - ycal) >= 10){
                    SiPuts("OBERT\n");
                    SiPuts("OBERT\n");
                    SiPuts("OBERT\n");
                    SiPuts("OBERT\n");
////                    sprintf(tmp, "Eix X: %d %d\n",Xant,Xoriginal); SiPuts(tmp);
////                    sprintf(tmp, "Eix Y: %d %d\n",Yant,Yoriginal); SiPuts(tmp);
                }   
                
                sprintf(tmp, "Eix X: %d\n",Xmostra); SiPuts(tmp);
                sprintf(tmp, "Eix Y: %d \n",Ymostra); SiPuts(tmp);
//                sprintf(tmp, "Eix Z: %d \n",Zmostra); SiPuts(tmp);
                Estat = 2; TiResetTics(T);
                break; 
                
        }
        
    }
}
void __interrupt() INTERRUPT_TMR2(void); 


int valorAngle(uint8_t eix){
    int tmp;
    I2C_start();
    WriteI2C((0x1D << 1 )| 0x00); /*NOP();*/
    I2C_wait();
    WriteI2C(eix); /*NOP();*/
    I2C_repeated_start();
    WriteI2C((0x1D << 1 )| 0x01); /*NOP();*/
    I2C_wait();
    SSP1CON2bits.RCEN = 1;
    WriteI2C(eix); /*NOP(); */                                       
    NotAckI2C();
    tmp = SSP1BUF;
    I2C_stop();
    SSP1CON2bits.RCEN = 0;
    return tmp;
}
void initBoard(){

    /** TRISx registers */ 
    TRISA = 0xF0; // A7-A6 are inputs, A5-A0 are outputs
    TRISB = 0x30; // B4, B5 are inputs.
    TRISC = 0x18; // C0 is an output, all others are inputs.
    /** ANSELx registers */
    ANSELA = 0x30; // 0x30 previously. 10
    ANSELB = 0xFF; //
    ANSELC = 0x00; // Turn off analog signals on Port C.

    /** ODx registers */ 
    ODCONA = 0x00;
    ODCONB = 0x00;
    ODCONC = 0x00;

    // PPS Unlock sequence
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x00;

    // Set the PPS for I2C pins over here until MCC can do this for us when the driver is released
    SSP1DATPPS = 0x13; //RC4 input is SDA1 (Page-235).
    SSP1CLKPPS = 0x14; //RC3 input is SCL1 (Page-235).
    RC3PPS = 0x15; //RC3 output is SCL1 (Page-239).
    RC4PPS = 0x14; //RC4 output is SDA1 (Page-239).
    RC5PPS = 0x10;
    // PPS lock sequence
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x01;
 
}
void initMSSP1_I2C(){
    OSCCON1 = 0x62;
    // CSWHOLD may proceed; SOSCPWR Low power; 
    OSCCON3 = 0x00;
    // MFOEN disabled; LFOEN disabled; ADOEN disabled; SOSCEN disabled; EXTOEN disabled; HFOEN disabled; 
    OSCEN = 0x00;
    // HFFRQ 4_MHz; 
    OSCFRQ = 0x02;
    // HFTUN 0; 
    OSCTUNE = 0x00;
    // I2C Mode Configuration.
    // Enable MSSP module.
    // SSP1CON1 = 0b0011000;
    SSP1CON1 = 0b00101000;

    // Slew Rate Control.
    SSP1STAT = 0x80; // This is correct for 100KHz BAUD - Slew rate control disabled.

    // Configure ACKSTAT, ACKDT, RCEN, PEN, RSEN and SEN - Normally these are cleared.
    SSP1CON2 = 0x10;

    // SBCDE, BOEN, SCIE, DHEN, and AHEN are all disabled, SDAHT is 100ns.
    SSP1CON3 = 0x00;

    // I2C BAUD Rate Setup.
    SSP1ADD = (_XTAL_FREQ/(4 * I2C1BAUD))-1; // 100KHz BAUD. Previously 0x4F

    // SCL/SDA PIN DIRECTION SETUP
    TRISC = 0x58;
    // clear the master interrupt flag
    PIR3bits.SSP1IF = 0;
    // enable the master interrupt
    PIE3bits.SSP1IE = 0; // 0 Temporarily disables, set to 1 to re-enable.

    // Enable MSSP module.
 //SSP1CON1bits.SSPEN = 1; 
}