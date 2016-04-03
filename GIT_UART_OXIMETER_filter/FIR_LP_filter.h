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

// Begin header file, FIR_LP_filter.h

#ifndef FIR_LP_FILTER_H_ // Include guards
#define FIR_LP_FILTER_H_

//*****************************************************************************
//                          #defines
//*****************************************************************************
// comment this lines to disable this functionalities
//#define LP_FILTER_50sps  //
#define LP_FILTER_100sps // 12% more SRAM compared with to 50sps
//#define LP_FILTER_200sps   // 36% more SRAM compared with to 50sps
//#define LP_FILTER_400sps   // 36% more SRAM compared with to 50sps
//#define LP_FILTER_1000sps   // % more SRAM compared with to 50sps

//*****************************************************************************
#ifdef LP_FILTER_50sps
static const int FIR_LP_filter_length = 69;
extern float FIR_LP_filter_coefficients[69];

typedef struct
{
    float * pointer;
    float state[138];
    float output;
} FIR_LP_filterType;
#endif



#ifdef LP_FILTER_100sps
static const int FIR_LP_filter_length = 69;
extern float FIR_LP_filter_coefficients[69];

typedef struct
{
    float * pointer;
    float state[138];
    float output;
} FIR_LP_filterType;
#endif

#ifdef LP_FILTER_200sps
static const int FIR_LP_filter_length = 137;
extern float FIR_LP_filter_coefficients[137];

typedef struct
{
    float * pointer;
    float state[274];
    float output;
} FIR_LP_filterType;
#endif

#ifdef LP_FILTER_400sps
static const int FIR_LP_filter_length = 273;
extern float FIR_LP_filter_coefficients[273];

typedef struct
{
    float * pointer;
    float state[546];
    float output;
} FIR_LP_filterType;
#endif


#ifdef LP_FILTER_1000sps
static const int FIR_LP_filter_length = 681;
extern float FIR_LP_filter_coefficients[681];

typedef struct
{
    float * pointer;
    float state[1362];
    float output;
} FIR_LP_filterType;
#endif


FIR_LP_filterType *FIR_LP_filter_create( void );
void FIR_LP_filter_destroy( FIR_LP_filterType *pObject );
 void FIR_LP_filter_init( FIR_LP_filterType * pThis );
 void FIR_LP_filter_reset( FIR_LP_filterType * pThis );
#define FIR_LP_filter_writeInput( pThis, input )  \
    FIR_LP_filter_filterBlock( pThis, &input, &pThis->output, 1 );

#define FIR_LP_filter_readOutput( pThis )  \
    pThis->output

 int FIR_LP_filter_filterBlock( FIR_LP_filterType * pThis, float * pInput, float * pOutput, unsigned int count );
#define FIR_LP_filter_outputToFloat( output )  \
    (output)

#define FIR_LP_filter_inputFromFloat( input )  \
    (input)

 void FIR_LP_filter_dotProduct( float * pInput, float * pKernel, float * pAccumulator, short count );
#endif // FIR_LP_FILTER_H_

