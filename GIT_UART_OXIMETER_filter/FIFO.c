/*
 * FIFO.c
 *
 *  Created on: 27/01/2016
 *      Author: Dany
 */
#include "FIFO.h"

// Two-pointer implementation of the receive FIFO
// can hold 0 to RXFIFOSIZE-1 elements
DataType volatile *PutPt; // put next
DataType volatile *GetPt; // get next
DataType volatile *BackPt;
DataType volatile *MidPt;
DataType volatile *FrontPt;
DataType volatile Fifo[FIFOSIZE];

// initialize pointer FIFO
void Fifo_Init(void){
   ;
   FrontPt = GetPt = PutPt = &Fifo[FIFOSIZE-1];
  BackPt = &Fifo[0]; // Empty
  MidPt = &Fifo[1];
}
// add element to end of pointer FIFO
// return RXFIFOSUCCESS if successful
int Fifo_Put(DataType data){
  DataType volatile *nextPutPt;
  nextPutPt = PutPt+1;
  if(nextPutPt == &Fifo[FIFOSIZE]){
    nextPutPt = &Fifo[0];  // wrap
  }
  if(nextPutPt == GetPt){
    return(FIFOFAIL);      // Failed, fifo full
  }
  else{
    *(PutPt) = data;       // Put
    PutPt = nextPutPt;     // Success, update
    return(FIFOSUCCESS);
  }
}
// remove element from front of pointer FIFO
// return FIFOSUCCESS if successful
int Fifo_Get(DataType *datapt){   // return by reference
  if(PutPt == GetPt ){
    return(FIFOFAIL);      // Empty if PutPt=GetPt
  }
  *datapt = *(GetPt++);
  if(GetPt == &Fifo[FIFOSIZE]){
     GetPt = &Fifo[0];   // wrap
  }
  return(FIFOSUCCESS);
}
// number of elements in pointer FIFO
// 0 to FIFOSIZE-1
unsigned short Fifo_Size(void){
  if(PutPt < GetPt){
    return ((unsigned short)(PutPt-GetPt+(FIFOSIZE*sizeof(DataType)))/sizeof(DataType));
  }
  return ((unsigned short)(PutPt-GetPt)/sizeof(DataType));
}

//*************************
// add element to end of pointer FIFO
// this function rewrite fifo elements on turn around. Act more like a circular buffer
void Fifo_Write(DataType data){
  DataType volatile *nextPutPt; // this avoid the use of else in if statement
  DataType volatile *nextBackPt; // Otherwise we can not increment in the end
  DataType volatile *nextMidPt;
  nextBackPt = BackPt+1;
  nextPutPt = PutPt+1;
  nextMidPt = MidPt+1;
  if(nextPutPt == &Fifo[FIFOSIZE]){
    nextPutPt = &Fifo[0];  // wrap
  }
  if(nextBackPt == &Fifo[FIFOSIZE]){
     nextBackPt = &Fifo[0];   // wrap
    }
  if(nextMidPt == &Fifo[FIFOSIZE]){
     nextMidPt = &Fifo[0];   // wrap
    }
    *(PutPt) = data;       // Put
    PutPt = nextPutPt;     // increment put pointer
    MidPt= nextMidPt;
    BackPt = nextBackPt;       // increment get pointer
    FrontPt = FrontPt+1;
}


