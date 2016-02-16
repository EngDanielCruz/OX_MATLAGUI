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
#include "FIFO.h"
#include "Butterword_filter.h"
#include "filtfilter.h"
#include "FIR_filter.h"
#include "DCnotch_filter.h"
#include "DC2notch_filter.h"


//*****************************************************************************
//                          Global Variables
//*****************************************************************************
//uint16_t NofSamples=4000;
float IR_FIFO_DATA[MAXSAMPLES];
float RED_FIFO_DATA[MAXSAMPLES];

uint16_t IRsample_cnt=0;
uint16_t REDsample_cnt=0;
uint16_t Discardsample_cnt=0;
uint8_t Fifocnt=0;
float auxVar;
float butwrd;
float detrend_data;
double DC_componente[150];
float DCnotch_Data[1000];
// linear regression coefficients
uint16_t Peaks_index[12];
uint16_t Valleys_index[12];
uint16_t preavPeakindex = 0;
uint16_t j=0;
uint16_t k=1;
float DCacumulator;
float IR_DC;

// initialize the default config values
struct confcom configValues ={400,0.0,14,1};
struct configregister configresvalue   ={3,14,16,204};
struct samplingoptions  samplingOptions ={0,0,0,0};

extern Butterword_filterType Butterword;
extern FIR_filterType fir11;
extern DCnotch_filterType DCnotch_filter ;
extern DC2notch_filterType DC2notch_filter;


//*****************************************************************************


