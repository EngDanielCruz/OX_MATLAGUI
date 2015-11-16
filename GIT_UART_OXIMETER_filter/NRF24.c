/*
 * NRF24.c
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



#include <LM4F120E5QR.h>
#include "NRF24.h"
#include "SPI.h"

//*****************************************************************************
//							Global Variables
//*****************************************************************************

uint8_t StatusReg[2];
 //uint8_t ENABLE_PIPEx;
// Variavel que serve para configurar os enderessos dos pipes
uint8_t DATA_PIPE0_ADDRESS[TX_ADR_LENGTH]  = {0,0xc1,0xc1,0xc1,0xc1};
uint8_t DATA_PIPE1_ADDRESS[TX_ADR_LENGTH]  = {1,0xc2,0xc2,0xc2,0xc2};
uint8_t DATA_PIPE2_ADDRESS[TX_ADR_LENGTH]  = {2,0xc2,0xc2,0xc2,0xc2};
uint8_t DATA_PIPE3_ADDRESS[TX_ADR_LENGTH]  = {3,0xc2,0xc2,0xc2,0xc2};
uint8_t DATA_PIPE4_ADDRESS[TX_ADR_LENGTH]  = {4,0xc2,0xc2,0xc2,0xc2};
uint8_t DATA_PIPE5_ADDRESS[TX_ADR_LENGTH]  = {5,0xc2,0xc2,0xc2,0xc2};
uint8_t DATA_PIPEx_ADDRESS[TX_ADR_LENGTH]  = {6,0xc2,0xc2,0xc2,0xc2};

uint8_t RX_PAYLOAD[TX_PLOAD_WIDTH] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
       0x00,0x00};
uint8_t TX_PAYLOAD[TX_PLOAD_WIDTH] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
                                         0x08,0x09};

 uint8_t DeviceCnt;

 struct Device
 {
    uint8_t Device_Slot;   // implementar mecanismo de iniciação
    uint8_t Device_ID;
    uint8_t Device_Pipe;
    uint8_t Device_Type;
 };

 struct Device RegisteredDevices[5]; // array de struct para guardar informação dos outros node
 const struct Device node = {0,0,0,0};   // strutc com informação deste node

//*************************************************************************
//                          FUNCTIONS
//*************************************************************************
/*
Description:
Escreve o valor value no registo reg_addr
 In/Out parameters:
 In: 'reg_adrr' register to write value 'value' to.
  Return: status register content.
*/
uint8_t Write_Reg(uint8_t reg_addr, uint8_t value){

	CsnPin_LOW;
	 SPI_transfer(WRITE_REG + reg_addr);
	 StatusReg [1] = SPI_transfer(value);
	CsnPin_HIGH;
	return StatusReg [1];
}

/*
Description:
this function read 1 byte registers only
 In/Out parameters:
 In: 'reg_adrr' register to read .
  Return: status register content.
*/

uint8_t Read_Reg(uint8_t reg_addr){

	CsnPin_LOW;
	 StatusReg [0] = SPI_transfer(READ_REG + reg_addr);
	 StatusReg [1] = SPI_transfer(0x0);
	CsnPin_HIGH;
	return StatusReg [1];

}

