#include "timer.h"
#include "uart.h"
#define         TI_FALS                         0
#define         TI_CERT                         1
#define         TI_NUMTIMERS                    26

// El següent define marca la distància temporal màxima
// entre un gettics i un resettics
//#define         TI_MAXTICS                      8640000000L //Suficient per aprox. 100 dies
#define         TI_MAXTICS                      30000 //Suficient per aprox. 30 o 40 segons
struct Timer {
	unsigned int h_TicsInicials;
	unsigned char b_busy;
} s_Timers[TI_NUMTIMERS]; 
static unsigned int  h_Tics=0;static unsigned char counter;
void TiInitTimer(){
// c INTCON1bits.NSTDIS = 1; // No nested interrupts

	for (counter=0;counter<TI_NUMTIMERS;counter++) {
		s_Timers[counter].b_busy=TI_FALS;
	}
	h_Tics=0;
	// Suposo que anem a 2Mhz de clock, el timer usa per defecte FOSC/2 (1MHz)
	// que vol dir 1us ns de rellotge 
	// Suposo que anem a 16Mhz de clock, el timer usa per defecte FOSC/2 (8MHz)
	// que vol dir 125s ns de rellotge 125*8000 = 1ms
	
	// La resta de valors de T1CON per defecte
//	PR3 = 0x0f;		// Nominal simulation value for 1ms period
//	TMR2L = 0xa0;		// Nominal simulation value for 1ms period
	// Activo la interrupció del timer 1
    T2CLKCON = 0x01;
    T2PR = 1000;		// Nominal simulation value for 1ms period //1000

    T2TMR = 0x00;
    PIR4bits.TMR2IF = 0;
    PIE4bits.TMR2IE = 1;	
    T2CON  = 0x80;		// Activo el timer

}
int TiGetTimer(void) {
// Post: retorna un numero de timer lliure
    unsigned char counter=0;
	while (s_Timers[counter].b_busy==TI_CERT) {
		counter++;
		if (counter == TI_NUMTIMERS) return -1;
	}
	s_Timers[counter].b_busy=TI_CERT;
	return (counter);
}
void TiResetTics(int Handle) {
// Pre: t es un valor retornat per TiGetTimer
// Post: posa a zero els tics d'aquest timer
	//__DI(); h_Tics integer, per tant, indivisible
	s_Timers[Handle].h_TicsInicials=h_Tics;
	//__EI();
}
unsigned int TiGetTics(int Handle) {
// Pre: t es un valor retoran per TiGetTimer
// Post: retorna el nombre de tics (ms) des de la ultima crida a TiResetTics
    volatile unsigned int actual;
	actual  = h_Tics; // indivisible si és un int, clar.
    return (actual-(s_Timers[Handle].h_TicsInicials));
}
 
 void __interrupt() INTERRUPT_TMR2(void){
  bool status =  PIR4bits.TMR2IF;
//    SiSendChar(status);
    if(status){
        h_Tics++;
        if (h_Tics>=TI_MAXTICS) {
		// Abans que dongui la volta, tots avall (cada 30s)
            for (counter=0;counter<TI_NUMTIMERS;counter++)
                if (s_Timers[counter].b_busy==TI_CERT)
                    s_Timers[counter].h_TicsInicials -= h_Tics;
            h_Tics=0;
        }
        // Clearing IF flag.
        PIR4bits.TMR2IF = 0;     
    }
}
