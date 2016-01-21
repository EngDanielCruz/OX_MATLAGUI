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
static const int FIR_filter_length = 24;
extern float FIR_filter_coefficients[24];

typedef struct
{
    float * pointer;
    float state[48];
    float output;
} FIR_filterType;

//*************************************************************************
//                          FUNCTIONS PROTOTYPES
//*************************************************************************
FIR_filterType *FIR_filter_create( void );
void FIR_filter_destroy( FIR_filterType *pObject );
 void FIR_filter_init( FIR_filterType * pThis );
 void FIR_filter_reset( FIR_filterType * pThis );
#define FIR_filter_writeInput( pThis, input )  \
    FIR_filter_filterBlock( pThis, &input, &pThis->output, 1 );

#define FIR_filter_readOutput( pThis )  \
    pThis->output

 int FIR_filter_filterBlock( FIR_filterType * pThis, float * pInput, float * pOutput, unsigned int count );
#define FIR_filter_outputToFloat( output )  \
    (output)

#define FIR_filter_inputFromFloat( input )  \
    (input)

 void FIR_filter_dotProduct( float * pInput, float * pKernel, float * pAccumulator, short count );

//--------------------------------------
float EMA_Process(uint16_t NewValue);

void Accumulator_Init_values(uint8_t);

//float Set_EMA_Alpha();

void linear_Regression(float x[],uint16_t n,double *a,double *b,double *r);
void Detrend(float y[],uint16_t n,double *a,double *b);
int Linear_Regression1(float y[],uint16_t n,double *a,double *b,double *r);
void Shiftarray(float shft[], uint8_t i, uint16_t numItems);
void Find_zero_cross(float input_data[],uint16_t n,uint16_t Yzero[],uint16_t *numofzeros);
uint16_t Find_peak_Recursively(float arr[], uint16_t mid,  uint16_t n, uint8_t thrs);
uint16_t Find_valleys_Recursively(float arr[], uint16_t mid,  uint16_t n, uint8_t thrs);
void Get_HeartRate(float arr[],uint16_t Xpks[],uint16_t Nzeros);
