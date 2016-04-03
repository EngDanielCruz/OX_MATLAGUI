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

// Begin header file, bpIIR_filter.h

#ifndef BPIIR_FILTER_H_ // Include guards
#define BPIIR_FILTER_H_

static const int bpIIR_filter_numStages = 2;
static const int bpIIR_filter_coefficientLength = 10;
extern float bpIIR_filter_coefficients[10];

typedef struct
{
    float state[8];
    float output;
} bpIIR_filterType;

typedef struct
{
    float *pInput;
    float *pOutput;
    float *pState;
    float *pCoefficients;
    short count;
} bpIIR_filter_executionState;


bpIIR_filterType *bpIIR_filter_create( void );
void bpIIR_filter_destroy( bpIIR_filterType *pObject );
 void bpIIR_filter_init( bpIIR_filterType * pThis );
 void bpIIR_filter_reset( bpIIR_filterType * pThis );
#define bpIIR_filter_writeInput( pThis, input )  \
    bpIIR_filter_filterBlock( pThis, &input, &pThis->output, 1 );

#define bpIIR_filter_readOutput( pThis )  \
    pThis->output

 int bpIIR_filter_filterBlock( bpIIR_filterType * pThis, float * pInput, float * pOutput, unsigned int count );
#define bpIIR_filter_outputToFloat( output )  \
    (output)

#define bpIIR_filter_inputFromFloat( input )  \
    (input)

 void bpIIR_filter_filterBiquad( bpIIR_filter_executionState * pExecState );
#endif // BPIIR_FILTER_H_