void ConfigNRF24_Reg(){
	uint32_t i;

	//CONFIG Reg.- coloca em modo de recepção, e define CRC de 2 Bytes
	   Write_Reg(CONFIG, 0x0F );   //(byte reg_addr, byte value)

	  //EN_AA - habilita autoACK no PIPE0
	   Write_Reg(EN_AA, 0x3F );

	   //EN_RXADDR - ativa os PIPE´s
	   Write_Reg(EN_RXADDR, 0x3F );

	  //SETUP_AW - define o endereço com tamanho de 5 Bytes --TX_ADR_LENGTH =5
	   Write_Reg(SETUP_AW, 0x03 );

	  //SETUP_RETR - configura retrasmissão automática dos pacotes
	   Write_Reg(SETUP_RETR, 0x24 ); // auto retransmit count enable and delay 750us

	  //RF_CH define o canal do modulo (TX e RX devem ser iguais) -- 83
	   Write_Reg(RF_CH, FREQ_CANAL );

	  //RF_SETUP - taxa em 2Mbps, e maxima potencia 0dbm
	   Write_Reg(RF_SETUP, 0xE );

	  //STATUS - faz reset do resgisto STATUS
	   Write_Reg(STATUS, 0x70 );

              //RX_ADDR_P0 - configura endereço de recepção PIPE0
               CsnPin_LOW;
               SPI_transfer(WRITE_REG + RX_ADDR_P0);
               for ( i=0 ; i < 5 ;i++){
                   SPI_transfer (DATA_PIPE0_ADDRESS[i]);
                  }
               CsnPin_HIGH;

               //RX_ADDR_P1 - configura endereço de recepção PIPE1
                      CsnPin_LOW;
                      SPI_transfer(WRITE_REG + RX_ADDR_P1);
                      for ( i=0 ; i < 5 ;i++){
                          SPI_transfer (DATA_PIPE1_ADDRESS[i]);
                         }
                      CsnPin_HIGH;
                //RX_ADDR_P2 - configura endereço de recepção PIPE2
                          CsnPin_LOW;
                          SPI_transfer(WRITE_REG + RX_ADDR_P2);
                          SPI_transfer (0x02);
                          CsnPin_HIGH;

                  //RX_ADDR_P3 - configura endereço de recepção PIPE3
                        CsnPin_LOW;
                        SPI_transfer(WRITE_REG + RX_ADDR_P3);
                        SPI_transfer (0x03);
                        CsnPin_HIGH;

	                //RX_ADDR_P4 - configura endereço de recepção PIPE4
	                       CsnPin_LOW;
	                       SPI_transfer(WRITE_REG + RX_ADDR_P4);
	                      SPI_transfer (0x04);
	                       CsnPin_HIGH;
                     //RX_ADDR_P5 - configura endereço de recepção PIPE5
                               CsnPin_LOW;
                               SPI_transfer(WRITE_REG + RX_ADDR_P5);
                               SPI_transfer (0x5);
                               CsnPin_HIGH;

	  //TX_ADDR - configura endereço de transmissão
	   CsnPin_LOW;
	   StatusReg [1] = SPI_transfer (WRITE_REG + TX_ADDR);
	   for (i=0 ; i < 5 ;i++){
		   SPI_transfer (DATA_PIPE0_ADDRESS[i]);
	    }

	   CsnPin_HIGH;
	  //RX_PW_P0 - tamanho do buffer PIPE0
	   Write_Reg(RX_PW_P0, TX_PLOAD_WIDTH );
	  //RX_PW_P1 - tamanho do buffer PIPE1
	   Write_Reg(RX_PW_P1, TX_PLOAD_WIDTH );
	 //RX_PW_P2 - tamanho do buffer PIPE2
	   Write_Reg(RX_PW_P2, TX_PLOAD_WIDTH );
	 //RX_PW_P3 - tamanho do buffer PIPE3
	   Write_Reg(RX_PW_P3, TX_PLOAD_WIDTH );
	 //RX_PW_P4 - tamanho do buffer PIPE4
	   Write_Reg(RX_PW_P4, TX_PLOAD_WIDTH );
	 //RX_PW_P5 - tamanho do buffer PIPE5
	   Write_Reg(RX_PW_P5, TX_PLOAD_WIDTH );

	   for ( i=0 ; i < 10000 ;i++);
	   CEPin_HIGH;  // entra modo de RX
	   for( i=0; i < 150000; i++);

}


void flush_TX(){
	CsnPin_LOW;
	SPI_transfer (0b11100001);
	CsnPin_HIGH;
}

void flush_RX(){
	CsnPin_LOW;
	SPI_transfer (0b11100010);
	CsnPin_HIGH;
}

void Read_data(){

	uint32_t i;
	CsnPin_LOW;
	SPI_transfer(0x61);             //load RX payload
	for ( i=0 ; i< TX_PLOAD_WIDTH ; i++){
	    RX_PAYLOAD[i] = SPI_transfer(0xFF);
	  }
	CsnPin_HIGH;

}

void Write_data(){
	uint32_t i;
	CEPin_LOW;
	flush_TX(); //flush TX BUFFER
	//STATUS - faz reset ao registo STATUS
	Write_Reg(STATUS, 0x70 );
	// W_TX_PAYLOAD - envia os dados para o buffer FIFO TX
	CsnPin_LOW;
	SPI_transfer(0xA0);
	 for ( i=0 ; i<TX_PLOAD_WIDTH ; i++){
		SPI_transfer(TX_PAYLOAD[i]);
	   }
	CsnPin_HIGH;
	//CONFIG - ativa modo de transmissão
	Write_Reg(CONFIG, 0x0E); //0x0F|0b00001110
	// Dá um pulso no CE de pelo menos 10us para o pacote ser enviado
	CEPin_HIGH;
	for ( i=0 ; i<48 ; i++);   // VERIFICAR ISTO
	CEPin_LOW;
	for ( i=0 ; i<10 ; i++);
	CEPin_HIGH;   //?????????????????
	// Se o pacote foi enviado a flag TX_DS será disparada -- enhanced shockburst
	// MAX_RT também pode ser disparada
  }


