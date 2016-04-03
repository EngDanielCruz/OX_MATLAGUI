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
// comment this lines to disable this functionalities
#define UART_ON
//#define HIGH_SPEED
//#define OLED_ON

#define THRHOLD 4.0
#define THRHOLD_N -4.0

#define  MAXSAMPLES 300       // Maximum number of samples. defines the length of the raw data arrays
#define  SAMPLES_TIME 3      // sample time in seconds*Sample rate
#define  IR_acc 0
#define  RED_acc 1

//***********MAX30100 registers*********

#define MAX30100_ADDR 0x57  // 0xAE>>1

//***********Register map***************
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
    uint8_t PWcontrol;  // used only for shifting the values from max fifo
    uint16_t SamplesWindow;
    uint16_t SampRate;
};

struct samplingoptions { // struct type to represent config commands receive in serial port
    uint8_t IRRawPlot;
    uint8_t REDRawPlot;
    uint8_t IRPlot;
    uint8_t RedPlot;
};

//extern uint8_t MAX_FIFO_DATA[64] ;
//extern const uint16_t NofSamples;
extern uint16_t sample_cnt;
extern float IR_FIFO_DATA[];
extern float RED_FIFO_DATA[];
extern float Filt_IRdata[];
extern float Filt_REDdata[];
extern uint16_t NofSamples;
uint8_t  SP_HighSpeed_Divisor;  // variable to hold down sample divisor in high speed mode


//*************************************************************************
//                          FUNCTIONS PROTOTYPES
//*************************************************************************

void Max30100_Init(void);
void Read_MAX_DATAFIFO(void);
void Read_MAX_Temp(float volatile *tempvalue);
void StopSampling(void);
void StartSampling(void);
void getPeak(float arrvalue[], uint16_t indexval, uint16_t Peaks_index[],uint8_t *npeaks);
void Get_HRate(uint8_t *nofpeaks,float Hr[]);
void Get_SPO2(float irrms, float redrms, float ir_dc,float red_dc,float *spo2);
void GetPeak_fromFIFO(uint16_t indexval, uint16_t Peaks_index[],uint8_t *npeaks);













