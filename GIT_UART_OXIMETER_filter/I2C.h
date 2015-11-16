/*
 * I2C.h
 *
 *  Created on: 11/10/2015
 *      Author: Dany
 */

#ifndef I2C_H_
#define I2C_H_
#endif /* I2C_H_ */

#define I2C_READ 0x1
#define I2C_WRITE 0x0
#define I2C_MCS_ACK             0x00000008  // Data Acknowledge Enable
#define I2C_MCS_DATACK          0x00000008  // Acknowledge Data
#define I2C_MCS_ADRACK          0x00000004  // Acknowledge Address
#define I2C_MCS_STOP            0x00000004  // Generate STOP
#define I2C_MCS_START           0x00000002  // Generate START
#define I2C_MCS_ERROR           0x00000002  // Error
#define I2C_MCS_RUN             0x00000001  // I2C Master Enable
#define I2C_MCS_BUSY            0x00000001  // I2C Busy

#define MAXRETRIES              5           // number of receive attempts before giving up

void I2C_Init(void);
uint32_t I2C_writeByte(uint8_t dataByte, uint8_t R_W, uint8_t Conditions);
uint32_t I2C_ReadByte(int8_t Conditions);
void setSlaveAddress(uint8_t slaveAddress);
void I2C_Stop();
