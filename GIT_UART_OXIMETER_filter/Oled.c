/*
 * Oled.c
 *
 *  Created on: 08/09/2015
 *      Author: Dany
 */


#include <LM4F120E5QR.h>
#include "Oled.h"
#include "font.h"
void SPI_Oled_Init(){
	uint32_t i;

//	1. Enable the SSI module using the RCGCSSI register
	SYSCTL->RCGCSSI|=(1<<1);
//	2. Enable the clock to the appropriate GPIO module via the RCGCGPIO register
	SYSCTL->RCGCGPIO |=(1<<3);
	for ( i=0 ; i < 40000 ;i++);
//	3. Set the GPIO AFSEL bits for the appropriate pins
	GPIOD->AFSEL |=(1<<0)|(1<<1)|(1<<3);
//	GPIOD->AFSEL &=~(1<<1);               // take control of the CSN Pin disable alternate function
										 //	otherwise cs pin will pulse every 8bit of data. we must send several bytes in one csn low cycle
//	4. Configure the PMCn fields in the GPIOPCTL register to assign the SSI signals to the appropriate pins
	GPIOD->PCTL |= (2<<0)|(2<<4)|(2<<12);
//  P1 config
	GPIOD->DIR |=(1<<1);  //P3 is an output
	GPIOD->DEN |=0xB;    // digital enable
	//GPIOD->DATA |=(1<<3); // CS=1

//	For each of the frame formats, the SSI is configured using the following steps:
//	1. Ensure that the SSE bit in the SSICR1 register is clear before making any configuration changes.
	SSI1->CR1 &=~(1<<1) ;
	//	2. Select whether the SSI is a master or slave:
	SSI1->CR1 = 0x00000000 ;
//	3. Configure the SSI clock source by writing to the SSICC register.
	SSI1->CC =0x5;
//	4. Configure the clock prescale divisor by writing the SSICPSR register.
	SSI1->CPSR =0x2;  // =CPSDVSR -> bus clock must be at least 2 times faster than the SSICLk...SSICLk<25MHz
//	5. Write the SSICR0 register with the following configuration: Mode0=default state
	SSI1->CR0 |=(0x4<<8); // SCR = 0x0, SSIClk = SysClk / (CPSDVSR * (1 + SCR)) =>8x10e6 = 80x10e6 / 2 * (1 + 0x4))
	SSI1->CR0 |=(0x07<<0);  // 8bits data size
//	SSI Serial Clock Phase
//	SSI1->CR0 &=~(0x1<<7);	// 1 Data is captured on the second clock edge transition.
//	SSI Serial Clock Polarity
//	SSI1->CR0 |=(0x1<<6);  // 1 A steady state High value is placed on the SSIClk pin when data is not being transferred.
//	7. Enable the SSI by setting the SSE bit in the SSICR1 register.
	SSI1->CR1 |=(1<<1);

}

void SPI_Oled_transfer(uint8_t data){
	while((SSI1->SR & (1<<0)) == 0 ){};  // wait until FIFO empty
	SSI1->DR = data;
	while((SSI1->SR & (1<<0)) == 0 ){};  // wait until FIFO empty
}

void Oled_Write_Cmd(uint8_t cmd){
	Oled_DC_LOW;
	//Oled_CS_LOW;
	SPI_Oled_transfer(cmd);
	//Oled_CS_HIGH;
}

void Oled_Write_Data(uint8_t data){

	Oled_DC_HIGH;
	//Oled_CS_LOW;
	SPI_Oled_transfer(data);
	//Oled_CS_HIGH;
}

