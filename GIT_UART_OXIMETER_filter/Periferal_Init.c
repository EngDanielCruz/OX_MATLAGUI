/*
 * Periferal_Init.c
 *
 *  Created on: 05/09/2015
 *      Author: Dany
 */

/* PORTF P2 -> blue led
 * PORTD P2 -> ADC -> Triggered by Timer3A if 80 MHz clock is use, Max time =53.687s
 * PORTC P4 -> Edge Interrupt
 * PWM -> PORTB P4
 * 	   -> Timer1A
 * Timer0A -> Interrupt
 * Timer2A -> encoder input edge counter
 *		   -> PORTB P0
 */
#include <LM4F120E5QR.h>
#include "Periferal_Init.h"

void Interrupt_IRQ(){
	// Enable the clock to the port by setting the appropriate bits in the RCGCGPIO register
	SYSCTL->RCGCGPIO |= (1<<2);
	// Set the direction of the GPIO port pins by programming the GPIODIR register.  '1'indicates output , '0' indicates input
	GPIOC->DIR &= ~(0<<4);
	// To enable GPIO pins as digital I/Os, set the appropriate DEN bit in the GPIODEN register
	GPIOC->DEN |=(1<<4);
	// Program the GPIOIS, GPIOIBE, GPIOBE, GPIOEV, and GPIOIM registers to configure the type, event, and mask of the interrupts for each port
	GPIOC->IS &=~(1<<4);
	GPIOC->IBE &=~(1<<4); //Interrupt generation is controlled by the GPIO Interrupt Event
	GPIOC->IEV &=~(1<<4);  // A falling edge or a High level on the corresponding pin triggers an interrupt.
	GPIOC->ICR |=(1<<4);  // The corresponding interrupt is cleared.
	GPIOC->IM |=(1<<4);   // The interrupt from the corresponding pin is sent to the interrupt controller.
	NVIC_SetPriority(GPIOC_IRQn, 0);
	//NVIC_EnableIRQ(GPIOC_IRQn);
	//MAX30100 interrupt
	    // Enable the clock to the port by setting the appropriate bits in the RCGCGPIO register
	        SYSCTL->RCGCGPIO |= (1<<1);
	        // Set the direction of the GPIO port pins by programming the GPIODIR register.  '1'indicates output , '0' indicates input
	        GPIOB->DIR &= ~(0<<1);
	        // To enable GPIO pins as digital I/Os, set the appropriate DEN bit in the GPIODEN register
	        GPIOB->DEN |=(1<<1);
	        // 4. Enable the INT pin for open-drain operation
	        //GPIOB->ODR |=(1<<1);
	        // Program the GPIOIS, GPIOIBE, GPIOBE, GPIOEV, and GPIOIM registers to configure the type, event, and mask of the interrupts for each port
	        GPIOB->IS &=~(1<<1);
	        GPIOB->IBE &=~(1<<1); //Interrupt generation is controlled by the GPIO Interrupt Event
	        GPIOB->IEV &=~(1<<1);  // A falling edge or a LOW level on the corresponding pin triggers an interrupt.
	        GPIOB->ICR |=(1<<1);  // The corresponding interrupt is cleared.
	        GPIOB->IM |=(1<<1);   // The interrupt from the corresponding pin is sent to the interrupt controller.
	        NVIC_SetPriority(GPIOB_IRQn, 0);
	        //NVIC_EnableIRQ(GPIOB_IRQn);
}


