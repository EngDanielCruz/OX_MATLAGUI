/*
 * UART.c
 *
 *  Created on: 15/11/2015
 *      Author: Dany
 */


#include <LM4F120E5QR.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "UART.h"
#include "MAX30100.h"

//*****************************************************************************
//                          Global Variables
//*****************************************************************************
char str[15];
extern struct confcom configValues;
extern struct configregister configresvalue ;
//*************************************************************************
//                          FUNCTIONS
//*************************************************************************
void UART_Init(){
    // 1. Enable the UART module using the RCGCUART register (see page 344).
        SYSCTL->RCGCUART |= (1<<0);

        // 2. Enable the clock to the appropriate GPIO module via the RCGCGPIO register (see page 340).
        // To find out which GPIO port to enable, refer to Table 23-5 on page 1351.
        SYSCTL->RCGCGPIO |= (1<<0);

        // 3. Set the GPIO AFSEL bits for the appropriate pins (see page 671). To determine which GPIOs to
        // configure, see Table 23-4 on page 1344
        GPIOA->AFSEL = (1<<1)|(1<<0);

        // 4. Configure the GPIO current level and/or slew rate as specified for the mode selected (see
        // page 673 and page 681

        // 5. Configure the PMCn fields in the GPIOPCTL register to assign the UART signals to the appropriate
        // pins (see page 688 and Table 23-5 on page 1351).
        GPIOA->PCTL = (1<<0)|(1<<4);

        GPIOA->DEN = (1<<0)|(1<<1);

        // Find  the Baud-Rate Divisor
        // BRD = 40,000,000 / (16 * 115200) = 21.7014
        // UARTFBRD[DIVFRAC] = integer(0.7014 * 64 + 0.5) = 45


        // With the BRD values in hand, the UART configuration is written to the module in the following order

        // 1. Disable the UART by clearing the UARTEN bit in the UARTCTL register
        UART0->CTL &= ~(1<<0);

        // 2. Write the integer portion of the BRD to the UARTIBRD register
        UART0->IBRD = 21;
        // 3. Write the fractional portion of the BRD to the UARTFBRD register.
        UART0->FBRD = 45;

        // 4. Write the desired serial parameters to the UARTLCRH register (in this case, a value of 0x0000.0060)
        UART0->LCRH = (0x3<<5);     // 8-bit, no parity, 1-stop bit

        // 5. Configure the UART clock source by writing to the UARTCC register
        UART0->CC = 0x0;

        // 6. Optionally, configure the µDMA channel (see “Micro Direct Memory Access (µDMA)” on page 585)
        // and enable the DMA option(s) in the UARTDMACTL register

        // 7. Enable the UART by setting the UARTEN bit in the UARTCTL register.
        UART0->CTL = (1<<0)|(1<<8)|(1<<9);
}


char readChar(void)
{
    char c;
    c = UART0->DR;
    return c;
}

void printChar(char c)
{
    while((UART0->FR & (1<<5)) != 0);
    UART0->DR = c;
}

void print_uint( unsigned long u, uint8_t digits)
{
    if (digits > 10) digits = 10;

    char digs[11], reversed[11];
    uint8_t a = 0, b, c;

    do
    {
        b = u % 10;
        digs[a] = b + 48;   // Convert to digit ASCII
        u /= 10;
        a++;
    } while (u);

    // Add padding zeroes if necessary
    b = 0;
    if (a < digits)
    {
        for (; b < digits - a; b++)
            reversed[b] = '0';
    }

    // Reverse the digits into most significant to least significant
    for (c = 0; b < digits; b++, c++)
        reversed[b] = digs[a - c - 1];

    reversed[digits] = '\0';
    print_String( (char *) reversed);
}

void print_String( const char *str)
{
    int a = 0;
    while (str[a] != '\0')
    {
        if (str[a] == '\n')
        {
            printChar('\r');
            printChar('\n');
        }
        else printChar(str[a]);
        a++;
    }
}



void printDouble( double val){
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)

    print_uint((int)val,5);  //prints the int part
    printChar('.'); // print the decimal point
   unsigned int frac;

       frac = (val - (int)val) * 1000;
       print_uint(frac,3);
}

void readStr(uint8_t len){
   uint8_t i=0; // control counter to prevent infinite loop in case of receive failure

for (i=0;i<len;i++){
    str[i] =0;
}
       i=0;
        while (i<len){
            // wait for the new character
            while((UART0->FR & (1<<4)) == 0){
                // if is the end of the string
                if (str[i] == '\n' || str[i] == '\r'){
                    break;
                }
                str[i]= readChar();
                i++;
            }

        }
}

void process_command(void){
    uint8_t i=0;
    uint8_t j=0;
    uint8_t auxcnt[3];
    char auxarray[6];
    for(i=0; i<12; i++){
        if (str[i] == ','){
            auxcnt[j]=i;
            j++;
        }
    }

   configValues.NofSamples= atoi(strncpy ( auxarray, str, auxcnt[0] ));
   configValues.alpha=atof(memcpy ( auxarray, &str[auxcnt[0]+1], auxcnt[1] ));
   configValues.taps = atoi(strncpy ( auxarray, (str+auxcnt[1]+1), auxcnt[2] ));

}

void process_REGISTER_command(){
    uint8_t i=0;
    uint8_t j=0;
    uint8_t auxcnt[4];
    char auxarray[15];
    for(i=0; i<16; i++){
        if (str[i] == ','){
            auxcnt[j]=i;
            j++;
        }
    }

    configresvalue.modeconfig = atoi(strncpy ( auxarray, str, auxcnt[0]-1 ));
    configresvalue.spo2config=atoi(strncpy ( auxarray, (str+auxcnt[0]+1), auxcnt[1] ));
    configresvalue.intconfig = atoi(strncpy ( auxarray, (str+auxcnt[1]+1), auxcnt[2] ));
    configresvalue.ledconfig=atoi(strncpy ( auxarray, (str+auxcnt[2]+1), auxcnt[3] ));


}










