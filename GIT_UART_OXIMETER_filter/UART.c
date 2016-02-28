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
char str[16];
extern struct confcom configValues;
extern struct configregister configresvalue ;
extern struct samplingoptions  samplingOptions;
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
        // BRD = 80,000,000 / (16 * 115200) = 43.4028
        // UARTFBRD[DIVFRAC] = integer(0.4028 * 64 + 0.5) = 26


        // With the BRD values in hand, the UART configuration is written to the module in the following order

        // 1. Disable the UART by clearing the UARTEN bit in the UARTCTL register
        UART0->CTL &= ~(1<<0);

        // 2. Write the integer portion of the BRD to the UARTIBRD register
        UART0->IBRD = 43;
        // 3. Write the fractional portion of the BRD to the UARTFBRD register.
        UART0->FBRD = 26;

        // 4. Write the desired serial parameters to the UARTLCRH register (in this case, a value of 0x0000.0060)
        UART0->LCRH = (0x3<<5);     // 8-bit, no parity, 1-stop bit

        // 5. Configure the UART clock source by writing to the UARTCC register
        UART0->CC = 0x0;

        // 6. Optionally, configure the µDMA channel (see “Micro Direct Memory Access (µDMA)” on page 585)
        // and enable the DMA option(s) in the UARTDMACTL register

        // 7. Enable the UART by setting the UARTEN bit in the UARTCTL register.
        UART0->CTL = (1<<0)|(1<<8)|(1<<9);
}
/*
 * AUX FUNCTIONs
 */
static unsigned long power(unsigned long base, int exp)
{
    int res = 1;
    int i;
    for (i = 0; i < exp; i++)
        res *= base;
    return res;
}

static inline float fmod_(float x, float y)
{
    return x - y * ((long) (x / y));
}


/*
 *
 */
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

