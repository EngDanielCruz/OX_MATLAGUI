/*
 * MAX30100.c
 *
 *  Created on: 19/10/2015
 *      Author: Dany
 */

#include <LM4F120E5QR.h>
#include <stdint.h>
#include "MAX30100.h"
#include "I2C.h"
#include "Filters.h"
#include "UART.h"

//*****************************************************************************
//                          Global Variables
//*****************************************************************************
//const NofSamples=4000;
uint16_t IR_FIFO_DATA[NofSamples];
uint16_t RED_FIFO_DATA[NofSamples];

uint16_t IRsample_cnt=0;
uint16_t REDsample_cnt=0;


//*****************************************************************************


void Max30100_Init(){

    uint16_t i = 0;

    for (i=0; i<NofSamples; i++){
        IR_FIFO_DATA[i]=0;
    }
    for (i=0; i<NofSamples; i++){
        RED_FIFO_DATA[i]=0;
    }


    I2C_writeByte(INTERRUPT_ENABLE, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
    I2C_writeByte(0x81, I2C_WRITE, ( I2C_MCS_RUN | I2C_MCS_STOP));                        // enable A_FULL interrrupt

    I2C_writeByte(FIFO_WRITE_PTR, I2C_WRITE, ( I2C_MCS_START | I2C_MCS_RUN));
    I2C_writeByte(0x00, I2C_WRITE, ( I2C_MCS_RUN | I2C_MCS_STOP));                         // clear FIFO_WRITE_PTR

    I2C_writeByte(FIFO_OVF_COUNTER, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
    I2C_writeByte(0x00, I2C_WRITE, ( I2C_MCS_RUN | I2C_MCS_STOP));                          // clear FIFO_OVF_COUNTER

    I2C_writeByte(FIFO_READ_PTR, I2C_WRITE, ( I2C_MCS_START | I2C_MCS_RUN));
    I2C_writeByte(0x00, I2C_WRITE, ( I2C_MCS_RUN | I2C_MCS_STOP));                          // clear FIFO_READ_PTR and stop
    // end burst----------------------------------------------------

    //I2C_writeByte(MODE_CONFIG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
   // I2C_writeByte(0x3, I2C_WRITE, (I2C_MCS_RUN));                         // enable mode 011 -> SPO2 enable

    //I2C_writeByte(SPO2_CONFIG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
   // I2C_writeByte(0x43, I2C_WRITE, (I2C_MCS_RUN | I2C_MCS_STOP));                         // 16 bit resolution,  PW and 100 sps

    I2C_writeByte(SPO2_CONFIG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
    I2C_writeByte(0xE, I2C_WRITE, (I2C_MCS_RUN | I2C_MCS_STOP));                         // 15 bit resolution, 800us PW

   // I2C_writeByte(SPO2_CONFIG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
   // I2C_writeByte(0x04, I2C_WRITE, (I2C_MCS_RUN | I2C_MCS_STOP));                         // 13 bit resolution, 200us PW and 1000 sps

    I2C_writeByte(LED_CONFIG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
    I2C_writeByte(0xCC, I2C_WRITE, (I2C_MCS_RUN | I2C_MCS_STOP));                         // 20.8 mA
    // end burst------------------------------------------------------

}

void Read_MAX_DATAFIFO(){

   // uint8_t num_available_samples=0;

    uint8_t highByte = 0;
    uint8_t lowByte = 0;
    //uint16_t i;

   // I2C_writeByte(FIFO_WRITE_PTR, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
   // uint32_t FifoWritePTR = I2C_ReadByte(((I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_STOP & ~I2C_MCS_ACK)));

   // I2C_writeByte(FIFO_READ_PTR, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
   // uint32_t FifoReadPTR = I2C_ReadByte(((I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_STOP & ~I2C_MCS_ACK)));

   // num_available_samples = FifoWritePTR - FifoReadPTR;

    I2C_writeByte(FIFO_DATA_REG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
    highByte = I2C_ReadByte( I2C_MCS_START|I2C_MCS_RUN | I2C_MCS_ACK);
    lowByte  = I2C_ReadByte( I2C_MCS_RUN | I2C_MCS_ACK);
    IR_FIFO_DATA[IRsample_cnt] =  (highByte << 8) | lowByte;
    IRsample_cnt++;

    highByte = I2C_ReadByte( I2C_MCS_RUN | I2C_MCS_ACK);
    lowByte = I2C_ReadByte( I2C_MCS_RUN & (~I2C_MCS_ACK) | I2C_MCS_STOP);
    RED_FIFO_DATA[REDsample_cnt] = (highByte << 8) | lowByte;
    REDsample_cnt++;

/*
    I2C_writeByte(FIFO_DATA_REG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
    for (i=0; i< 15; i++){

       highByte = I2C_ReadByte( I2C_MCS_START|I2C_MCS_RUN | I2C_MCS_ACK);
       lowByte  = I2C_ReadByte( I2C_MCS_RUN | I2C_MCS_ACK);
       IR_FIFO_DATA[IRsample_cnt] =  ((highByte << 8) | lowByte)>>1;
       IRsample_cnt++;

       highByte = I2C_ReadByte( I2C_MCS_RUN | I2C_MCS_ACK);
       lowByte = I2C_ReadByte( I2C_MCS_RUN | I2C_MCS_ACK);
       RED_FIFO_DATA[REDsample_cnt] = ((highByte << 8) | lowByte)>>1;
       REDsample_cnt++;
    }
*/
    if (IRsample_cnt == NofSamples-1){
        StopSampling();
        IRsample_cnt=0;
        REDsample_cnt=0;
     // send end of sampling process notification
        printChar('A');
        printChar('\r');
        printChar('\n');
    }

    /* I2C_writeByte(FIFO_DATA_REG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
    MAX_FIFO_DATA[6]   = I2C_ReadByte( I2C_MCS_START|I2C_MCS_RUN | I2C_MCS_ACK);
    for (i=0; i< 2; i++){
       j=i*4;
    MAX_FIFO_DATA[j+1] = I2C_ReadByte( I2C_MCS_RUN | I2C_MCS_ACK);
    MAX_FIFO_DATA[j+2] = I2C_ReadByte( I2C_MCS_RUN | I2C_MCS_ACK);
    MAX_FIFO_DATA[j+3] = I2C_ReadByte( I2C_MCS_RUN | I2C_MCS_ACK);
    MAX_FIFO_DATA[j+4] = I2C_ReadByte( I2C_MCS_RUN | I2C_MCS_STOP);
    }*/

}

void StartSampling(){


   /* I2C_writeByte(FIFO_WRITE_PTR, I2C_WRITE, ( I2C_MCS_START | I2C_MCS_RUN));
    I2C_writeByte(0x00, I2C_WRITE, ( I2C_MCS_RUN | I2C_MCS_STOP ));                         // clear FIFO_WRITE_PTR

    I2C_writeByte(FIFO_OVF_COUNTER, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
    I2C_writeByte(0x00, I2C_WRITE, ( I2C_MCS_RUN | I2C_MCS_STOP ));                          // clear FIFO_OVF_COUNTER

    I2C_writeByte(FIFO_READ_PTR, I2C_WRITE, ( I2C_MCS_START | I2C_MCS_RUN));
    I2C_writeByte(0x00, I2C_WRITE, ( I2C_MCS_RUN | I2C_MCS_STOP ));                          // clear FIFO_READ_PTR and stop

    I2C_writeByte(SPO2_CONFIG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
    I2C_writeByte(0x47, I2C_WRITE, (I2C_MCS_RUN | I2C_MCS_STOP ));                         // 16 bit resolution, 1.6ms PW and 100 sps

    I2C_writeByte(LED_CONFIG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
    I2C_writeByte(0xCC, I2C_WRITE, (I2C_MCS_RUN | I2C_MCS_STOP ));                         // 20.8 mA

    I2C_writeByte(INTERRUPT_ENABLE, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
    I2C_writeByte(0x91, I2C_WRITE, ( I2C_MCS_RUN | I2C_MCS_STOP ));                        // enable A_FULL interrrupt
*/
    I2C_writeByte(MODE_CONFIG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
    I2C_writeByte(0x3, I2C_WRITE, (I2C_MCS_RUN | I2C_MCS_STOP ));                         // enable mode 011 -> SPO2 enable



}

void StopSampling(){
    I2C_writeByte(MODE_CONFIG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
    I2C_writeByte(0x00, I2C_WRITE, (I2C_MCS_RUN | I2C_MCS_STOP ));                         // enable mode 011 -> SPO2 enable.
}










