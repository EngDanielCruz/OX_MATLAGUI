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
//-----------------------


//*************************************************************************
//                          FUNCTIONS PROTOTYPES
//*************************************************************************
//******************************* FIR *********************************



//****************************************************************************
//**************************************************************************

float EMA_Process(uint16_t NewValue);

void Accumulator_Init_values(uint8_t);

//float Set_EMA_Alpha();

int linear_Regression_fifo(double *a,double *b,double *r);
void Detrend(float y[],uint16_t n,double *a,double *b);
void Detrend_fifo(float *data,double *a,double *b);
int Linear_Regression1(float y[],uint16_t n,double *a,double *b,double *r);
void Shiftarray(float shft[], uint8_t i, uint16_t numItems);
void Find_zero_cross(float input_data[],uint16_t n,uint16_t Yzero[],uint16_t *numofzeros);
uint16_t Find_peak_Recursively(float arr[], uint16_t mid,  uint16_t n, uint8_t thrs);
uint16_t Find_valleys_Recursively(float arr[], uint16_t mid,  uint16_t n, uint8_t thrs);
void Get_HeartRate(float arr[],uint16_t Xpks[],uint16_t Nzeros,uint8_t dSaplesMax[]);
