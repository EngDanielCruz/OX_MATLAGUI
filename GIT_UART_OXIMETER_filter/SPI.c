/*
 * SPI.c
 *
 *  Created on: 05/09/2015
 *      Author: Dany
 */
#include <LM4F120E5QR.h>

void SPI_Init(){

//	1. Enable the SSI module using the RCGCSSI register
	SYSCTL->RCGCSSI|=(1<<0);
//	2. Enable the clock to the appropriate GPIO module via the RCGCGPIO register
	SYSCTL->RCGCGPIO |=(1<<0);
	while((SYSCTL->RCGCGPIO & 0x01)==0){};
//	3. Set the GPIO AFSEL bits for the appropriate pins
	GPIOA->AFSEL |=(1<<2)|(1<<4)|(1<<5);
	GPIOA->AFSEL &=~(1<<3);               // take control of the CSN Pin disable alternate function
										 //	otherwise cs pin will pulse every 8bit of data. we must send several bytes in one csn low cicle
//	4. Configure the PMCn fields in the GPIOPCTL register to assign the SSI signals to the appropriate pins
	GPIOA->PCTL = (2<<8)|(2<<12)|(2<<16)|(2<<20);
//  P3 config
	GPIOA->DIR |=(1<<3);  //P3 is an output
	GPIOA->DEN |=0x3C;    // digital enable
	GPIOA->DATA |=(1<<3); // CS=1

//	For each of the frame formats, the SSI is configured using the following steps:
//	1. Ensure that the SSE bit in the SSICR1 register is clear before making any configuration changes.
	SSI0->CR1 &=~(1<<1) ;
	//	2. Select whether the SSI is a master or slave:
	SSI0->CR1 = 0x00000000 ;
//	3. Configure the SSI clock source by writing to the SSICC register.
	SSI0->CC =0x5;
//	4. Configure the clock prescale divisor by writing the SSICPSR register.
	SSI0->CPSR =0x2;  // =CPSDVSR -> bus clock must be at least 2 times faster than the SSICLk...SSICLk<25MHz
//	5. Write the SSICR0 register with the following configuration: Mode0=default state
	SSI0->CR0 &=~(0x1<<8); // SCR = 0x0, SSIClk = SysClk / (CPSDVSR * (1 + SCR)) =>8x10e6 = 40x10e6 / (1 * (1 + 0x4))
	SSI0->CR0 |=(0x07<<0);  // 8bits data size
//	7. Enable the SSI by setting the SSE bit in the SSICR1 register.
	SSI0->CR1 |=(1<<1);

}

uint8_t SPI_transfer(uint8_t data){
	while((SSI0->SR & (1<<0)) == 0 ){};  // wait until FIFO empty
	SSI0->DR = data;
	while((SSI0->SR & (1<<2)) == 0 ){};  // Wait until response
	return SSI0->DR;
}

