/* 
 * File:   i2c.h
 * Author: Jan Kubovy <jan@kubovy.eu>
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h> 
    // Configuration which SPP module to use.
//#ifndef I2C_SSPSTAT
#define I2C_SSPSTAT SSP1STAT
#define I2C_SSPCON2 SSP1CON2
#define I2C_BUFFER SSP1BUF
//#endif

// Operation modes
#define I2C_MODE_WRITE 0x00
#define I2C_MODE_READ 0x01
    
// Acknowledgments
#define I2C_ACK 0x01
#define I2C_NACK 0x00

inline void I2C_wait(void);

void I2C_start(void);

void I2C_repeated_start(void);
void NotAckI2C(void);
void WaitAck(void);
void WriteI2C(unsigned char byte);
void AckI2C(void);
void I2C_stop(void) ;
void I2C_select(uint8_t address, bool mode);

uint8_t I2C_read(uint8_t ack);

void I2C_write(uint8_t data);

void I2C_send(uint8_t address, uint8_t data);


void I2C_sendN(uint8_t address, uint8_t n, ...);
uint8_t I2C_receive(uint8_t address, uint8_t reg);
#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */