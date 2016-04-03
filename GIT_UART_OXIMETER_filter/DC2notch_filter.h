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

// Begin header file, DC2notch_filter.h

#ifndef DC2NOTCH_FILTER_H_ // Include guards
#define DC2NOTCH_FILTER_H_

static const int DC2notch_filter_numStages = 1;
static const int DC2notch_filter_coefficientLength = 5;
extern float DC2notch_filter_coefficients[5];

typedef struct
{
    float state[4];
    float output;
} DC2notch_filterType;

typedef struct
{
    float *pInput;
    float *pOutput;
    float *pState;
    float *pCoefficients;
    short count;
} DC2notch_filter_executionState;


DC2notch_filterType *DC2notch_filter_create( void );
void DC2notch_filter_destroy( DC2notch_filterType *pObject );
 void DC2notch_filter_init( DC2notch_filterType * pThis );
 void DC2notch_filter_reset( DC2notch_filterType * pThis );
#define DC2notch_filter_writeInput( pThis, input )  \
    DC2notch_filter_filterBlock( pThis, &input, &pThis->output, 1 );

#define DC2notch_filter_readOutput( pThis )  \
    pThis->output

 int DC2notch_filter_filterBlock( DC2notch_filterType * pThis, float * pInput, float * pOutput, unsigned int count );
#define DC2notch_filter_outputToFloat( output )  \
    (output)

#define DC2notch_filter_inputFromFloat( input )  \
    (input)

 void DC2notch_filter_filterBiquad( DC2notch_filter_executionState * pExecState );
#endif // DC2NOTCH_FILTER_H_

