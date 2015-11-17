/*
 * UART.h
 *
 *  Created on: 15/11/2015
 *      Author: Dany
 */

#ifndef UART_H_
#define UART_H_
#endif /* UART_H_ */

//*****************************************************************************
//                          #defines
//*****************************************************************************


//*****************************************************************************
//                            Variables
//*****************************************************************************
extern char str[13];

//*************************************************************************
//                          FUNCTIONS PROTOTYPES
//*************************************************************************

void UART_Init(void);
char readChar(void);
void readStr(void);
void printChar(char);
void print_String( const char *str);
void print_uint( unsigned long u, uint8_t digits);
void printDouble( double val);
void process_command(void);
