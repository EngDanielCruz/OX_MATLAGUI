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
extern char str[16];

//*************************************************************************
//                          FUNCTIONS PROTOTYPES
//*************************************************************************

void UART_Init(void);
char readChar(void);
void readStr(uint8_t len);
void printChar(char);
void print_String( const char *str);
void print_uint( unsigned long u, uint8_t digits);
void printDouble( double val);
void process_command(uint8_t);
void process_REGISTER_command(void);
void print_int(  long i);
void Printfloat(float f, uint8_t decimal);