void Max30100_Init(){

    uint16_t i = 0;


    for (i=0; i<configValues.NofSamples; i++){
        IR_FIFO_DATA[i]=0;
    }
    for (i=0; i<configValues.NofSamples; i++){
        RED_FIFO_DATA[i]=0;
    }


    I2C_writeByte(INTERRUPT_ENABLE, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));            //0x81
    I2C_writeByte(configresvalue.intconfig, I2C_WRITE, ( I2C_MCS_RUN | I2C_MCS_STOP));                        // enable A_FULL interrrupt

    I2C_writeByte(FIFO_WRITE_PTR, I2C_WRITE, ( I2C_MCS_START | I2C_MCS_RUN));
    I2C_writeByte(0x00, I2C_WRITE, ( I2C_MCS_RUN | I2C_MCS_STOP));                         // clear FIFO_WRITE_PTR

    I2C_writeByte(FIFO_OVF_COUNTER, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
    I2C_writeByte(0x00, I2C_WRITE, ( I2C_MCS_RUN | I2C_MCS_STOP));                          // clear FIFO_OVF_COUNTER

    I2C_writeByte(FIFO_READ_PTR, I2C_WRITE, ( I2C_MCS_START | I2C_MCS_RUN));
    I2C_writeByte(0x00, I2C_WRITE, ( I2C_MCS_RUN | I2C_MCS_STOP));                          // clear FIFO_READ_PTR and stop
    // end burst----------------------------------------------------

    //I2C_writeByte(MODE_CONFIG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
   // I2C_writeByte(configresvalue.modeconfig, I2C_WRITE, (I2C_MCS_RUN));                         // enable mode 011 -> SPO2 enable

    //I2C_writeByte(SPO2_CONFIG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
   // I2C_writeByte(0x43, I2C_WRITE, (I2C_MCS_RUN | I2C_MCS_STOP));                         // 16 bit resolution,  PW and 100 sps

    I2C_writeByte(SPO2_CONFIG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
    I2C_writeByte(configresvalue.spo2config, I2C_WRITE, (I2C_MCS_RUN | I2C_MCS_STOP));                         // 15 bit resolution, 800us PW

   // I2C_writeByte(SPO2_CONFIG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
   // I2C_writeByte(0x04, I2C_WRITE, (I2C_MCS_RUN | I2C_MCS_STOP));                         // 13 bit resolution, 200us PW and 1000 sps

    I2C_writeByte(LED_CONFIG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
    I2C_writeByte( configresvalue.ledconfig, I2C_WRITE, (I2C_MCS_RUN | I2C_MCS_STOP));                         // 40.2 mA
    // end burst------------------------------------------------------

}

void Read_MAX_DATAFIFO(){

   // uint8_t num_available_samples=0;

    uint8_t highByte = 0;
    uint8_t lowByte = 0;
    float fifoget_value;
    uint16_t i;
   // int8_t FifoWritePTR;
   // uint8_t FifoReadPTR;

//    I2C_writeByte(FIFO_WRITE_PTR, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
//    FifoWritePTR = I2C_ReadByte(((I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_STOP & ~I2C_MCS_ACK)));

//    I2C_writeByte(FIFO_READ_PTR, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
//    FifoReadPTR = I2C_ReadByte(((I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_STOP & ~I2C_MCS_ACK)));


//    num_available_samples = abs(16 + (FifoWritePTR+1) - FifoReadPTR) % 16;
/*
if(num_available_samples >= 1){

    for (i=0; i< num_available_samples; i++){
       I2C_writeByte(FIFO_DATA_REG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
       highByte = I2C_ReadByte( I2C_MCS_START|I2C_MCS_RUN | I2C_MCS_ACK);
       lowByte  = I2C_ReadByte( I2C_MCS_RUN | I2C_MCS_ACK);
       IR_FIFO_DATA[IRsample_cnt] =  ((highByte << 8) | lowByte)>>1;
       IRsample_cnt++;

       highByte = I2C_ReadByte( I2C_MCS_RUN | I2C_MCS_ACK);
       lowByte = I2C_ReadByte( I2C_MCS_RUN | I2C_MCS_ACK);
       RED_FIFO_DATA[REDsample_cnt] = ((highByte << 8) | lowByte)>>1;
       REDsample_cnt++;
    }
}
*/  if (Discardsample_cnt <100){
    // read the fifo just to keep the read and write pointers up to date
            I2C_writeByte(FIFO_DATA_REG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
            highByte = I2C_ReadByte( I2C_MCS_START|I2C_MCS_RUN | I2C_MCS_ACK);
            lowByte  = I2C_ReadByte( I2C_MCS_RUN | I2C_MCS_ACK);
            //IR_FIFO_DATA[IRsample_cnt] =  (highByte << 8) | lowByte;
            //IRsample_cnt++;
            IR_FIFO_DATA[0] =  (highByte << 8) | lowByte;



            highByte = I2C_ReadByte( I2C_MCS_RUN | I2C_MCS_ACK);
            lowByte = I2C_ReadByte( I2C_MCS_RUN & (~I2C_MCS_ACK) | I2C_MCS_STOP);
           //RED_FIFO_DATA[REDsample_cnt] = (highByte << 8) | lowByte;
           // REDsample_cnt++;
            RED_FIFO_DATA[0] = (highByte << 8) | lowByte;
            // Load the filter
            //FIR_filter_writeInput( (&fir11), IR_FIFO_DATA[0] );
           // Butterword_filter_writeInput( (&Butterword), IR_FIFO_DATA[0]  );
            //Filt_data[0] = Butterword_filter_readOutput( (&Butterword) );
            //DCnotch_filter_writeInput((&DCnotch_filter), Filt_data[0]);
    Discardsample_cnt++;

    }else{

        I2C_writeByte(FIFO_DATA_REG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
        highByte = I2C_ReadByte( I2C_MCS_START|I2C_MCS_RUN | I2C_MCS_ACK);
        lowByte  = I2C_ReadByte( I2C_MCS_RUN | I2C_MCS_ACK);
        IR_FIFO_DATA[IRsample_cnt] =  ((highByte << 8) | lowByte);    //16bits


        highByte = I2C_ReadByte( I2C_MCS_RUN | I2C_MCS_ACK);
        lowByte = I2C_ReadByte( I2C_MCS_RUN & (~I2C_MCS_ACK) | I2C_MCS_STOP);
        RED_FIFO_DATA[REDsample_cnt] = ((highByte << 8) | lowByte);


        //FIR_filter_writeInput( (&fir11), IR_FIFO_DATA[IRsample_cnt] );              // Write one sample into the filter
        //Filt_data[IRsample_cnt] = FIR_filter_readOutput( (&fir11) );        // Read one sample from the filter and store it in the array.
        Butterword_filter_writeInput( (&Butterword), IR_FIFO_DATA[IRsample_cnt]  );
        Filt_data[IRsample_cnt] = Butterword_filter_readOutput( (&Butterword) );

        //DCacumulator = Filt_data[IRsample_cnt]+DCacumulator;    //

        //NOTCH FILTER
        FIR_filter_writeInput( (&fir11), Filt_data[IRsample_cnt]);
        DCnotch_Data[IRsample_cnt] = FIR_filter_readOutput( (&fir11) );

        //Check if DCnotch_Data >0 -- looking for peaks


        IRsample_cnt++;
        REDsample_cnt++;
// Send raw data... I+data;R+data
/*

           printDouble( IR_FIFO_DATA[0]);    // send float
           printChar('\r');
           printChar('\n');

           printDouble( RED_FIFO_DATA[0]);    // send float
           printChar('\r');
           printChar('\n');
*/



        if (IRsample_cnt > 800){
            StopSampling();

            for(i=0;i<200;i++){
                 if (DCnotch_Data[i] > 0){
                     getPeak(DCnotch_Data, i, Peaks_index);
                 }else{

                 }
            }
            i=0;
            k=1;
            // estimate DC component
           // IR_DC = (DCacumulator /200);  // divide by N =125;
            DCnotch_filter_reset((&DCnotch_filter));
            Butterword_filter_reset((&Butterword));
            DCacumulator=0;
            IRsample_cnt=0;
            REDsample_cnt=0;
            Discardsample_cnt=0;
            Fifocnt=0;

/*
                            // Detrend the raw data
                            filtfilter(IR_FIFO_DATA,Filt_data);
                            // get DC component
                            for(i=0; i<(400); i++){
                                DC_componente[i] = IR_FIFO_DATA[i]-Filt_data[i];
                            }
                            // load butterword low pass filter wiht dame samples
                            for(i=0; i<(configValues.NofSamples-300); i++){
                               butwrd= (float)(Filt_data[i]);
                               Butterword_filter_writeInput( (&Butterword), butwrd );
                            }
                            for(i=0; i<(configValues.NofSamples); i++){
                            butwrd= (float)(Filt_data[i]);
                            Butterword_filter_writeInput( (&Butterword), butwrd );
                            // pass filtered value to the FIFO
                            butterwordfiltdata[i] = Butterword_filter_readOutput( (&Butterword) );
                            }
*/
                if (samplingOptions.IRRawPlot == 1){

                }
                if(samplingOptions.REDRawPlot == 1){

                }
                if(samplingOptions.IRPlot == 1){

                }
                if(samplingOptions.RedPlot == 1){

                }


           // }
         // send end of sampling process notification
            printChar('A');
            printChar('\r');
            printChar('\n');
        }
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
    I2C_writeByte(configresvalue.modeconfig, I2C_WRITE, (I2C_MCS_RUN | I2C_MCS_STOP ));                         // enable mode 011 -> SPO2 enable



}

void StopSampling(){
    I2C_writeByte(MODE_CONFIG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
    I2C_writeByte(0x00, I2C_WRITE, (I2C_MCS_RUN | I2C_MCS_STOP ));                         // enable mode 011 -> SPO2 enable.
}


uint16_t getPeak(float arrvalue[], uint16_t indexval, uint16_t Peaks_index[]){
    if(indexval<2){
        return 0;
    }

    if(arrvalue[indexval] >= arrvalue[Peaks_index[j]]){
         return Peaks_index[j]=indexval;
    }else{
               if (k >= 3 ){
                    Peaks_index[j]=indexval;
                    j++;
                    k=1;
                    return Peaks_index[j-1];
                }
         Peaks_index[j]=indexval-k;
         k++;
         return Peaks_index[j];
    }
}







