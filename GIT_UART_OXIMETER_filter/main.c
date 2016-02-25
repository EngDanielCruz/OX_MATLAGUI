/* PORTF PF2 -> blue led
 * PORTD PD2 -> ADC -> Triggered by Timer3A if 80 MHz clock is use, Max time =53.687s
 * PORTC PC4 -> Edge Interrupt
 * PWM -> PORTB PB4    ----- MUDAR-----
 * 	   -> Timer1A
 * Timer0A -> Interrupt
 * Timer2A -> encoder input edge counter
 *		   -> PORTB PB0
 *SPI -> PORTA PA2...5
 *PA2->SCK	|
 *PA3->CSN	|-	SPI config (SPI.c)
 *PA4->MISO	|-
 *PA5->MOSI	|
 *PC4->IRQ  -> Edge Interrupt function (Periferal.c)
 *PC5->CE   -> GPIO_init (Periferal.c)
 *
 *SPI -> PORTD PA0,1,3
 *PD0->SCK	|-D0
 *PD1->CS	|-	SPI1--Oled.c
 *---->MISO	|-
 *PD3->MOSI	|-D1
 *PC6->RES  -> GPIO_init (Periferal.c)
 *PC7->DC   -> GPIO_init (Periferal.c)
 */



#include <LM4F120E5QR.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Periferal_Init.h"
#include "NRF24.h"
#include "Oled.h"
#include "SPI.h"
#include "I2C.h"
#include "MAX30100.h"
#include "Filters.h"
#include "UART.h"
#include "MA_filter.h"
//#include "Butterword_filter.h"
#include "DCnotch_filter.h"
#include "DC2notch_filter.h"
#include "DC_blockFIR_filter.h"

#include "FIFO.h"
#include "FIR_LP_filter.h"
//#include <math.h>

//*****************************************************************************
//                          #defines
//*****************************************************************************
#define FALSE 0x00000000  // substituir por uma struct
#define TRUE 0x00000001
#define setbit(b,n) ((b) |=   (1<<(n)))        /* Set bit number n in byte b   */
#define clearbit(b,n) ((b) &= (~(1<<(n))))       /* Clear bit number n in byte b */
#define readbit(b,n) ((b) &    (1<<(n)))        /* Read bit number n in byte b  */
#define flipbit(b,n) ((b) ^=   (1<<(n)))        /* Flip bit number n in byte b  */
#define bit_is_set(b,n)   (b & (1<<n))      /* Test if bit number n in byte b is set   */
#define bit_is_clear(b,n) (!(b & (1<<n)))   /* Test if bit number n in byte b is clear */

//*****************************************************************************
//                          Prototypes functions
//*****************************************************************************
void CheckInterrupts(void);
void Check_MAX_Interrupts(void);

//*****************************************************************************
//                          Global Variables
//*****************************************************************************
volatile int T0flags = FALSE;
volatile int IRQflags_NRF = FALSE;
volatile int IRQflags_MAX = FALSE;
volatile int ADCflag=FALSE;
float voltage=0;
volatile uint32_t ADCvalue;
unsigned long val = 0;
float rps = 0;
uint32_t duty_cycle;
// uint8_t StatusReg[2];

extern struct confcom configValues;
extern struct samplingoptions  samplingOptions;

extern MA_filterType fir;
MA_filterType fir;           // Statically declare the FIR filter

extern FIR_LP_filterType FIR_LP_filter;
FIR_LP_filterType FIR_LP_filter;

extern FIR_LP_filterType FIR_RED_LP_filter;
FIR_LP_filterType FIR_RED_LP_filter;

//extern Butterword_filterType Butterword;
//Butterword_filterType Butterword;

extern DCnotch_filterType DCnotch_filter;
DCnotch_filterType DCnotch_filter;

extern DC2notch_filterType DC2notch_filter;
DC2notch_filterType DC2notch_filter;

extern DC_blockFIR_filterType DC_blockFIR_filter;  // Object to filter IR
DC_blockFIR_filterType DC_blockFIR_filter;

