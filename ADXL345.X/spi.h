/* 
 * File:   spi.h
 * Author: DavidPallàs
 *
 * Created on 25 de agosto de 2022, 12:55
 */

#ifndef SPI_H
#define	SPI_H
#include <xc.h>

void WriteSpi(unsigned char Address, unsigned char data);
int  ReadSpi(unsigned char Address);

void SPIInit (void);

#endif	/* SPI_H */