void print_int(  long i){

    char digs[12], reversed[12];
        uint8_t a = 0, b, neg = 0;

        if (i < 0)
        {
            neg = 1;
            i = -i;
        }

        do
        {
            b = i % 10;
            digs[a] = b + 48;   // Convert to digit ASCII
            i /= 10;
            a++;
        } while (i);

        // Reverse the digits into most significant to least significant
        if (neg)
        {
            reversed[0] = '-';
            for (b = 1; b < a + neg; b++)
                reversed[b] = digs[a - b];
            a += 1; // Extend the length of the string by 1 due to - sign
        }
        else for (b = 0; b < a; b++)
            reversed[b] = digs[a - b - 1];

        reversed[a] = '\0';
        print_String((char *) reversed);

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
/*
 * void Printfloat:
 * Outputs the floating point number f to the given number of decimal
    places over UART.
    decimal is the number of decimal places to output.
*/
void Printfloat(float f, uint8_t decimal)
{
    char digs[12], reversed[12];
    uint8_t a = 0, b, neg = 0;

    if (f < 0)
    {
        neg = 1;
        f = -f;
    }

    if (decimal)
    {
        f *= power(10, decimal);

        for (a = 0; a < decimal; a++)
        {
            b = (uint8_t) fmod_(f, 10);
            digs[a] = b + 48;   // Convert to digit ASCII
            f /= 10;
        }

        digs[a] = '.';
        a++;
    }

    do
    {
        b = (uint8_t) fmod_(f, 10);
        digs[a] = b + 48;   // Convert to digit ASCII
        f /= 10;
        a++;
    } while ((int) f && a < 11);

    // Reverse the digits into most significant to least significant
    if (neg)
    {
        reversed[0] = '-';
        for (b = 1; b < a + neg; b++)
            reversed[b] = digs[a - b];
        a += 1; // Extend the length of the string by 1 due to - sign
    }
    else for (b = 0; b < a; b++)
        reversed[b] = digs[a - b - 1];

    reversed[a] = '\0';
    print_String((char *) reversed);
}



void readStr(uint8_t len){
   uint8_t i=0; // control counter to prevent infinite loop in case of receive failure
   uint8_t endloop=0;
for (i=0;i<len;i++){
    str[i] =0;
}
       i=0;
        while ((i<len)&&(endloop==0)){
            // wait for the new character
            while((UART0->FR & (1<<4)) == 0){
                // if is the end of the string
                str[i]= readChar();
                if (str[i] == 10){
                    endloop=1;
                    break;
                }
               i++;
            }

        }
}

void process_command(uint8_t choose){ // choose which kind of commands should be processed
    uint8_t i=0;
    uint8_t j=0;
    uint8_t auxcnt[4]={0,0,0,0};
    char auxarray[8]={0,0,0,0,0,0,0};
    for(i=0; i<12; i++){
        if (str[i] == ','){
            auxcnt[j]=i;
            j++;
        }
    }
    if (choose==0){
       configValues.NofSamples= atoi(strncpy ( auxarray, str, auxcnt[0] ));
       configValues.alpha=atof(memcpy ( auxarray, &str[auxcnt[0]+1], auxcnt[1] ));
       configValues.taps = atoi(strncpy ( auxarray, (str+auxcnt[1]+1), auxcnt[2] ));
       configValues.filt_type = atoi(strncpy ( auxarray, (str+auxcnt[2]+1), auxcnt[3] ));
    }else {
        samplingOptions.IRRawPlot = atoi(strncpy ( auxarray, str, auxcnt[0] ));
        samplingOptions.REDRawPlot = atoi(strncpy ( auxarray, (str+auxcnt[0]+1), auxcnt[1] ));
        samplingOptions.IRPlot = atoi(strncpy ( auxarray, (str+auxcnt[1]+1), auxcnt[2] ));
        samplingOptions.RedPlot = atoi(strncpy ( auxarray, (str+auxcnt[2]+1), auxcnt[3] ));
    }

}



void process_REGISTER_command(){
    uint8_t i=0;
    uint8_t j=0;
    uint8_t PWcont;
    uint8_t SPO_SR;
    uint8_t auxcnt[4]={0,0,0};
    char auxarray[5]={0,0,0,0,0};
    for(i=0; i<16; i++){
        if (str[i] == ','){
            auxcnt[j]=i;
            j++;
        }
    }

    configresvalue.modeconfig = atoi(strncpy ( auxarray, str, auxcnt[0] ));
    configresvalue.spo2config=atoi(strncpy ( auxarray, (str+auxcnt[0]+1), auxcnt[1] ));
    configresvalue.intconfig = atoi(strncpy ( auxarray, (str+auxcnt[1]+1), auxcnt[2] ));
    configresvalue.ledconfig=atoi(strncpy ( auxarray, (str+auxcnt[2]+1), auxcnt[3] ));

    // extract the value of PW (ADC resolution). This vales is used MAX30100.c for shifting the values from max fifo.
    PWcont = (configresvalue.spo2config & (0x3));
    if(PWcont==3){
        configresvalue.PWcontrol=0;
    }else if(PWcont==2){
        configresvalue.PWcontrol=1;
    }else if(PWcont==1){
        configresvalue.PWcontrol=2;
    }else{
        configresvalue.PWcontrol=3;
    }
    // extract the value of SPO2_SR (Sample rate). This vales is used MAX30100.c to define time sample window. time Window in seconds is define in MAX30100.h by macro
    SPO_SR = (configresvalue.spo2config & (0x1C));
    if(SPO_SR==0x0){
        configresvalue.SamplesWindow=(50*SAMPLES_TIME);
    }else if(SPO_SR==0x4){
        configresvalue.SamplesWindow=(100*SAMPLES_TIME);
    }else if(SPO_SR==0xC){
        configresvalue.SamplesWindow=(200*SAMPLES_TIME);
    }else if(SPO_SR==0x10){
        configresvalue.SamplesWindow=(400*SAMPLES_TIME);
    }else if(SPO_SR==0x14){
        configresvalue.SamplesWindow=(600*SAMPLES_TIME);
    }else if(SPO_SR==0x18){
        configresvalue.SamplesWindow=(800*SAMPLES_TIME);
    }else{
        configresvalue.SamplesWindow=(1000*SAMPLES_TIME);
    }


}










