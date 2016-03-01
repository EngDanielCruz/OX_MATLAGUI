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
#include "Oled.h"
#include "Butterword_filter.h"
#include "filtfilter.h"
#include "DCnotch_filter.h"
#include "DC2notch_filter.h"
#include "DC_blockFIR_filter.h"
#include "FIR_LP_filter.h"

//*****************************************************************************
//                          Global Variables
//*****************************************************************************
//uint16_t NofSamples=400;
float IR_FIFO_DATA[1];
float RED_FIFO_DATA[1];
float Filt_IRdata[MAXSAMPLES];//
float Filt_REDdata[MAXSAMPLES];

uint16_t IRsample_cnt=0;
uint16_t REDsample_cnt=0;
uint16_t Discardsample_cnt=0;
uint16_t samplcnt=0;
float DCnotch_Data[200];
float DC_RED_notch_Data[200];
float IRrms;
float REDrms;
float SPO2;
uint8_t SPO2store[20];
uint8_t Nofpeacks;
uint16_t Peaks_index[20];
uint16_t preavPeakindex = 0;
uint16_t j=0;
uint8_t h=0;
float HR[20];
float HRavg;
uint8_t HRstore[20];
float DCacumulator;
float DC_RED_acumulator;
float RMSacumulator;
float RMS_RED_acumulator;
float IR_DC;
float RED_DC;

// initialize the default config values
struct confcom configValues ={400,0.0,14,1};                              // default values
struct configregister configresvalue   ={3,3,16,204,3,(SAMPLES_TIME*50)}; // default values
struct samplingoptions  samplingOptions ={0,0,0,0};                       // default values

extern Butterword_filterType Butterword;
extern FIR_LP_filterType FIR_LP_filter;
extern FIR_LP_filterType FIR_RED_LP_filter;
extern DCnotch_filterType DCnotch_filter ;
extern DC2notch_filterType DC2notch_filter;
extern DC_blockFIR_filterType DC_blockFIR_filter;
extern DC_blockFIR_filterType DC_block_RED_FIR_filter;
extern DataType volatile *MidPt;
extern DataType volatile *BackPt;
extern DataType volatile *PutPt;
extern DataType volatile Fifo[];


//*****************************************************************************