void Ping_TX()
{
    //Write_data();
    //Write_data_To_data_pipeX(1);
}

void Process_Data(uint8_t Statpipe){//

	uint8_t i;

	// verificar mecanismo de identificação do dispositivo
	//****************************

	// MSG is a Service MSG (SMSG) and it is for me?
	if ((Statpipe==0) && (RX_PAYLOAD[4] == node.Device_ID)){
	    if (RX_PAYLOAD[2] == 1){ // request empty pipe SMSG?
	        // Attribute an empty pipe mechanism

	            for ( i = 0 ; i < 5 ; i++) {
	         // Check if device ID is already registered
	                if(RegisteredDevices[i].Device_Slot == OCCUPIED){
	                    if(RegisteredDevices[i].Device_ID == RX_PAYLOAD[0]){   // the is a non free slot with the same id?=> already registered
	                     // abort Pipe request
	                     // Send 0xF code for Pipe already registered. this prevent the registration of 2 devices with the same Id
	                     //Send_Error_Code(PIPE_ALREADY_REG,0x3,RegisteredDevices[i].Device_ID );
	                     // send pipe instead of the error code
	                     Send_COM_PIPE( RegisteredDevices[i].Device_Pipe, RegisteredDevices[i].Device_ID );  // send the msg
	                        return;
	                    }
	                }

	            }// end for
	        // if device ID was not registered... do it
	            for ( i = 0 ; i < 5 ; i++) {
	        // look for the first empty slot
	                if(RegisteredDevices[i].Device_Slot == FREE){
	        // fill the struct elements for device[i]
	                   RegisteredDevices[i].Device_Slot = OCCUPIED;
	                   RegisteredDevices[i].Device_ID = RX_PAYLOAD[0];
	                   RegisteredDevices[i].Device_Type = RX_PAYLOAD[1];
	        // pipe attribution mechanics
	                   RegisteredDevices[i].Device_Pipe = i+1;
	                   DeviceCnt++;
	                   Send_COM_PIPE( RegisteredDevices[i].Device_Pipe, RegisteredDevices[i].Device_ID);  // send the msg
	                   return;
	                }

	            }// end for
	         if(DeviceCnt>=5) {
	             // rotina pouco correta por causa do RX_PAYLOAD[0]
	             Send_Error_Code(NOPIPE_AVAILABLE,0x3, RX_PAYLOAD[0]); // the is not an empty pipe send CODE =0xB
	             return;
	         }
	    }// END of request empty pipe SMSG if loop
	} // end of SMSG

	if (Statpipe!=0){

        for ( i = 0 ; i < 5 ; i++) {
     // Check if device ID is already registered
            if(RegisteredDevices[i].Device_Slot == OCCUPIED){
                if(RegisteredDevices[i].Device_ID == RX_PAYLOAD[0]){   //  registered device
                    if(RX_PAYLOAD[1] == DSR){  // data send request?
                        TX_PAYLOAD[0] = node.Device_ID;
                        TX_PAYLOAD[4] = RegisteredDevices[i].Device_ID;
                        TX_PAYLOAD[1] = DSP;  // data send permission
                        Write_data_To_data_pipeX(RegisteredDevices[i].Device_Pipe,RegisteredDevices[i].Device_ID);
                    }

                    if(RX_PAYLOAD[1] == DATA_MSG){  // relevant data from node ?
                    // process the data from node
                    //    (.....)
                    }
                    return;
                }
            }

        }// end for
    // node is not registered... reject smg
    Send_Error_Code(PIPE_NOT_REG,0x3, RX_PAYLOAD[0]);
    return;
	}

}// Function end
/*
Description:
Send the attributed pipe to TX (answer to the request pipe msg)
 In/Out parameters:
 In: 'attrPipe' pipe number.
     'Destination_ID' ID of the destination node
  Return: none.
*/
void Send_COM_PIPE(uint8_t attrPipe, uint8_t Destination_ID ){
    uint8_t i;
    // adicionar mecanismo de identificação do dispositivo

      for ( i=0 ; i<TX_PLOAD_WIDTH ; i++){
      TX_PAYLOAD[i]=0; // make sure that all bytes in the payload are 0
      }
      TX_PAYLOAD[0]=node.Device_ID;
      TX_PAYLOAD[4]=Destination_ID;
      TX_PAYLOAD[2]=1;
      TX_PAYLOAD[3]=attrPipe;
      Write_data_To_data_pipeX(0,Destination_ID);
}

