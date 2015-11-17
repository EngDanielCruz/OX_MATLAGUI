/*
 * Filters.c
 *
 *  Created on: 12/11/2015
 *      Author: Dany
 */

#include <LM4F120E5QR.h>
#include "Filters.h"
#include "MAX30100.h"

//*****************************************************************************
//                          Global Variables
//*****************************************************************************
extern struct confcom configValues;
float Filt_data[MAXSAMPLES];
float  ACC=0;
float NewValue=0;

//*************************************************************************
//                          FUNCTIONS
//*************************************************************************
/*
float Set_EMA_Alpha(){
    float alpha = 1/FILTERTAPS;
    return alpha;
}
*/
void Accumulator_Init_values(uint8_t dataselect ){
    uint8_t i;
    uint32_t sum=0;
    ACC=0;
    if(dataselect==IR_acc){
    // find y((FILTERTAPS-1)/2) by averaging  sample [0 , FILTERTAPS-1]
      for (i=0; i<configValues.taps; i++){
        sum = sum + IR_FIFO_DATA[i];
      }
    ACC=sum/configValues.taps;
    Filt_data[((configValues.taps-1)>>1)]=ACC;
        }
    if(dataselect==RED_acc){
    // find y((FILTERTAPS-1)/2) by averaging  sample [0 , FILTERTAPS-1]
      for (i=0; i<configValues.taps; i++){
        sum = sum + RED_FIFO_DATA[i];
      }
    ACC=sum/configValues.taps;
    Filt_data[((configValues.taps-1)>>1)]=ACC;
    }
}

float EMA_Process(uint16_t Value){
     NewValue = ACC + (configValues.alpha*(Value-ACC));
     ACC=NewValue;
     return NewValue;
}