void Max30100_Init(){

 /*   uint16_t i = 0;


    for (i=0; i<configValues.NofSamples; i++){
        IR_FIFO_DATA[i]=0;
    }
    for (i=0; i<configValues.NofSamples; i++){
        RED_FIFO_DATA[i]=0;
    }
*/

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
    //float fifoget_value;
    //uint16_t i;
   // int8_t FifoWritePTR;
   // uint8_t FifoReadPTR;

    if (Discardsample_cnt <250){  // This routine serve mainly to load up the filters structures
    // read the fifo just to keep the read and write pointers up to date
            I2C_writeByte(FIFO_DATA_REG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
            highByte = I2C_ReadByte( I2C_MCS_START|I2C_MCS_RUN | I2C_MCS_ACK);
            lowByte  = I2C_ReadByte( I2C_MCS_RUN | I2C_MCS_ACK);
            //IR_FIFO_DATA[IRsample_cnt] =  (highByte << 8) | lowByte;
            //IRsample_cnt++;
            IR_FIFO_DATA[0] =  ((highByte << 8) | lowByte)>>configresvalue.PWcontrol;

            highByte = I2C_ReadByte( I2C_MCS_RUN | I2C_MCS_ACK);
            lowByte = I2C_ReadByte( I2C_MCS_RUN & (~I2C_MCS_ACK) | I2C_MCS_STOP);
           //RED_FIFO_DATA[REDsample_cnt] = (highByte << 8) | lowByte;
           // REDsample_cnt++;
            RED_FIFO_DATA[0] = ((highByte << 8) | lowByte)>>configresvalue.PWcontrol;
            // Load the filter
            FIR_LP_filter_writeInput((&FIR_LP_filter), IR_FIFO_DATA[0]);
            Filt_IRdata[0] = FIR_LP_filter_readOutput( (&FIR_LP_filter) );
            DC_blockFIR_filter_writeInput( (&DC_blockFIR_filter), Filt_IRdata[0]);
            DCnotch_Data[0] = DC_blockFIR_filter_readOutput( (&DC_blockFIR_filter));

            FIR_LP_filter_writeInput((&FIR_RED_LP_filter), RED_FIFO_DATA[0]);
            Filt_REDdata[0] = FIR_LP_filter_readOutput( (&FIR_RED_LP_filter) );
            DC_blockFIR_filter_writeInput( (&DC_block_RED_FIR_filter), Filt_REDdata[0]);
            // load the fifo
            Fifo_Write(DCnotch_Data[0]);

    Discardsample_cnt++;

    }else{

        I2C_writeByte(FIFO_DATA_REG, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
        highByte = I2C_ReadByte( I2C_MCS_START|I2C_MCS_RUN | I2C_MCS_ACK);
        lowByte  = I2C_ReadByte( I2C_MCS_RUN | I2C_MCS_ACK);
        IR_FIFO_DATA[0] =  (((highByte << 8) | lowByte))>>configresvalue.PWcontrol;    //16bits


        highByte = I2C_ReadByte( I2C_MCS_RUN | I2C_MCS_ACK);
        lowByte = I2C_ReadByte( I2C_MCS_RUN & (~I2C_MCS_ACK) | I2C_MCS_STOP);
        RED_FIFO_DATA[0] = (((highByte << 8) | lowByte))>>configresvalue.PWcontrol;
//****************************************************************************************
//************************** LOW PASS FILTER SECTION**************************************
        FIR_LP_filter_writeInput( (&FIR_LP_filter), IR_FIFO_DATA[0]  );       // Write one sample into the filter
        Filt_IRdata[0] = FIR_LP_filter_readOutput( (&FIR_LP_filter) );       // Read one sample from the filter and store it in the array.

        FIR_LP_filter_writeInput((&FIR_RED_LP_filter), RED_FIFO_DATA[0]);
        Filt_REDdata[0] = FIR_LP_filter_readOutput( (&FIR_RED_LP_filter) );
        // accumulator to calculate the DC value
        DCacumulator = DCacumulator + Filt_IRdata[0];    // 52 clock cycles
        DC_RED_acumulator = DC_RED_acumulator + Filt_REDdata[0];

//*************************HIGT PASS FILTER SECTION (eliminate DC component)***************
        DC_blockFIR_filter_writeInput( (&DC_blockFIR_filter), Filt_IRdata[0]);
        DCnotch_Data[0] = DC_blockFIR_filter_readOutput( (&DC_blockFIR_filter) );

        DC_blockFIR_filter_writeInput( (&DC_block_RED_FIR_filter), Filt_REDdata[0]);
        DC_RED_notch_Data[0] = DC_blockFIR_filter_readOutput( (&DC_block_RED_FIR_filter) );
        // accumulator to calculate rms value
        RMSacumulator= (DCnotch_Data[0] * DCnotch_Data[0]) + RMSacumulator; // 28 clock cycles (just because the use of MAC (vmla.f32))
        RMS_RED_acumulator= (DC_RED_notch_Data[0] * DC_RED_notch_Data[0]) + RMS_RED_acumulator; //
        // Write data in the fifo
        Fifo_Write(DCnotch_Data[0]);


//*********************************************************************************************

//*********************Send data... I+data;R+data*****************************
#ifdef UART_ON
           printChar('s'); // start character
           printChar(';');
           //Printfloat(IR_FIFO_DATA[0], 2);    // send float
           //printChar(';');
           //Printfloat(RED_FIFO_DATA[0], 2);    // send float
           //printChar(';');
           Printfloat(DCnotch_Data[0], 3);    // send float
           printChar(';');
           Printfloat(DC_RED_notch_Data[0], 3);    // send float
           printChar(';');
           printChar('\r');
           printChar('\n');

#endif
//*********************************************************************************************
           // check peaks
          GetPeak_fromFIFO(samplcnt, Peaks_index,&Nofpeacks);

           IRsample_cnt++;
           REDsample_cnt++;
           samplcnt++;
        if (IRsample_cnt > configresvalue.SamplesWindow){ //


           /* for(i=0;i<configresvalue.SamplesWindow;i++){
                 if (DCnotch_Data[i] > 0){
                     getPeak(DCnotch_Data, i, Peaks_index,&Nofpeacks);
                 }else{

                 }
            }*/

            //i=0;

            // calculate the sqrt using PFU sqrt.f32 instruction
            IRrms= (__sqrtf(RMSacumulator/configresvalue.SamplesWindow)); // intrinsic to bypass the overhead of calling sqrtf
            REDrms= (__sqrtf(RMS_RED_acumulator/configresvalue.SamplesWindow));
            // estimate DC component
            IR_DC = (DCacumulator/configresvalue.SamplesWindow);
            RED_DC = (DC_RED_acumulator/configresvalue.SamplesWindow);
            // get heart rate
            Get_HRate(&Nofpeacks, HR);
            // get SPO2
            Get_SPO2(IRrms, REDrms, IR_DC, RED_DC, &SPO2);

            // Store the values in arrays to use in matlab
            HRstore[h]=(uint8_t)HRavg;
            SPO2store[h]=(uint8_t)SPO2;

            h++;
            Nofpeacks=0;
            DCacumulator=0;
            DC_RED_acumulator=0;
            RMSacumulator=0;
            RMS_RED_acumulator=0;
            IRsample_cnt=0;
            REDsample_cnt=0;
#ifdef OLED_ON
            // print values in the OLED display
            // HR
            Oled_int2string(24, 24 ,(uint8_t)HRavg);
            // SPO2
            Oled_int2string(40, 42 ,(uint8_t)SPO2);
#endif

#ifdef UART_ON
            // Send values to UART
            printChar('S'); // start character
            printChar(';');
            Printfloat(HRavg,2);    // send float
            printChar(';');
            Printfloat(SPO2,2);    // send float
            printChar('\r');
            printChar('\n');
#endif


            if (h>=4){
                StopSampling();

                DC_blockFIR_filter_reset((&DC_blockFIR_filter));
                DC_blockFIR_filter_reset((&DC_block_RED_FIR_filter));

                FIR_LP_filter_reset((&FIR_LP_filter));
                FIR_LP_filter_reset((&FIR_RED_LP_filter));

                Discardsample_cnt=0;
                h=0;
                samplcnt=0;
                j=0;
            }

//**********************************DISPLAY THE RESULT*******************************
            //Oled_int2string(16, 24, (uint16_t)HR[1]);


        }
    }
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


void getPeak(float arrvalue[], uint16_t indexval, uint16_t Peaks_index[],uint8_t *npeaks){
//j-> variable to keep track of the next Peak_index index element

    // We need at least 3 samples to compute the 2 slopes necessaries for peak detection
    // check if indexval >=2
    if (indexval >= 2){
        // check if f(indexval)>0
        if(arrvalue[indexval]>0){
            // check if the solpe1 is >0 and slop2 is <0
            if(((arrvalue[indexval-1]-arrvalue[indexval-2])>0) && ((arrvalue[indexval]-arrvalue[indexval-1])<=0)){
                //arrvalue[indexval] is a local peak
                Peaks_index[j]= indexval-1;    // store the peak index in Peaks_index array
                j++;
            }
        }
    }

    *npeaks=j;
}

void Get_HRate(uint8_t *nofpeaks,float Hr[]){

    uint8_t i=0;
    uint16_t SampMax[11];
    float HRacc=0;
// find how many´samples separate 2 consecutive max
    for (i=1; i<*nofpeaks;i++ ){
        if (Peaks_index[i]!=0){      // just to make sure that we don´t iterate beyond the number of peaks
            SampMax[i-1] = Peaks_index[i]-Peaks_index[i-1];
            Hr[i-1]=60/(((float)SampMax[i-1])/50);  // The use of floats make the process faster. Declaring Hr
                                                    // as float make the process faster to.
           HRacc=Hr[i-1] + HRacc;
        }
    }
    HRavg=HRacc/((float)(*nofpeaks-1));
}

void Get_SPO2(float irrms, float redrms, float ir_dc,float red_dc,float *spo2){

    float REDratio;
    float IRratio;
    float ratio;

    REDratio = (redrms/red_dc);
    IRratio = (irrms/ir_dc);
    ratio = (REDratio/IRratio);
    *spo2 = 110-(25*ratio);
    // now we new to calculate SPO2 from the ratio

}

void GetPeak_fromFIFO(uint16_t indexval, uint16_t Peaks_index[],uint8_t *npeaks){
    // We need at least 3 samples to compute the 2 slopes necessaries for peak detection

        if(*PutPt>0){
            // check if the solpe1 is >0 and slop2 is <0
            if(((*BackPt-*PutPt)>0) && ((*MidPt-*BackPt)<=0)){  // I need to use this eq because the fifowrite function increment the pointers. so i need to compensate
                //*startAddr-1 is a local peak
                Peaks_index[j]= indexval-1 ;    // store the peak index in Peaks_index array
                j++;
            }
        }
    *npeaks=j;

}
