/*
 * Filters.c
 *
 *  Created on: 12/11/2015
 *      Author: Dany
 */

#include <LM4F120E5QR.h>
#include "Filters.h"
#include "MAX30100.h"
#include <math.h>

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

void linear_Regression(int32_t y[],uint16_t n,double *a,double *b,double *r){  // x is the FIFO_DATA array
                                                                     // a,b and r must be  previously defined and passed to array (&a,&b,&r)
    uint16_t i;
    float x1=0,x12=0,y1=0,x1y1=0,e;

      *a = 0;
      *b = 0;
      *r = 0;
      for(i=0;i<n;i++) {
      x1+=i;

      y1+=y[i];

      x1y1+=(i*y[i]);

      x12+=(i*i);

      }

      *b = (n*x1y1-x1*y1)/(n*x12-x1*x1);

      *a=(y1/n)-((*b)*x1/n);

      e=y[0]-(*a-(*b)*0);

      *a+=e;
}

int Linear_Regression1(int32_t y[],uint16_t n,double *a,double *b,double *r){
    int i;
    double sumx=0,sumy=0,sumx2=0,sumy2=0,sumxy=0;
    double sxx,syy,sxy;

       *a = 0;
       *b = 0;
       *r = 0;
       if (n < 2)
          return(0);

       // Compute some things we need
       for (i=0;i<n;i++) {
          sumx += i;
          sumy += y[i];
          sumx2 += (i * i);
          sumy2 += (y[i] * y[i]);
          sumxy += (i * y[i]);
       }
       sxx = sumx2 - sumx * sumx / n;
       syy = sumy2 - sumy * sumy / n;
       sxy = sumxy - sumx * sumy / n;

       /* Infinite slope (b), non existant intercept (a) */
       if (abs(sxx) == 0)
          return(0);

       /* Calculate the slope (b) and intercept (a) */
       *b = sxy / sxx;
       *a = sumy / n - (*b) * sumx / n;

       /* Compute the regression coefficient */
       if (abs(syy) == 0)
          *r = 1;
       else
          *r = sxy / sqrt(sxx * syy);

       return(1);
}





void Detrend(int32_t y[],uint16_t n,double *a,double *b){
    uint16_t i;
    for(i=0;i<n;i++) {
        y[i]=y[i]-((*a)+((*b)*i));
    }
}












