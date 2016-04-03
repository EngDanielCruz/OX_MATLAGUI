/*
 * FIFO.h
 *
 *  Created on: 27/01/2016
 *      Author: Dany
 */

#ifndef FIFO_H_
#define FIFO_H_

#define FIFOSIZE 3 // can be any size
#define FIFOSUCCESS 1
#define FIFOFAIL    0

typedef float DataType;





// Two-pointer implementation of the receive FIFO
// can hold 0 to RXFIFOSIZE-1 elements

// initialize pointer FIFO
void Fifo_Init(void);
// add element to end of pointer FIFO
// return FIFOSUCCESS if successful
int Fifo_Put(DataType data);
// remove element from front of pointer FIFO
// return FIFOSUCCESS if successful
int Fifo_Get(DataType *datapt);   // return by reference.
// number of elements in pointer FIFO
// 0 to FIFOSIZE-1
unsigned short Fifo_Size(void);
// add element to end of pointer FIFO
// this function rewrite fifo elements on turn around. Act more like a circular buffer
void Fifo_Write(DataType data);

#endif /* FIFO_H_ */
