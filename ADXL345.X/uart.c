#include "uart.h"
#include <xc.h> 

#define BUFFER_SIZE    512
#define B4800     5
#define B9600     0
#define B19200    1
#define B38400	  2
#define B57600    3
#define B115200   4

//
//--------------------------------CONSTANTS---AREA-----------
//
//#define BUFFER_SIZE      (16*(64+20))
//
//---------------------------End--CONSTANTS---AREA-----------
//


//
//--------------------------------VARIABLES---AREA-----------
//
//static char Buffer[BUFFER_SIZE];
static int Inici, Fi, Quants;
//
//---------------------------End--VARIABLES---AREA-----------
//

//
//--------------------------------PROTOTIPUS--AREA-----------
//
static void Posa(char);
static char Treu(void);
#define UARTAturada() (IEC0bits.U1TXIE == 0)

void SiInit(int Baud) {
// Pre: Baud is B9600, B19200, B57600 or B115200
    Inici = Fi = Quants = 0; PIE3bits.RCIE = 1;PIR3bits.RCIF = 0;RC1STAbits.CREN = 1;
    TX1STA = 0x24; 
    BAUD1CON = 0x08;
    
    RC1STA = 0x90;
    // Baud Rate = 19200; SP1BRGL 12; 
    SP1BRGL = 0x0C;

    // Baud Rate = 19200; SP1BRGH 0; 
    SP1BRGH = 0x00;   
	
}
void SiSendChar(unsigned char c) {
// Post: espera que el caràcter anterior s'hagi enviat i envia aquest
//    while(U1STAbits.UTXBF == 1){ClrWdt();}
	while (TX1STAbits.TRMT == 0){CLRWDT();}
//        while (PIR3bits.TXIF == 0){ CLRWDT();}

	TX1REG = c;
}

void SiPuts(char *s) {
	while(*s) SiSendChar(*s++);
}
int SiCharAvail(void) {
// Pre: retorna el nombre de caràcters rebuts que no s'han recollit
// amb la funció GetChar encara
	if (RC1STAbits.OERR == 1) RC1STAbits.CREN = 0;
    //return Inici != Fi;
	return Quants;
}

unsigned char SiGetChar(void) {
// Pre: SiCharAvail() és major que zero
// Post: Treu i retorna el primer caràcter de la cua de recepció.
	return Treu();
}

#define BUFFER_SIZE_RX3     64
static unsigned char BufferRx3[BUFFER_SIZE_RX3];
static void Posa(char c) {
//	if (Quants >= BUFFER_SIZE-2) return;
	BufferRx3[Inici++] = c;
	if (Inici == BUFFER_SIZE_RX3) Inici = 0;
	Quants++;
}
static char Treu(void) {
	char tmp;
	tmp = BufferRx3[Fi++];
	if (Fi == BUFFER_SIZE_RX3) Fi = 0;
	Quants --;
	return tmp;
}
 

