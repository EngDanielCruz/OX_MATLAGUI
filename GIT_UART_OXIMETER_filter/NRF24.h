/*
 * NRF24.h
 *
 *  Created on: 05/09/2015
 *      Author: Dany
 * -------------------------------------------------
 * -------------------------------------------------
 *      *SPI -> PORTA P2...5
 *PA2->SCK	|
 *PA3->CSN	|-	SPI config (SPI.c)
 *PA4->MISO	|-
 *PA5->MOSI	|
 *PC4->IRQ  -> Edge Interrupt function (Periferal.c)
 *PC5->CE   -> GPIO_init (Periferal.c)
 */

#ifndef NRF24_H_
#define NRF24_H_
#endif /* NRF24_H_ */

#define readbit(b,n) ((b) &    (1<<(n)))        /* Read bit number n in byte b*/

//*****************************************************************************
//							#defines
//*****************************************************************************

#define FREE 0x0
#define OCCUPIED 0x1
//-----------------
// error code def.
#define NOPIPE_AVAILABLE 0xB // the is not an empty pipe send CODE =0xB
#define PIPE_ALREADY_REG 0XF // Send 0xF code for Pipe already registered
#define PIPE_NOT_REG 0xE     // Pipe is not registered
#define RX_RESETED 0xA       // Send 0xA code to inform that RX (base node) was reseted
#define DSR 0xC              // Data send request
#define DSP 0xD              // Data send permission
#define DATA_MSG 0xCD            // Data MSG from node
//-----------------

#define CsnPin_LOW GPIOA->DATA &= ~(1<<3)
#define CsnPin_HIGH GPIOA->DATA |= (1<<3)
#define CEPin_HIGH GPIOC->DATA |= (1<<5)
#define CEPin_LOW GPIOC->DATA &= ~(1<<5)
//*************************************************************************
//				NRF registers, commands and config #defines
//*************************************************************************
// SPI(nRF24L01) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define CD              0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address

// SPI(nRF24L01) commands
#define READ_REG        0x00  // Define read command to register
#define WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define NOP             0xFF  // Define No Operation, might be used to read status register

// Define nRF24L01 interrupt flag's

#define IDLE            0x00  // Idle, no interrupt pending
#define MAX_RT          0x10  // Max #of TX retrans interrupt
#define TX_DS           0x20  // TX data sent interrupt
#define RX_DR           0x40  // RX data received

/*
// Predefine TX payload packet..
byte const data_in[TX_PLOAD_WIDTH] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
                                         0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f}; */
//opções

#define FREQ_CANAL    83              // 1 a 126
#define TX_ADR_LENGTH   5            // 5 bytes TX(RX) address width
#define TX_PLOAD_WIDTH  10          // 16 bytes TX and RX payload
#define ENDTX    75                // 1 a 255  define 1 dos 5 byte para o address
#define ENDRX    75               // 1 a 255  têm de ser os dois iguais para auto ACK


//*****************************************************************************
//                          Global Variables
//*****************************************************************************
 extern uint8_t DATA_PIPE0_ADDRESS[TX_ADR_LENGTH] ;
 extern uint8_t DATA_PIPE1_ADDRESS[TX_ADR_LENGTH] ;
 extern uint8_t DATA_PIPE2_ADDRESS[TX_ADR_LENGTH] ;
 extern uint8_t DATA_PIPE3_ADDRESS[TX_ADR_LENGTH] ;
 extern uint8_t DATA_PIPE4_ADDRESS[TX_ADR_LENGTH] ;
 extern uint8_t DATA_PIPE5_ADDRESS[TX_ADR_LENGTH] ;
 extern uint8_t DATA_PIPEx_ADDRESS[TX_ADR_LENGTH] ;

 extern uint8_t RX_PAYLOAD[TX_PLOAD_WIDTH] ;
 extern uint8_t TX_PAYLOAD[TX_PLOAD_WIDTH] ;




//*************************************************************************
//                          FUNCTIONS PROTOTIPES
//*************************************************************************

uint8_t Write_Reg(uint8_t reg_addr, uint8_t value);
uint8_t Read_Reg(uint8_t reg_addr);
void flush_TX(void);
void flush_RX(void);
void Read_data(void);
void Write_data(void);
void ConfigNRF24_Reg(void);
void Process_Data(uint8_t);
void Ping_TX(void);
void Send_COM_PIPE(uint8_t,uint8_t);
uint8_t get_pipe_from_STATUS();
uint8_t Write_data_To_data_pipeX(uint8_t,uint8_t);
void Send_Error_Code(uint8_t, uint8_t,uint8_t);