void PWM_Init(uint16_t period, uint16_t high){

	// activate timer1
	 SYSCTL->RCGCTIMER |= (1<<1); //0b0000 0010
	// activate port B
	 SYSCTL->RCGCGPIO |= (1<<1); // duplicado com portledinit
	// ready?
	 while((SYSCTL->PRGPIO & 0x0002) == 0){};
	// enable alt funct on PB4
	 GPIOB->AFSEL |= (1<<4);
	// enable digital I/O on PB4
	 GPIOB->DEN |= (1<<4);
	// configure PB4 as T1CCP0
	 GPIOB->PCTL = (GPIOB->PCTL & 0xFFF0FFFF)+0x00070000;
	// disable analog functionality on PB4
	 GPIOB->AMSEL &= ~0x10;
	// 1. Ensure the timer is disabled (the TnEN bit is cleared) before making any changes.
	TIMER1 ->CTL &= ~(1<<0);
	// 2. Write the GPTM Configuration (GPTMCFG) register with a value of 0x0000.0004.
	TIMER1 -> CFG = 0x00000004;
	// 3. In the GPTM Timer Mode (GPTMTnMR) register, set the TnAMS bit to 0x1, the TnCMR bit to 0x0, and the TnMR field to 0x2.
	TIMER1 -> TAMR = 0x0000000A;
	// 4. Configure the output state of the PWM signal (whether or not it is inverted) in the TnPWML field of the GPTM Control (GPTMCTL) register.

	// 5. If a prescaler is to be used, write the prescale value to the GPTM Timer n Prescale Register (GPTMTnPR).

	// 6. If PWM interrupts are used, configure the interrupt condition in the TnEVENT field in the GPTMCTL register and enable the interrupts by setting the TnPWMIE bit in the GPTMTnMR
	// register. Note that edge detect interrupt behavior is reversed when the PWM output is inverted (see page 696).

	// 7. Load the timer start value into the GPTM Timer n Interval Load (GPTMTnILR) register.
	TIMER1 -> TAILR = period-1;  // timer start value
	// 8. Load the GPTM Timer n Match (GPTMTnMATCHR) register with the match value.
	TIMER1 -> TAMATCHR = period-high-1;  // duty cycle = high/period
	// 9. Set the TnEN bit in the GPTM Control (GPTMCTL) register to enable the timer and begin generation of the output PWM signal.
	TIMER1 -> CTL |= (1<<0); //0b0000 0001


}

void PWM_Duty(uint16_t high){

	TIMER1 -> TAMATCHR = (TIMER1 -> TAILR) - high;   // duty cycle = high/period
}


void ADC_initTimer3(uint32_t period){

	//---Module Initialization---

//1. Enable the ADC clock using the RCGCADC register (see page 326).
	SYSCTL->RCGC0 |= (1<<16);  // activate ADC0
	int counter=0;
while (counter <1000){          // delay to stabilize the clock
	++counter;
}
//2. Enable the clock to the appropriateGPIO, GPIOD, modules via the RCGCGPIO register (see page 314).
//To find out which GPIO ports to enable, refer to “Signal Description” on page 760.
	SYSCTL->RCGCGPIO |= (1<<3);	// Activate PORTD clock

	 counter=0;
while (counter <1000){          // delay to stabilize the clock
	++counter;
}
//3. Set the GPIOD[2,3] AFSEL bits for the ADC input pins (see page 630). To determine which GPIOs to
//configure, see Table 21-4 on page 1137.
	GPIOD->DIR &= ~(1<<2);		// config PD2 As a input
	GPIOD->AFSEL |= (1<<2);		// EN alternate function on PD2

//4. Configure the AINx signals to be analog inputs by clearing the corresponding DEN bit in the
//GPIO Digital Enable (GPIODEN) register (see page 641).
	GPIOD->DEN &= ~(1<<2);		// Disable digital I/o on PD2

//5.Disable the analog isolation circuit for all ADC input pins that are to be used by writing a 1 to
//the appropriate bits of the GPIOAMSEL register (see page 646) in the associated GPIO block.
	GPIOD->AMSEL |= (1<<2); 		// The analog function of the pin is enabled, the isolation is disabled, and the pin is capable of analog functions.

//6. If required by the application, reconfigure the sample sequencer priorities in the ADCSSPRI register.

	// Timer3A config.
//Enable and provide a clock to 16/32-bit general purpose timer module 0 in Run mode (enable timer0)
	SYSCTL->RCGCTIMER |= (1<<3); //activate timer3 0b0000 1000
//disable timerA by giving a value of 0 to bit 0
	TIMER3->CTL &= ~(1<<0);
// enable timer3A trigger to ADC
	TIMER3->CTL |= (1<<5);
// configure for 32-bit timer mode
	TIMER3->CFG = 0x00000000;
//This value set to the GPTMTAMR register allows us to choose periodic mode
	TIMER3->TAMR |= (0x2<<0);
//Setting TACDIR(bit 4) to 0 in order to make the timer count down
	TIMER3->TAMR &= ~(1<<4);
// prescale value for trigger
	TIMER3->TAPR =0x0000;   // if 80 MHz clock is use Max time =53.687s
//Loads the start value into to the GPTMTnILR
	TIMER3->TAILR = period-1; // start value for trigger
// disable all interrupts
	TIMER3->IMR = 0x00000000;
// enable timer0A
	TIMER3->CTL |= (1<<0);

	//---Sample Sequencer Configuration--- sample sequencer 2

//1. Disable the sequencer during programming clearing the corresponding ASENn bit in the ADCACTSS register
	ADC0->ACTSS &= ~(1<<2);  // disable sample sequencer 2 during config

//2. Configure the trigger event for the sample sequencer in the ADCEMUX register. (default=0x0)
	ADC0->EMUX = (0x5<<8);   // TIMER trigger- In addition, the trigger must be enabled with the TnOTE bit 	in the GPTMCTL register

//3. For each sample in the sample sequence, configure the corresponding input source in the ADCSSMUXn register.
	ADC0->SSMUX2 = 0x0005; 	// set channel 5 to mux0

//4. For each sample in the sample sequence, configure the sample control bits in the corresponding nibble in the ADCSSCTLn register.
	ADC0->SSCTL2 = 0x0006; 	//1 samples END2 bit is set.

//5. If interrupts are to be used, set the corresponding MASK bit in the ADCIM register.
	ADC0->IM |= (1<<2);		// enable interrupt on Sequence sample 2 (SS2)

//6. Enable the sample sequencer logic by setting the corresponding ASENn bit in the ADCACTSS register.
	ADC0->ACTSS |= (1<<2);  // Enable sample sequencer 2
//This bit is cleared by writing a 1. Clearing thi bit also clears the INR3 bit in the ADCRIS register.
	ADC0->ISC |= (1<<2);
	NVIC_SetPriority(ADC0SS2_IRQn,0);
	//NVIC_EnableIRQ(ADC0SS2_IRQn); //from CMSIS Library

}
/*
// Used for sofware trigger
unsigned long Read_ADC_SS2(){

	unsigned long Result;
	ADC0->PSSI = (1<<2); 		//Begin sampling on Sample Sequencer 2
	while ((ADC0->RIS & 0x04) == 0){
			}		// wait for the conversion done
	//ADC0->SSFIFO2 & 0xFFF;
	Result = ADC0->SSFIFO2 & 0xFFF;		// read the result
	ADC0->ISC = (1<<2); 				// clear flag

	return Result;
}*/


