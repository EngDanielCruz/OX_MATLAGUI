/*
 * Oled.h
 *
 *  Created on: 08/09/2015
 *      Author: Dany
 */

#ifndef OLED_H_
#define OLED_H_
#endif /* OLED_H_ */



//*****************************************************************************
//							#defines
//*****************************************************************************

#define Oled_CS_LOW  GPIOD->DATA &= ~(1<<1)
#define Oled_CS_HIGH GPIOD->DATA |= (1<<1)
#define Oled_DC_HIGH GPIOC->DATA |= (1<<7)
#define Oled_DC_LOW  GPIOC->DATA &= ~(1<<7)
#define Oled_RST_HIGH GPIOC->DATA |= (1<<6)
#define Oled_RST_LOW  GPIOC->DATA &= ~(1<<6)


#define SSD1306_LCDWIDTH                  128
#define SSD1306_LCDHEIGHT                 64

//-------------------------Command map-----------------------------------------
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x01
#define SSD1306_SWITCHCAPVCC 0x02

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A


//*****************************************************************************
//							Prototypes functions
//*****************************************************************************
void SPI_Oled_Init(void);
void Oled_Init();
/*
Description:
Load TX buffer deploy it
 In/Out parameters:
 In: 'data'
  Return: none.
*/
void SPI_Oled_transfer(uint8_t data);
/*
Description:
Write a command
 In/Out parameters:
 In: 'cmd' command
  Return: none.
*/
void Oled_Write_Cmd(uint8_t cmd);
/*
Description:
Write a single byte in one segment of the display (column)
 In/Out parameters:
 In: 'data'
  Return: none.
*/
void Oled_Write_Data(uint8_t data);
/*
Description:
Write a character on display
 In/Out parameters:
 In: 'character'
 Return: none.
*/
void Oled_Write_Char( char character);
/*
Description:
Write a string in coordinate  x,y
 In/Out parameters:
 In: 'string'
     'x' x coordinate [0,127]
     'y' y coordinate [0,63].
  Return: none.
*/
void Oled_Write_String( uint8_t x, uint8_t y, char *string);
/*
Description:
Turn off the display (not tested)
 In/Out parameters:
 In:
 Return: none.
*/
void Oled_TrunOff(void);
/*
Description:
Clear the entire display area
 In/Out parameters:
 In: none
 Return: none.
*/
void Oled_Clear_Display(void);
/*
Description:
Turn one pixel on
 In/Out parameters:
 In: 'x' x coordinate [0,127]
     'y' y coordinate [0,63].
 Return: none.
*/
void Oled_Draw_Pixel(uint8_t x, uint8_t y);
/*
Description:
Draw a horizontal line
 In/Out parameters:
 In: 'x' x line origin coordinate [0,127]
     'y' y line origin coordinate  [0,63].
     'Nofpix' line length in pixels
 Return: none.
*/
void Oled_Draw_Hline(uint8_t x, uint8_t y, uint8_t Nofpix);
/*
Description:
Draw a horizontal line
 In/Out parameters:
 In: 'x' x line origin coordinate [0,127]
     'y' y line origin coordinate  [0,63].
     'Nofpix' line length in pixels
 Return: none.
*/
void Oled_Draw_Vline(uint8_t x, uint8_t y, uint8_t Nofpix);
/*
Description:
Draw a horizontal line
 In/Out parameters:
 In: 'x' x line origin coordinate [0,127]
     'y' y line origin coordinate  [0,63].
     'Nofpix' line length in pixels
 Return: none.
*/
/*
Description:
this function is used to write what ever you wont when the display is initialized
 In/Out parameters:
 In:
 Return: none.
*/
void Oled_DisplayWhen_Init();



void Oled_int2string(uint8_t x, uint8_t y, uint16_t intnumber);












