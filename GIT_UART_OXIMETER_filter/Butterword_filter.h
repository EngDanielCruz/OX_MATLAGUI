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

// Begin header file, Butterword_filter.h

#ifndef BUTTERWORD_FILTER_H_ // Include guards
#define BUTTERWORD_FILTER_H_

static const int Butterword_filter_numStages = 2;
static const int Butterword_filter_coefficientLength = 10;
extern float Butterword_filter_coefficients[10];

typedef struct
{
    float state[8];
    float output;
} Butterword_filterType;

typedef struct
{
    float *pInput;
    float *pOutput;
    float *pState;
    float *pCoefficients;
    short count;
} Butterword_filter_executionState;


Butterword_filterType *Butterword_filter_create( void );
void Butterword_filter_destroy( Butterword_filterType *pObject );
 void Butterword_filter_init( Butterword_filterType * pThis );
 void Butterword_filter_reset( Butterword_filterType * pThis );
#define Butterword_filter_writeInput( pThis, input )  \
    Butterword_filter_filterBlock( pThis, &input, &pThis->output, 1 );

#define Butterword_filter_readOutput( pThis )  \
    pThis->output

 int Butterword_filter_filterBlock( Butterword_filterType * pThis, float * pInput, float * pOutput, unsigned int count );
#define Butterword_filter_outputToFloat( output )  \
    (output)

#define Butterword_filter_inputFromFloat( input )  \
    (input)

 void Butterword_filter_filterBiquad( Butterword_filter_executionState * pExecState );
#endif // BUTTERWORD_FILTER_H_

