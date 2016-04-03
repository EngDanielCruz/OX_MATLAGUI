/******************************* SOURCE LICENSE *********************************
Copyright (c) 2015 MicroModeler.

A non-exclusive, nontransferable, perpetual, royalty-free license is granted to the Licensee to
use the following Information for academic, non-profit, or government-sponsored research purposes.
Use of the following Information under this License is restricted to NON-COMMERCIAL PURPOSES ONLY.
Commercial use of the following Information requires a separately executed written license agreement.

This Information is distributed WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

******************************* END OF LICENSE *********************************/

// A commercial license for MicroModeler DSP can be obtained at http://www.micromodeler.com/launch.jsp

// Begin header file, DCnotch_filter.h

#ifndef DCNOTCH_FILTER_H_ // Include guards
#define DCNOTCH_FILTER_H_

static const int DCnotch_filter_numStages = 1;
static const int DCnotch_filter_coefficientLength = 5;
extern float DCnotch_filter_coefficients[5];

typedef struct
{
    float state[4];
    float output;
} DCnotch_filterType;

typedef struct
{
    float *pInput;
    float *pOutput;
    float *pState;
    float *pCoefficients;
    short count;
} DCnotch_filter_executionState;


DCnotch_filterType *DCnotch_filter_create( void );
void DCnotch_filter_destroy( DCnotch_filterType *pObject );
 void DCnotch_filter_init( DCnotch_filterType * pThis );
 void DCnotch_filter_reset( DCnotch_filterType * pThis );
#define DCnotch_filter_writeInput( pThis, input )  \
    DCnotch_filter_filterBlock( pThis, &input, &pThis->output, 1 );

#define DCnotch_filter_readOutput( pThis )  \
    pThis->output

 int DCnotch_filter_filterBlock( DCnotch_filterType * pThis, float * pInput, float * pOutput, unsigned int count );
#define DCnotch_filter_outputToFloat( output )  \
    (output)

#define DCnotch_filter_inputFromFloat( input )  \
    (input)

 void DCnotch_filter_filterBiquad( DCnotch_filter_executionState * pExecState );
#endif // DCNOTCH_FILTER_H_

