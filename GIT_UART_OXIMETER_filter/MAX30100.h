/*
 * MAX30100.h
 *
 *  Created on: 19/10/2015
 *      Author: Dany
 */

#ifndef MAX30100_H_
#define MAX30100_H_
#endif /* MAX30100_H_ */


//*****************************************************************************
//                          #defines
//*****************************************************************************
#define  MAXSAMPLES 2000
#define  IR_acc 0
#define  RED_acc 1

#define MAX30100_ADDR 0x57  // 0xAE>>1

//                       ----REGISTER MAP----
// STATUS
#define INTERRUPT_STATUS 0x00
#define INTERRUPT_ENABLE 0x01

//FIFO
#define FIFO_WRITE_PTR    0x02
#define FIFO_OVF_COUNTER 0x03
#define FIFO_READ_PTR     0x04
#define FIFO_DATA_REG     0x05

// CONFIGURATION
#define MODE_CONFIG  0x06
#define SPO2_CONFIG  0x07
#define LED_CONFIG   0X09

// TEMPERATURE
#define TEMP_INTEGER  0X16
#define TEMP_FRACTION 0X17

//PART ID

#define REVISION_ID 0XFE
#define PART_ID     0XFF

//*****************************************************************************
//                            Variables
//*****************************************************************************
struct STATUS_REG  {
    uint8_t A_FULL:   1;
    uint8_t TEMP_RDY: 1;
    uint8_t HR_RDY:   1;
    uint8_t SPO2_RDY: 1;
    uint8_t RESERVE1: 1;
    uint8_t RESERVE2: 1;
    uint8_t RESERVE3: 1;
    uint8_t PWR_RDY:  1;
};

struct confcom { // struct type to represent config commands receive in serial port
    uint16_t NofSamples;
    float alpha;
    uint16_t taps;
    uint16_t filt_type;
};
struct configregister { // struct type to represent REGISTERconfig commands receive in serial port
    uint16_t modeconfig;
    uint16_t spo2config;
    uint16_t intconfig;
    uint16_t ledconfig;

};

//extern uint8_t MAX_FIFO_DATA[64] ;
//extern const uint16_t NofSamples;
extern uint16_t sample_cnt;
extern int32_t IR_FIFO_DATA[];
extern float RED_FIFO_DATA[];
extern uint16_t NofSamples;

//*************************************************************************
//                          FUNCTIONS PROTOTYPES
//*************************************************************************

void Max30100_Init(void);
void Read_MAX_DATAFIFO(void);
void StopSampling(void);
void StartSampling(void);














