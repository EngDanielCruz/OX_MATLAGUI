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

// Begin header file, MA_filter.h

#ifndef MA_FILTER_H_ // Include guards
#define MA_FILTER_H_

static const int MA_filter_length = 20;

typedef struct
{
    float * pointer;
    float state[20];
    float output;
    float accumulator;
} MA_filterType;


MA_filterType *MA_filter_create( void );
void MA_filter_destroy( MA_filterType *pObject );
 void MA_filter_init( MA_filterType * pThis );
 void MA_filter_reset( MA_filterType * pThis );
#define MA_filter_writeInput( pThis, input )  \
    MA_filter_filterBlock( pThis, &input, &pThis->output, 1 );

#define MA_filter_readOutput( pThis )  \
    pThis->output

 int MA_filter_filterBlock( MA_filterType * pThis, float * pInput, float * pOutput, unsigned int count );
#define MA_filter_outputToFloat( output )  \
    (output)

#define MA_filter_inputFromFloat( input )  \
    (input)

#endif // MA_FILTER_H_

