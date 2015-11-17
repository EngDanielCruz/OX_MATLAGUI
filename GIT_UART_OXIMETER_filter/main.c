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
 *PD0->SCK	|
 *PD1->CS	|-	SPI1--Oled.c
 *---->MISO	|-
 *PD3->MOSI	|
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
#include "SPI.h"
#include "Oled.h"
#include "I2C.h"
#include "MAX30100.h"
#include "Filters.h"
#include "UART.h"

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
void Oled_int2string(uint8_t x, uint8_t y, uint16_t intnumber);
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
    //SPI_Oled_Init();
    //ConfigNRF24_Reg();
    Interrupt_IRQ();   // init IRQ pin interrupt
    Timer_Init();
    //Oled_Init();
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


    while (1){
        uint16_t i;
        uint8_t initPos=0;
        uint16_t finalPos=0;
        char SerialCommand;
// Polling Uart receive start sampling command
        if((UART0->FR & (1<<4)) == 0){
            SerialCommand =readChar();
            switch(SerialCommand){

            case 'S':
            {
                print_uint( (NofSamples-1), 4);   // print the number of samples
                printChar('\r');
                printChar('\n');
                // start sampling
                   I2C_writeByte(INTERRUPT_STATUS, I2C_WRITE, (I2C_MCS_START | I2C_MCS_RUN));
                   uint32_t MAX_StatusReg = I2C_ReadByte(((I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_STOP)));
                   StartSampling();
            break;
            }
            case 'R':  // send raw data
            {
            // send IR_FIFO_DATA to UART
                for(i=0; i<(NofSamples-1); i++){
                    print_uint( IR_FIFO_DATA[i], 5);
                    printChar('\r');
                    printChar('\n');
                }
            // send IR_FIFO_DATA to UART
                for(i=0; i<(NofSamples-1); i++){
                    print_uint( RED_FIFO_DATA[i], 5);
                    printChar('\r');
                    printChar('\n');
                }
            break;
            }
            case 'F':
            {
            // filter red data
                initPos=((FILTERTAPS-1)>>1)+1;       // start at 6 position for n of taps=11
                finalPos=(4000-((FILTERTAPS-1)>>1));
                ACC=0;
                NewValue=0;
                Accumulator_Init_values(RED_acc);
                for(i=initPos; i<finalPos; i++){
                    Filt_data[i]= EMA_Process(RED_FIFO_DATA[i]);
                }
            // send RED Filt_data to UART
                for(i=0; i<(NofSamples-1); i++){
                    printDouble( Filt_data[i]);    // send float
                    printChar('\r');
                    printChar('\n');
                }
            break;
            }
            case 'f':    //the filter routine take 29946 clock cycles=0.007487s
            {
            // filter ir data
                initPos=((FILTERTAPS-1)>>1)+1;       // start at 6 position for n of taps=11
                finalPos=(4000-((FILTERTAPS-1)>>1));
                ACC=0;
                NewValue=0;
                Accumulator_Init_values(IR_acc);
                for(i=initPos; i<finalPos; i++){
                    Filt_data[i]= EMA_Process(IR_FIFO_DATA[i]);
                }
           // send IR Filt_data to UART
                for(i=0; i<(NofSamples-1); i++){
                    printDouble( Filt_data[i]);    // send float
                    printChar('\r');
                    printChar('\n');
                }

            break;
            }

            case 'C':  // config routine
            {
                uint8_t len;
                uint8_t j=0;
                uint8_t commapos[3];
                char nofsamp[4];
                int nofsampp;
                char  alphaa[3];
                int  alphaaa;
                char tapss[3];
                int tapsss;
                len=strlen(str);

                for(i=0;i<len;i++){
                    if (str[i]==','){
                        commapos[j]=i;
                        j++;
                    }
                }
                for(i=0;i<commapos[0];i++){
                    nofsamp[i]=str[i];
                }
                for(i=(commapos[0]+1);i<commapos[1];i++){
                    alphaa[i]=str[i];
                }
                for(i=(commapos[1]+1);i<commapos[2];i++){
                    tapss[i]=str[i];
                }
                nofsampp=atoi(nofsamp);
                alphaaa=atoi(alphaa);
                tapsss=atoi(tapss);
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
            StopSampling();
              }else if(((MAX_StatusReg) &    (1<<(5)))){          // HR_RDY

                   }else if(((MAX_StatusReg) &    (1<<(6)))){     //TEMP_RDY

                        }else if(((MAX_StatusReg) &    (1<<(7)))){    // A_FULL->15 samples on data fifo
                            Read_MAX_DATAFIFO();                  //Read DATA FIFO
                             }
}











void Oled_int2string(uint8_t x, uint8_t y, uint16_t intnumber){

   char  ascii[10]={'0','1','2','3','4','5','6','7','8','9'};
    char istData[4];
    istData[0] = ascii[(intnumber/1000)];
    intnumber = intnumber % 1000;
    istData[1] = ascii[(intnumber/100)];
    intnumber = intnumber % 100;
    istData[2] = ascii[(intnumber/10)];
    intnumber = intnumber % 10;
    istData[3] = ascii[(intnumber)];
    Oled_Write_String( x,  y,istData);

}
