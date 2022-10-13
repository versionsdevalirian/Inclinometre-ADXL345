/* 
 * File:   uart.h
 * Author: DavidPall�s
 *
 * Created on 24 de agosto de 2022, 18:11
 */

#ifndef UART_H
#define	UART_H

#include <stdint.h>
#include <stdbool.h>
#include <xc.h>
void SiInit(int Baud);
// Pre: Baud is B9600, B19200, B57600 or B115200
void SiClearBuffer(void);
int  SiCharAvail(void);
// Pre: retorna el nombre de car�cters rebuts que no s'han recollit
// amb la funci� GetChar encara
unsigned char SiGetChar(void);
// Pre: SiCharAvail() �s major que zero
// Post: Treu i retorna el primer car�cter de la cua de recepci�.
void SiSendChar(unsigned char c);
void SiPuts(char *s);

#endif	/* UART_H */