void Oled_Init(){
	uint32_t i;
	Oled_RST_LOW;
	for ( i=0 ; i < 40000 ;i++); //set RES# pin LOW (logic low) for at least 3us (100us)
	Oled_RST_HIGH;
	for ( i=0 ; i < 40000 ;i++);
	Oled_Write_Cmd(SSD1306_CHARGEPUMP);
	Oled_Write_Cmd(0x14); // ??
	for ( i=0 ; i < 40000 ;i++);
	Oled_Write_Cmd(SSD1306_DISPLAYON);

	Oled_Write_Cmd(SSD1306_SETDISPLAYOFFSET);
	Oled_Write_Cmd(0x00);
	Oled_Write_Cmd(SSD1306_SETSTARTLINE);
	Oled_Write_Cmd(0xA1);  //set segment re-map (horizontal flip) - reset value is 0xa0 (or 0xa1)
								//Set Multiplex Ratio
	Oled_Write_Cmd(SSD1306_COMSCANDEC); // reset value = normal mode
	Oled_Write_Cmd(0xDA);  //set COM pins hardware configuration
	Oled_Write_Cmd(0x12);  //reset value is 0x12
	Oled_Write_Cmd(SSD1306_SETCONTRAST);
	Oled_Write_Cmd(0xFF);
	//Oled_Write_Cmd(SSD1306_DISPLAYALLON_RESUME); //disable entire display on
	Oled_Write_Cmd(SSD1306_NORMALDISPLAY);
	//Oled_Write_Cmd(SSD1306_SETDISPLAYCLOCKDIV);
	//Oled_Write_Cmd(0x80);
	//Oled_Write_Cmd(SSD1306_SETVCOMDETECT);  //vcomh deselect level (brightness)

	Oled_Write_Cmd(0x20); //set horizontal addressing mode for screen clear
	Oled_Write_Cmd(0x01);

	Oled_Write_Cmd(SSD1306_COLUMNADDR); //set column start and end address
	Oled_Write_Cmd(0x00); //set column start address
	Oled_Write_Cmd(0x7f); //set column end address (127)  (128 bites)

	Oled_Write_Cmd(SSD1306_PAGEADDR);  //set row start and end address
	Oled_Write_Cmd(0x00); //set row start address
	Oled_Write_Cmd(0x07); //set row end address (8 pages) (8*8=64 bites)

	 for (i=0; i<1024; i++) Oled_Write_Data(0x00); // clear oled screen



	 Oled_Write_Cmd(SSD1306_MEMORYMODE);  //set page addressing mode
	 Oled_Write_Cmd(0x02);   				//Page Addressing Mode (RESET)

	 /*Oled_Write_Cmd(SSD1306_LEFT_HORIZONTAL_SCROLL); //set right horizontal scroll
	 Oled_Write_Cmd(0x0); //dummy byte
	 Oled_Write_Cmd(0x0); //page start address
	 Oled_Write_Cmd(0x7); //scroll speed
	 Oled_Write_Cmd(0x7); //page end address
	 Oled_Write_Cmd(0x0); //dummy byte
	 Oled_Write_Cmd(0xff); //dummy byte
	 Oled_Write_Cmd(SSD1306_ACTIVATE_SCROLL);  //start scrolling

	 */

	 Oled_Write_Cmd(0xB5); //set page start address to page 0
	 Oled_Write_Cmd(0x00); // Low nibble
	 Oled_Write_Cmd(0x10);  //=> High nibble  0x0 ... [0x10;0F]

	 for ( i=0 ; i < 400 ;i++);


}// end of Oled_init


void Oled_Write_String(uint8_t x, uint8_t y, char *string){

    uint8_t page=0x0;

    uint8_t x1=(x>>4);
    x1 |=(1<<4);
    uint8_t x2=(x & 0xF);
    page = (y/8) + 0xB0;

    Oled_Write_Cmd(page); //set page start address
    Oled_Write_Cmd(x1);   // low nibble 4 bits
    Oled_Write_Cmd(x2);   // high nibble 4bits + bit 5=1

	while(*string){
		Oled_Write_Char( *string);
		string++;
	}
}


void Oled_Write_Char(char character){

    uint8_t i;
	for ( i=0 ; i < 6;i++){
		Oled_Write_Data( ASCII[character-0x20][i]);
	}

}


void Oled_TrunOff(){

	Oled_Write_Cmd(SSD1306_DISPLAYOFF);
}

void Oled_Clear_Display(){
    uint32_t i;
    Oled_Write_Cmd(0x20); //set horizontal addressing mode for screen clear
    Oled_Write_Cmd(0x01);

    Oled_Write_Cmd(SSD1306_COLUMNADDR); //set column start and end address
    Oled_Write_Cmd(0x00); //set column start address
    Oled_Write_Cmd(0x7f); //set column end address (127)  (128 bites)

    Oled_Write_Cmd(SSD1306_PAGEADDR);  //set row start and end address
    Oled_Write_Cmd(0x00); //set row start address
    Oled_Write_Cmd(0x07); //set row end address (8 pages) (8*8=64 bites)

    for (i=0; i<1024; i++) Oled_Write_Data(0x00); // clear oled screen



    Oled_Write_Cmd(SSD1306_MEMORYMODE);  //set page addressing mode
    Oled_Write_Cmd(0x02);                  //Page Addressing Mode (RESET)
}