void Timer_Init(){
	/*****One-Shot/Periodic Timer Mode*****/ //[pg.683]

	  //Enable and provide a clock to 16/32-bit general purpose timer module 0 in Run mode (enable timer0)
	  SYSCTL->RCGCTIMER |= (1<<0); //0b0000 0001

	  //Step1:Ensure the time is disabld (the TnEN bit in the GPTMCTL register is cleared) before making any changes [pg.698]
	  //disable timer0 by giving a value of 0 to bit 0
	  TIMER0->CTL &= ~(1<<0);

	  //Step2:Write the GPTM Configuration Register (GPTMCFG) with a value of 0x0000.0000 [pg.688]
	  //For a 16/32 bit timer, this value selects the 32 bit timer configuration
	  //For a 32/64 bit wide timer, this value selects the 64 bit timer configuration
	  TIMER0->CFG = 0x00000000;

	  //Step3:Configure the TnMR field in the GPTM Timer n Mode Register (GPTMTnMR)
	  //This value set to the GPTMTAMR register allows us to choose periodic mode
	  TIMER0->TAMR |= (0x2<<0);

	  //Step4: 0 - timer counts down: 1 - timer counts up, starting from 0x0
	  //Setting TACDIR(bit 4) to 0 in order to make the timer count down
	  TIMER0->TAMR &= ~(1<<4);

	  //Step5: Load the start value into the GPTM Timer n Interval Load Register (GPTMTnILR)
	  //Loads the start value into to the GPTMTnILR
	  TIMER0->TAILR =0X61A80; //10ms0x30D40; //5ms 0x75300;//12ms // //40,000,000 = 1 sec for 40MHz system clock

	  //Step6: If interrupts are required, set the appropriate bits in the GPTM Interrupt Mask Register (GPTMIMR)
	  TIMER0->IMR |= (1<<0); //Enabled Interrupt
	  //NVIC->ISER[0] |= (1<<19); //Interrupts from registers directly
	  //NVIC_EnableIRQ(TIMER0A_IRQn); //from CMSIS Library

	  //Step7: Set the TnEN bit in the GPTMCTL register to enable the timer and start counting
	  //Enable timer0 by giving a vaule of 1 to bit 0, this will get the timer to start counting
	  TIMER0->CTL |= (1<<0);

	  //Step8: Clear status flag (GPTMICR) by writing a 1 to the appropriate bit
	  TIMER0->ICR |= (1<<0);
	  //Wrinting a 1 to bit 0 of this register clears the TATORIS bit in the GPTMRIS register and the TATOMIS bit in the GPTMMIS register
	  //By giving TATORIS bit a value of 1, we say that Timer A has timed out. This interrupt is asserted when a one-shot or
	  //periodic mode timer reaches it's count limit

}

