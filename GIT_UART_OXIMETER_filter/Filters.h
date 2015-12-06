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

void linear_Regression(float x[],uint16_t n,double *a,double *b,double *r);
void Detrend(float y[],uint16_t n,double *a,double *b);
int Linear_Regression1(float y[],uint16_t n,double *a,double *b,double *r);
void Shiftarray(float shft[], uint8_t i, uint16_t numItems);


