/*
 * Filters.h
 *
 *  Created on: 12/11/2015
 *      Author: Dany
 */

#ifndef FILTERS_H_
#define FILTERS_H_
#endif /* FILTERS_H_ */

//*****************************************************************************
//                          #defines
//*****************************************************************************
//#define FILTERTAPS 11


//*****************************************************************************
//                            Variables
//*****************************************************************************
extern float Filt_data[];
extern float  ACC;
extern float NewValue;
//*************************************************************************
//                          FUNCTIONS PROTOTYPES
//*************************************************************************
float EMA_Process(uint16_t NewValue);

void Accumulator_Init_values(uint8_t);

//float Set_EMA_Alpha();

void linear_Regression(uint16_t x[],uint16_t n,float *a,float *b,float *r);
void Detrend(uint16_t y[],uint16_t n,float *a,float *b);
