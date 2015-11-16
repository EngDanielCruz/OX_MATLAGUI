/*
 * Periferal_Init.h
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

#ifndef PERIFERAL_INIT_H_
#define PERIFERAL_INIT_H_
#endif /* PERIFERAL_INIT_H_ */

//*****************************************************************************
//							Prototypes functions
//*****************************************************************************
void ADC_initTimer3(uint32_t);
//unsigned long Read_ADC_SS2(void);
void Timer_Init(void);
void GPIO_init(void);
void PWM_Init(uint16_t period, uint16_t high);
void PWM_Duty(uint16_t high);
void Interrupt_IRQ(void);
void TimerEdgeCounter_init(void);


//*****************************************************************************
