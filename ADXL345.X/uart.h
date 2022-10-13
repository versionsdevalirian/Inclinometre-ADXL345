/* 
 * File:   uart.h
 * Author: DavidPallàs
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
// Pre: retorna el nombre de caràcters rebuts que no s'han recollit
// amb la funció GetChar encara
unsigned char SiGetChar(void);
// Pre: SiCharAvail() és major que zero
// Post: Treu i retorna el primer caràcter de la cua de recepció.
void SiSendChar(unsigned char c);
void SiPuts(char *s);

#endif	/* UART_H */

