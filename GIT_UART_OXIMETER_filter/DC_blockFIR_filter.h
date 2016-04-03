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

// Begin header file, DC_blockFIR_filter.h

#ifndef DC_BLOCKFIR_FILTER_H_ // Include guards
#define DC_BLOCKFIR_FILTER_H_

//*****************************************************************************
//                          #defines
//*****************************************************************************
// comment this lines to disable this functionalities
//#define HP_FILTER_20sps
//#define HP_FILTER_50sps  //
#define HP_FILTER_100sps // 12% more SRAM compared with to 50sps
//#define HP_FILTER_200sps   // 36% more SRAM compared with to 50sps

//*****************************************************************************

#ifdef HP_FILTER_20sps
static const int DC_blockFIR_filter_length = 81;
extern float DC_blockFIR_filter_coefficients[81];

typedef struct
{
    float * pointer;
    float state[162];
    float output;
} DC_blockFIR_filterType;
#endif

#ifdef HP_FILTER_50sps
static const int DC_blockFIR_filter_length = 197;
extern float DC_blockFIR_filter_coefficients[197];

typedef struct
{
    float * pointer;
    float state[394];
    float output;
} DC_blockFIR_filterType;
#endif

#ifdef HP_FILTER_100sps
static const int DC_blockFIR_filter_length = 393;
extern float DC_blockFIR_filter_coefficients[393];

typedef struct
{
    float * pointer;
    float state[786];
    float output;
} DC_blockFIR_filterType;
#endif

#ifdef HP_FILTER_200sps
static const int DC_blockFIR_filter_length = 787;
extern float DC_blockFIR_filter_coefficients[787];

typedef struct
{
    float * pointer;
    float state[1574];
    float output;
} DC_blockFIR_filterType;
#endif


DC_blockFIR_filterType *DC_blockFIR_filter_create( void );
void DC_blockFIR_filter_destroy( DC_blockFIR_filterType *pObject );
 void DC_blockFIR_filter_init( DC_blockFIR_filterType * pThis );
 void DC_blockFIR_filter_reset( DC_blockFIR_filterType * pThis );
#define DC_blockFIR_filter_writeInput( pThis, input )  \
    DC_blockFIR_filter_filterBlock( pThis, &input, &pThis->output, 1 );

#define DC_blockFIR_filter_readOutput( pThis )  \
    pThis->output

 int DC_blockFIR_filter_filterBlock( DC_blockFIR_filterType * pThis, float * pInput, float * pOutput, unsigned int count );
#define DC_blockFIR_filter_outputToFloat( output )  \
    (output)

#define DC_blockFIR_filter_inputFromFloat( input )  \
    (input)

 void DC_blockFIR_filter_dotProduct( float * pInput, float * pKernel, float * pAccumulator, short count );
#endif // DC_BLOCKFIR_FILTER_H_