void Oled_Draw_Pixel(uint8_t x, uint8_t y){

        uint8_t page=0x0;
        uint8_t pagebit=0x0;

        uint8_t x1=(x>>4);
        x1 |=(1<<4);
        uint8_t x2=(x & 0xF);
        page = (y/8) + 0xB0;
        pagebit = y-(y/8)*8;

        Oled_Write_Cmd(page); //set page start address
        Oled_Write_Cmd(x1);
        Oled_Write_Cmd(x2);  //=> to 0x0 ... [0x10;0F]

        Oled_Write_Data( (1<<pagebit));

}

void Oled_Draw_Hline(uint8_t x, uint8_t y, uint8_t Nofpix){

        uint8_t i;
        uint8_t page=0x0;
        uint8_t pagebit=0x0;

        uint8_t x1=(x>>4);
        x1 |=(1<<4);
        uint8_t x2=(x & 0xF);
        page = (y/8) + 0xB0;
        pagebit = y-(y/8)*8;

        Oled_Write_Cmd(page); //set page start address
        Oled_Write_Cmd(x1);
        Oled_Write_Cmd(x2);  //=> to 0x0 ... [0x10;0F]

        for (i=0; i<Nofpix; i++){
            Oled_Write_Data((1<<pagebit));
        }
}

void Oled_Draw_Vline(uint8_t x, uint8_t y, uint8_t Nofpix){

        uint8_t i;
        uint8_t page=0x0;
        //uint8_t pagebit=0x0;

        uint8_t x1=(x>>4);
        x1 |=(1<<4);
        uint8_t x2=(x & 0xF);
        page = (y/8) + 0xB0;
       // pagebit = y-(y/8)*8;  // y/8=integer

        Oled_Write_Cmd(page); //set page start address
        Oled_Write_Cmd(x1);   // segment low nibble
        Oled_Write_Cmd(x2);  // segment high nibble

        uint8_t lastpage = ((y+Nofpix)/8)+0xB0;


            uint8_t k=0;
            for(i=page; i<lastpage; i++){
                Oled_Write_Cmd(i); //set page start address
                Oled_Write_Cmd(x1);   // segment low nibble
                Oled_Write_Cmd(x2);  // segment high nibble
                for ( k=0 ; k < 4 ;k++);
                Oled_Write_Data(0xFF);
            }
            Oled_Write_Cmd(lastpage); //set page start address
            Oled_Write_Cmd(x1);   // segment low nibble
            Oled_Write_Cmd(x2);  // segment high nibble
            for ( k=0 ; k < 4 ;k++);
            uint32_t lastbit = Nofpix-(Nofpix/8)*8;
            Oled_Write_Data(0xFF>>(7-lastbit));

}

void Oled_DisplayWhen_Init(){

   // Oled_Write_String(56,30, "0");
    Oled_Draw_Hline(0, 0, 127);
    Oled_Draw_Hline(0, 15, 127);
    //Oled_Draw_Vline(0, 0, 63);
    //Oled_Draw_Vline(127, 0, 63);
    Oled_Write_String(0,24, "HR:");
    Oled_Write_String(0,40, "SPO2:");


}


void Oled_int2string(uint8_t x, uint8_t y, uint16_t intnumber){

    uint8_t i;
    char  ascii[10]={'0','1','2','3','4','5','6','7','8','9'};
    char istData[3];
    //istData[0] = ascii[(intnumber/1000)];
    //intnumber = intnumber % 1000;
    istData[0] = ascii[(intnumber/100)];
    intnumber = intnumber % 100;
    istData[1] = ascii[(intnumber/10)];
    intnumber = intnumber % 10;
    istData[2] = ascii[(intnumber)];
    //Oled_Write_String( x,  y,istData);

    uint8_t page=0x0;
    uint8_t x1=(x>>4);
    x1 |=(1<<4);
    uint8_t x2=(x & 0xF);
    page = (y/8) + 0xB0;

    Oled_Write_Cmd(page); //set page start address
    Oled_Write_Cmd(x1);   // low nibble 4 bits
    Oled_Write_Cmd(x2);   // high nibble 4bits + bit 5=1

    for(i=0; i<3; i++){
        Oled_Write_Char( istData[i]);
    }

}




















