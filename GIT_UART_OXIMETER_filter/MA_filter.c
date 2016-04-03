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

#include "MA_filter.h"

#include <stdlib.h> // For malloc/free
#include <string.h> // For memset


MA_filterType *MA_filter_create( void )
{
    MA_filterType *result = (MA_filterType *)malloc( sizeof( MA_filterType ) ); // Allocate memory for the object
    MA_filter_init( result );                                           // Initialize it
    return result;                                                              // Return the result
}

void MA_filter_destroy( MA_filterType *pObject )
{
    free( pObject );
}

 void MA_filter_init( MA_filterType * pThis )
{
    MA_filter_reset( pThis );

}

 void MA_filter_reset( MA_filterType * pThis )
{
    memset( &pThis->state, 0, sizeof( pThis->state ) ); // Reset state to 0
    pThis->pointer = pThis->state;                      // History buffer points to start of state buffer
    pThis->output = 0;                                  // Reset output
    pThis->accumulator = 0; // Reset accumulator

}

 int MA_filter_filterBlock( MA_filterType * pThis, float * pInput, float * pOutput, unsigned int count )
{
    int originalCount = count;
    float accumulator = pThis->accumulator;

    while( count-- )
    {
        accumulator += *pInput;                 // Add the new sample
        *(pOutput++) = accumulator * 0.047619047619047616;  // Write the output. (Divide by $filter.N})
        accumulator -= *(pThis->pointer);       // Subtract the oldest sample

        *(pThis->pointer++) = *(pInput++);  // Store the new sample in the circular history buffer
        if( pThis->pointer >= pThis->state + MA_filter_length )     // Handle wrap-around
            pThis->pointer -= MA_filter_length;
    }

     pThis->accumulator = accumulator;  // Store the accumulator
     return originalCount;

}