void TimerEdgeCounter_init(){   //PB0
	// activate port B
	 SYSCTL->RCGCGPIO |= (1<<1); // duplicado
	// 0 Corresponding pin is an input.
	 GPIOB->DIR &= ~(1<<0);
	// 1 The associated pin functions as a peripheral signal and is controlled by the alternate hardware function.
	 GPIOB->AFSEL |=(1<<0);
	// configure PB0 as T2CCP0
	 GPIOB->PCTL = (GPIOB->PCTL & 0xFFFFFFF0)+0x00000007;
	// 1 The digital functions for the corresponding pin are enabled.
	 GPIOB->DEN |=(1<<0);
	 //Enable and provide a clock to 16/32-bit general purpose timer module 2 in Run mode (enable timer2)
	 SYSCTL->RCGCTIMER |= (1<<2); //0b0000 0100
	// 1. Ensure the timer is disabled (the TnEN bit is cleared) before making any changes.
	 TIMER2->CTL &= ~(1<<0);
	// 2. Write the GPTM Configuration (GPTMCFG) register with a value of 0x0000.0004.
	 TIMER2->CFG = 0x00000004;
	// 3. In the GPTM Timer Mode (GPTMTnMR) register, write the TnCMR field to 0x0 and the TnMR field to 0x3.
	 TIMER2->TAMR |=(1<<4);//The timer counts up. When counting up, the timer starts from a value of 0x0.
	 TIMER2->TAMR &=~(1<<2);
	 TIMER2->TAMR |=(3<<0);
	// 4. Configure the type of event(s) that the timer captures by writing the TnEVENT field of the GPTM Control (GPTMCTL) register.
	 TIMER2->CTL &=~(3<<2);  //0x0 Positive edge
	// 5. If a prescaler is to be used, write the prescale value to the GPTM Timer n Prescale Register (GPTMTnPR).
	// TIMER2->TAPR =0xFF; // prescale 24bit
	// 6. Load the timer start value into the GPTM Timer n Interval Load (GPTMTnILR) register.
	 TIMER2->TAILR=0xFFFFFFFF; // upper bound, leave it in default state 0xFFFF.FFFF
	// 7. Load the prescaler match value (if any) in the GPTM Timer n Prescale Match (GPTMTnPMR) register.
	// TIMER2->TAPMR=0xFF;
	// 8. Load the event count into the GPTM Timer n Match (GPTMTnMATCHR) register. Note that
	//when executing an up-count, the value of GPTMTnPR and GPTMTnILR must be greater than the value of GPTMTnPMR and GPTMTnMATCHR.
	 TIMER2->TAMATCHR = 0xFFFFFFFF;
	 TIMER2->TAV =0;
	// 9. If interrupts are required, set the CnMIM bit in the GPTM Interrupt Mask (GPTMIMR) register.
	// TIMER2->IMR |=(1<<1);
	 //TIMER2->IMR |=(1<<2);
	// 10. Set the TnEN bit in the GPTMCTL register to enable the timer and begin waiting for edge events.
	 //TIMER2->ICR |=(1<<1);
     TIMER2->CTL |=(1<<0);
	// NVIC_SetPriority(TIMER2A_IRQn, 0);
	// NVIC_EnableIRQ(TIMER2A_IRQn); //from CMSIS Library

}

void GPIO_init(){
// Initiate  LEDs an Buttons
	  SYSCTL->RCGCGPIO |= 0x20;
	  GPIOF->LOCK = 0x4C4F434B;  // unlock the lock register
	  //GPIOF->CR = 0xFF;            // enable commit for PORT F
	  GPIOF->AMSEL = 0x00;        // disable analog functionality
	  GPIOF->PCTL = 0x00000000; // configure port f as GPIO
	  GPIOF->AFSEL = 0x00;
	  GPIOF->DIR =  0x0E; //0b0000 0100 (determines if pin is output or input)
	  GPIOF->PUR = 0x11;
	  GPIOF->DEN = 0xFF;
	  //GPIOF->DATA &= ~(0x7<<1);






// NRF CE pin configuration
	  SYSCTL->RCGCGPIO |= (1<<2);
	  GPIOC->AFSEL &= ~(1<<5);
	  GPIOC->DIR |= (1<<5);
	  //GPIOF->DR8R|= (1<<5);
	  GPIOC->DEN |= (1<<5);
// Oled RST and DC pin configuration
	  //SYSCTL->RCGCGPIO |= (1<<2);
	  GPIOC->AFSEL &= ~(1<<6);   // RST
	  GPIOC->AFSEL &= ~(1<<7);   // DC
	  GPIOC->DIR |= (1<<6)|(1<<7);
	  //GPIOF->DR8R|= (1<<5);
	  GPIOC->DEN |= (1<<6)|(1<<7);

}