extern DC_blockFIR_filterType DC_block_RED_FIR_filter;  // Object to filter IR
DC_blockFIR_filterType DC_block_RED_FIR_filter;


//*****************************************************************************
//                          interrupt handlers
//*****************************************************************************

extern void GPIO_PortC_Handler(void){
    GPIOC->ICR |=(1<<4);
    IRQflags_NRF = TRUE;
}
extern void GPIO_PortB_Handler(void){
    GPIOB->ICR |=(1<<1);
    IRQflags_MAX = TRUE;
}

extern void ADCSeq2_HANDLER(void){

    ADC0->ISC = (1<<2);                 // clear flag
    ADCflag = TRUE;
    ADCvalue = ADC0->SSFIFO2 & 0xFFF;   // read the result
}

extern void TIMER0A_HANDLER(void){
    GPIOF->DATA ^= (1<<1);
    TIMER0->ICR |= (1<<0);
    T0flags = TRUE;
}

extern void TIMER2A_HANDLER(void){
    TIMER2->ICR |=(1<<1);

}

//*****************************************************************************
//*****************************************************************************


int main(void){

    SystemInit ();
    GPIO_init();
    SPI_Init();
    SPI_Oled_Init();
    //ConfigNRF24_Reg();
    Interrupt_IRQ();   // init IRQ pin interrupt
    Timer_Init();
    Oled_Init();
    I2C_Init();
    UART_Init();



//-----------------------------------------------------
    //PWM_Init(16000, 8000);
    //ADC_initTimer3(40000000);
    //TimerEdgeCounter_init();
    //SystemCoreClockUpdate();

    //NVIC_EnableIRQ(ADC0SS2_IRQn); //from CMSIS Library

//------------------------------------------------------
     // flush_TX();
     // flush_RX();
//------------------------------------------------------
      setSlaveAddress(MAX30100_ADDR);    // this work with only one slave
      Max30100_Init();
//------------------------------------------------------
     // NVIC_EnableIRQ(TIMER0A_IRQn); //from CMSIS Library
      //NVIC_EnableIRQ(GPIOC_IRQn);
      NVIC_EnableIRQ(GPIOB_IRQn);
     // Oled_DisplayWhen_Init();
//  Send error code that inform all nodes that base node was reseted
     //Send_Error_Code(RX_RESETED,0x3);

      MA_filter_init( &fir );                           // Initialize the filter
      FIR_LP_filter_init(&FIR_LP_filter);
      FIR_LP_filter_init(&FIR_RED_LP_filter);
     // Butterword_filter_init(&Butterword);
      DCnotch_filter_init(&DCnotch_filter);
      DC2notch_filter_init(&DC2notch_filter);
      DC_blockFIR_filter_init( &DC_blockFIR_filter);
      DC_blockFIR_filter_init( &DC_block_RED_FIR_filter);

      Oled_DisplayWhen_Init();

      Fifo_Init();


//***************************************************************
//                       WHILE LOOP
//***************************************************************


    while (1){
        uint16_t i;
        //uint8_t initPos=0;
        //uint16_t finalPos=0;
        char SerialCommand;

        if (bit_is_clear(GPIOF->DATA,0x4)){   // Unlock hand-shake
            printChar('A');
            printChar('\r');
            printChar('\n');
            for(i=0; i<(configValues.NofSamples-1); i++){
                print_uint( 1111, 5);
                printChar('\r');
                printChar('\n');
            }

        }
// Polling Uart receive start sampling command
        if((UART0->FR & (1<<4)) == 0){
            SerialCommand =readChar();
            switch(SerialCommand){

            case 'S':
            {
                readStr(14);
                process_command(1);

                // initialize FIFO--- don´t forget

                // start sampling
                   I2C_writeByte(INTERRUPT_STATUS, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
                   uint32_t MAX_StatusReg = I2C_ReadByte(((I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_STOP)));
                   StartSampling();
            break;
            }
            case 'R':  // send raw data
            {
            // send IR_FIFO_DATA to UART
                for(i=0; i<(configValues.NofSamples-1); i++){
                    print_uint( IR_FIFO_DATA[i], 5);
                    printChar('\r');
                    printChar('\n');
                }
            // send IR_FIFO_DATA to UART
                for(i=0; i<(configValues.NofSamples-1); i++){
                    print_uint( RED_FIFO_DATA[i], 5);
                    printChar('\r');
                    printChar('\n');
                }
            break;
            }
            case 'F':
            {
            // filter RED data
                if (configValues.filt_type==1){ // MA filter
                    /*
                    initPos=((configValues.taps-1)>>1)+1;       // start at 6 position for n of taps=11
                    finalPos=(configValues.NofSamples-((configValues.taps-1)>>1));
                    ACC=0;
                    NewValue=0;
                    Accumulator_Init_values(RED_acc);
                    for(i=initPos; i<finalPos; i++){
                        Filt_data[i]= EMA_Process(RED_FIFO_DATA[i]);
                    }
                    for( i = 0; i < configValues.NofSamples; ++ i ){             // Loop for the length of the array
                        MA_filter_writeInput( (&fir), RED_FIFO_DATA[i] );              // Write one sample into the filter
                        Filt_data[i] = MA_filter_readOutput( (&fir) );        // Read one sample from the filter and store it in the array.

                    }*/

                }else if(configValues.filt_type==2){  // FIR filter


                }else { // IIR filter

                }
                // send RED Filt_data to UART
                    for(i=0; i<(configValues.NofSamples-1); i++){
                        printDouble( Filt_IRdata[i]);    // send float
                        printChar('\r');
                        printChar('\n');
                    }
            break;
            }
            case 'f':    //the filter routine take 29946 clock cycles=0.007487s
            {
            // filter IR data
                if (configValues.filt_type==1){
                    /*
                    initPos=((configValues.taps-1)>>1)+1;       // start at 6 position for n of taps=11
                    finalPos=(configValues.NofSamples-((configValues.taps-1)>>1));
                    ACC=0;
                    NewValue=0;
                    Accumulator_Init_values(IR_acc);
                    for(i=initPos; i<finalPos; i++){
                        Filt_data[i]= EMA_Process(IR_FIFO_DATA[i]);
                    }
                    for( i = 0; i < configValues.NofSamples; ++ i ){             // Loop for the length of the array
                        MA_filter_writeInput( (&fir), IR_FIFO_DATA[i] );              // Write one sample into the filter
                      Filt_data[i] = MA_filter_readOutput( (&fir) );        // Read one sample from the filter and store it in the array.
                    }

                }else if(configValues.filt_type==2){  // FIR filter

                    for( i = 0; i < configValues.NofSamples; ++ i ){             // Loop for the length of the array
                        FIR_filter_writeInput( (&fir11), IR_FIFO_DATA[i] );              // Write one sample into the filter
                       Filt_data[i] = FIR_filter_readOutput( (&fir11) );        // Read one sample from the filter and store it in the array.
                    }

                }else { // IIR filter

                    for( i = 0; i < configValues.NofSamples; ++ i ){             // Loop for the length of the array
                        Butterword_filter_writeInput( (&Butterword), IR_FIFO_DATA[i] );              // Write one sample into the filter
                       Filt_data[i] = Butterword_filter_readOutput( (&Butterword) );        // Read one sample from the filter and store it in the array.
                   }

                }
           // send IR Filt_data to UART
                for(i=0; i<(configValues.NofSamples-1); i++){
                    printDouble( Filt_data[i]);    // send float
                    printChar('\r');
                    printChar('\n');
                */}

            break;
            }

            case 'C':  // config sampling and filter routine
            {
                readStr(14);
                process_command(0);

            break;
            }
            case 'L':  // config registers routine
            {
                readStr(15);
                process_REGISTER_command();
                Max30100_Init();

            break;
            }
            case 'D':  // linear regression
            {/*
                double a=0;
                double b=0;
                double r=0;
                //uint16_t numItems=0;
               // uint16_t  numzeros=0;
                //numItems = configValues.NofSamples-((configValues.taps-1)>>1);
                //numzeros = configValues.NofSamples-(configValues.taps-1);     //n=numzeros

                //Shiftarray(Filt_data, (configValues.taps-1)>>1, numItems);

                Linear_Regression1(Filt_data,configValues.NofSamples-1,&a,&b,&r);  // ATENTION 11 taps
                Detrend(Filt_data,configValues.NofSamples,&a,&b);
                // send IR_FIFO_DATA to UART
                for(i=0; i<(configValues.NofSamples-1); i++){
                  Printfloat(Filt_data[i], 5);
                  printChar('\r');
                  printChar('\n');
                }
            break;
            }
            case 'Z':
            {
                uint16_t i=0;
                uint8_t threshold=4;
                uint16_t  numelements=0;   // total elements in the data array elements
                uint16_t  numofzeros=0;   // nº of windows (when the "function" cross zero)
                uint16_t Yzero[40];
                uint16_t Xpeaks[40];
 //               uint8_t deltaSaplesMax[6];
                for(i=0;i<40;i++) {
                    Yzero[i]=0;
                    Xpeaks[i]=0;
                }
                numelements = configValues.NofSamples-2;

                Find_zero_cross(Filt_data,numelements, Yzero,&numofzeros);

                // send the the indication of how many characters we will send
                print_int(numofzeros);  // in matlab array start in 1
                printChar('\r');
                printChar('\n');
                // send ZERO cross
                for(i=0; i<numofzeros; i++){
                  print_int( Yzero[i]);
                  printChar('\r');
                  printChar('\n');
                }

                // FIND PEAKS
                // loop over all windows
                for (i=0; i<numofzeros;i++ ){ // < deal with the last zero
                    uint16_t winSise;
                    uint16_t midindex;
                    winSise =(Yzero[i+1]- Yzero[i]);
                    midindex = ( uint16_t)(Yzero[i] + Yzero[i+1])>>1;  // (downval+upval)/2
                    // I am interested first in positive windows only
                    // so check it
                    if (Filt_data[midindex]>0){
                        Xpeaks[i]=Find_peak_Recursively(Filt_data, midindex, winSise, threshold);
                    }else {
                        Xpeaks[i] = Find_valleys_Recursively(Filt_data, midindex, winSise, threshold);
                     }
                }
                // send the the indication of how many characters we will send
                //print_int(numofzeros);  // in matlab array start in 1
                //printChar('\r');
                //printChar('\n');
                // send ZERO cross
                for(i=0; i<numofzeros; i++){
                   print_int( Xpeaks[i]);
                   printChar('\r');
                   printChar('\n');
                }
 //               Get_HeartRate(Filt_data, Xpeaks, numofzeros, deltaSaplesMax);

                for(i=0; i<6; i++){
                   print_int( deltaSaplesMax[i]);
                   printChar('\r');
                   printChar('\n');
                }
*/
            break;
            }


            default:
            {
            break;
            }
               }// end o switch
        }// end of if

// NRF24L01 interrupt events
         if(IRQflags_NRF == TRUE)
            {
             CheckInterrupts();
             IRQflags_NRF = FALSE;
            }
// MAX30100 interrupt events
         if(IRQflags_MAX == TRUE)
            {
             //StopSampling();
             Check_MAX_Interrupts();
             IRQflags_MAX = FALSE;
            }
// TIMER0 interrupt event
         if(T0flags == TRUE)
            {

             //Ping_TX();

             //uint32_t MAX_StatusReg=0;
             I2C_writeByte(INTERRUPT_STATUS, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
             uint32_t MAX_StatusReg = I2C_ReadByte(((I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_STOP)));
             StartSampling();
             T0flags = FALSE;
            }

//-----------------------------------------------------------------
/*
          if(ADCflag == TRUE)
            {

             duty_cycle=(16000*ADCvalue)/4096;
             PWM_Duty( duty_cycle);
             //voltage= (val*3.3)/4096;
            // Oled_int2string(24,24,duty_cycle);
             ADCflag = FALSE;
            }
*/
    }// end of main while loop

} // end of main



void CheckInterrupts(){
    uint8_t i;
    uint8_t interrup_flag = Read_Reg(STATUS);  // read the content of STATUS register

    if(readbit(interrup_flag, 5)){         // TX_DS interrupt flag foi disparada?  // O pacote é removido automáticamente do buffer
         Write_Reg(CONFIG,0x0F);            // Write 1 on bit TX_DS to clear the flag on STATUS Register
        GPIOF->DATA |= (1<<1);                              // volta para o modo RX -- PRIM_RX=1
//RX_ADDR_P0 - configura endereço de recepção do meu PIPE0
        DATA_PIPE0_ADDRESS[0] = 0;
        CsnPin_LOW;
        SPI_transfer (WRITE_REG + RX_ADDR_P0);
            for ( i = 0 ; i < 5 ; i++){
                SPI_transfer (DATA_PIPE0_ADDRESS[i]);
            }
        CsnPin_HIGH;
        //Oled_Write_String(0,0, "pack send");//
           //red
    }

    if(readbit(interrup_flag, 6))                 // RX_DR interrupt flag foi disparada?
            {
           uint16_t i;
           uint8_t Stpipe = get_pipe_from_STATUS();
           for ( i=0 ; i < 1000 ;i++);
            Read_data();                                  // Lê o payload
            Process_Data(Stpipe);
            //for ( i=0 ; i < 10000 ;i++);
           // Write_data_To_data_pipeX(1);
          //  Oled_Write_String(0,0, "Pack received");//

        GPIOF->DATA |= (1<<2);  //blue
           }

    if(readbit(interrup_flag, 4))              // MAX_RT interrupt flag foi disparada? o pacote não foi enviado
            {
              flush_TX();                             //flush TX BUFFER
              Write_Reg(CONFIG,0x0F );          // volta para o modo RX -- PRIM_RX=1
              //Oled_Write_String(0,0, "pack not send");//
              GPIOF->DATA |= (1<<3);  //Green
//RX_ADDR_P0 - configura endereço de recepção do meu PIPE0
              DATA_PIPE0_ADDRESS[0] = 0;
              CsnPin_LOW;
              SPI_transfer (WRITE_REG + RX_ADDR_P0);
                for ( i = 0 ; i < 5 ; i++){
                    SPI_transfer (DATA_PIPE0_ADDRESS[i]);
                }
             CsnPin_HIGH;
            }
    Write_Reg(STATUS,0x70);  //  clear the flag on STATUS Register-- RX_DR , TX_DS and MAX_RT
    // the IRQ interrupt flag already was clear in the interrupt handler
}


void Check_MAX_Interrupts(){

// 1º read the interrupt status register and wait for PWR_RDY flag
     I2C_writeByte(INTERRUPT_STATUS, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
// experimentar o bit field
     uint32_t MAX_StatusReg = I2C_ReadByte(((I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_STOP & ~I2C_MCS_ACK)));

// 2º wait for PWR_RDY flag...
     if((readbit(MAX_StatusReg,0)) == 1){   // Power ready-> proceed to config routine
        Max30100_Init();
        }else if(((MAX_StatusReg) &    (1<<(4)))){               // SPO2_RDY
            //StopSampling();
            Read_MAX_DATAFIFO();
            //StopSampling();
              }else if(((MAX_StatusReg) &    (1<<(5)))){          // HR_RDY

                   }else if(((MAX_StatusReg) &    (1<<(6)))){     //TEMP_RDY

                        }else if(((MAX_StatusReg) &    (1<<(7)))){    // A_FULL->15 samples on data fifo
                            Read_MAX_DATAFIFO();                  //Read DATA FIFO
                             }
}