uint8_t get_pipe_from_STATUS(){

    uint8_t status =Read_Reg(STATUS);
    uint8_t PiPe = (readbit(status, 3)*4)+(readbit(status, 2)*2)+(readbit(status, 1));

   return PiPe;
  }


uint8_t Write_data_To_data_pipeX(uint8_t Pipe, uint8_t Destination_ID ) {

    uint32_t i;


    if (Pipe > 5) {
      // Erro: Pipe inválido
      return 0;
    }
    if (Pipe == 0) {
        //TX_ADDR - configura endereço de transmissão para o pipe0 do node=Destination_ID
        DATA_PIPE0_ADDRESS[0] = Destination_ID;
        CsnPin_LOW;
         SPI_transfer (WRITE_REG + TX_ADDR);
        for ( i = 0 ; i < 5 ; i++) {
            SPI_transfer (DATA_PIPE0_ADDRESS[i]);
        }
        CsnPin_HIGH;
        // configura o RX_ADDR_P0 = TX_ADDR para receber o ACK
        CsnPin_LOW;
        SPI_transfer (WRITE_REG + RX_ADDR_P0);
              for ( i = 0 ; i < 5 ; i++){
                 SPI_transfer (DATA_PIPE0_ADDRESS[i]);
               }
         CsnPin_HIGH;
    } else {
        DATA_PIPEx_ADDRESS[0]=Pipe;
        //TX_ADDR - configura endereço de transmissão
        CsnPin_LOW;
        SPI_transfer (WRITE_REG + TX_ADDR);
        for ( i = 0 ; i < 5 ; i++) {
            SPI_transfer (DATA_PIPEx_ADDRESS[i]);
        }
        CsnPin_HIGH;
        // configura o RX_ADDR_P0 = TX_ADDR para receber o ACK
        CsnPin_LOW;
        SPI_transfer (WRITE_REG + RX_ADDR_P0);
           for ( i = 0 ; i < 5 ; i++){
               SPI_transfer (DATA_PIPEx_ADDRESS[i]);
           }
           CsnPin_HIGH;
    }

    CEPin_LOW;
    flush_TX(); //flush TX BUFFER
    //STATUS - faz reset ao registo STATUS
    Write_Reg(STATUS, 0x70 );
    // W_TX_PAYLOAD - envia os dados para o buffer FIFO TX
    CsnPin_LOW;
    SPI_transfer(0xA0);
     for ( i=0 ; i<TX_PLOAD_WIDTH ; i++){
        SPI_transfer(TX_PAYLOAD[i]);
       }
    CsnPin_HIGH;
    //CONFIG - ativa modo de transmissão
    Write_Reg(CONFIG, 0x0E); //0x0F|0b00001110
    // Dá um pulso no CE de pelo menos 10us para o pacote ser enviado
    CEPin_HIGH;
    for ( i=0 ; i<48 ; i++);   // VERIFICAR ISTO
    CEPin_LOW;
    for ( i=0 ; i<10 ; i++);
    CEPin_HIGH;   //?????????????????
    // Se o pacote foi enviado a flag TX_DS será disparada -- enhanced shockburst
    // MAX_RT também pode ser disparada
    return 1;
}

/*
Description:
Send error code
 In/Out parameters:
 In: errorcode    -- NOPIPE_AVAILABLE 0xB -> the is not an empty pipe send CODE =0xB
                  -- PIPE_ALREADY_REG 0XF -> Send 0xF code for Pipe already registered
     Request_type -- 0x1-> request empty pipe SMSG
                  -- 0x3-> error code "notifier"
  Return: none.
*/
void Send_Error_Code(uint8_t errorcode, uint8_t Request_type,uint8_t Destination_ID){

    uint8_t i;
    for ( i=0 ; i<TX_PLOAD_WIDTH ; i++){
      TX_PAYLOAD[i]=0; // make sure that all bytes in the payload are 0
     }
     TX_PAYLOAD[0]=node.Device_ID;
     TX_PAYLOAD[4]=Destination_ID;
     TX_PAYLOAD[2]=Request_type;  // SMSG
     TX_PAYLOAD[3]=errorcode;  // error code
     Write_data_To_data_pipeX(0,Destination_ID);

}


















