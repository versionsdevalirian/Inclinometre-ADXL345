/* 
 * File:   timer.h
 * Author: DavidPallàs
 *
 * Created on 24 de agosto de 2022, 18:40
 */
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#ifndef TIMER_H
#define	TIMER_H
void TiInitTimer();
bool TMR2_HasOverflowOccured(void);
int TiGetTimer(void);
// Post: retorna un numero de timer lliure
void TiResetTics(int t);
// Pre: t es un valor retornat per TiGetTimer
// Post: posa a zero els tics d'aquest timer
unsigned int  TiGetTics(int t);
// Pre: t es un valor retoran per TiGetTimer
// Post: retorna el nombre de tics (ms) des de la ultima crida a TiResetTics

#endif	/* TIMER_H */

