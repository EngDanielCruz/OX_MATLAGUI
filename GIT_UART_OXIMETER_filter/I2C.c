/*
 * I2C.c
 *
 *  Created on: 11/10/2015
 *      Author: Dany
 */
#include <LM4F120E5QR.h>
#include "I2C.h"





void I2C_Init(){

    // from datasheet:
    // 1. Enable the I2C clock using the RCGCI2C register in the System Control module (see page 322)
    SYSCTL->RCGCI2C = (1<<0);  // I2C Module 0
   // 2. Enable the clock to the appropriate GPIO module via the RCGCGPIO register in the System Control module
   SYSCTL->RCGCGPIO |= (1<<1);  // enable PORTB
   // 3.In the GPIO module, enable the appropriate pins for their alternate function using the GPIOAFSEL register
   GPIOB->AFSEL |= (1<<2)|(1<<3); //Enabling the alternate functions for pin 2 and 3
   GPIOB->DEN |= (1<<2)|(1<<3); //enable digital IO on PB2 and PB3
   // 4. Enable the I2CSDA pin for open-drain operation
   GPIOB->ODR |=(1<<3);
   // 5. Configure the PMCn fields in the GPIOPCTL register to assign the I2C signals to the appropriate pins.
   GPIOB->PCTL |= (3<<8)|(3<<12);
   // 6. Initialize the I2C Master by writing the I2CMCR register with a value of 0x0000.0010
   I2C0->MCR = (1<<4);
   // 7. Set the desired SCL clock speed of 400 Kbps by writing the I2CMTPR register with the correct
        //value. The value written to the I2CMTPR register represents the number of system clock periods
       //in one SCL clock period. The TPR value is determined by the following equation:
       //TPR = (System Clock/(2*(SCL_LP + SCL_HP)*SCL_CLK))-1;
   I2C0->MTPR |=(0x9<<0);             // for 80MHz and 400Kb I2C clock speed
}


uint32_t I2C_writeByte(uint8_t dataByte, uint8_t R_W, uint8_t Conditions){

         //Clear bit, transmitting data .... set bit, read data
          if(R_W == 0)
          {
            I2C0->MSA &= ~(1<<0); //Clear bit, transmitting data
          }
          else
          {
            I2C0->MSA |= (1<<0); //set bit, receiving data
          }
 //------------------------------------
         //Place data (byte) to be transmitted in the data register by writing the I2CMDR register with the desired data.
         I2C0->MDR = dataByte;

         while(I2C0->MCS & I2C_MCS_BUSY){};  // wait for I2C ready

         // Initiate a single byte transmit of the data from Master to Slave by
        //writing the I2CMCS register with a value of 0x0000.0007 (START, RUN)
         I2C0->MCS =Conditions;

         while(I2C0->MCS & I2C_MCS_BUSY){};// wait for transmission done

             if((I2C0->MCS & (I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR)) != 0){
                // send stop if nonzero
                 I2C0->MCS = ( I2C_MCS_STOP);
                 // return error bits if nonzero
              return (I2C0->MCS & (I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR));
             }
         // return error bits
         return (I2C0->MCS & (I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR)); // no error=0
}


uint32_t I2C_ReadByte(int8_t Conditions){
        while(I2C0->MCS & I2C_MCS_BUSY){};  // wait for I2C ready
         I2C0->MSA |= (1<<0);
         I2C0->MCS =Conditions;
        while(I2C0->MCS & I2C_MCS_BUSY){};// wait for transmission done

    if(((I2C0->MCS &  I2C_MCS_ERROR) != 0)){//An error occurred on the last operation.
      return 0xAA;                            //The error can be from the slave address not being acknowledged
    }                                        //or the transmit data not being acknowledged
    return (I2C0->MDR);          // usually returns c on error
}

void setSlaveAddress(uint8_t slaveAddress)
{
  //MSA register consists of eight bits: 7 address bits and 1 Receive/Send bit, which determines if the next operation is Receive(1) or Transmit(0)
  //slaveAddress = 0b _001.0000
  I2C0->MSA = (slaveAddress<<1); //Specify the slave address of the master
}

void I2C_Stop(){

    I2C0->MCS =(I2C_MCS_STOP & ~(I2C_MCS_ACK)) ;

}





































