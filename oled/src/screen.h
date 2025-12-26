#ifndef __SCREEN_C_H
#define __SCREEN_C_H

#include "stdio.h"
#include <pico/stdlib.h>
#include "hardware/spi.h"
#include "hardware/pwm.h"

/**
 * data
 **/
#define UBYTE uint8_t
#define UWORD uint16_t
#define UDOUBLE uint32_t

/**
 * GPIOI config
 **/

#define OLED_PIN_SDA_DATA 6 // I2C_SDA | data input
#define OLED_PIN_SCL_CLOCKIN 7 // I2C_SCL | clock input
#define OLED_PIN_DATA_COMMAND 8 // OLED_DC | Data (high) / Command (low)

#define OLED_PIN_CHIPSELECT 9 // OLED_CS | Chip select (low active)
#define OLED_PIN_CLOCKIN 10 // OLED_CLK | Clock input
#define OLED_PIN_DATA 11 // OLED_DIN | SPI Mosi pin for Data input
#define OLED_PIN_RESET 12 //  OLED_RST | Reset
#define OLED_PIN_BL 13 // looks like POWER pin. But not in docs... not 100% sure

#define SPI_PORT spi1

#define IIC_CMD 0X00
#define IIC_RAM 0X40

#define OLED_WIDTH 128 
#define OLED_WIDTH_BYTES 16 
#define OLED_HEIGHT 64 


void OLED_Init(void);
void OLED_Clear(void);
void OLED_Display(const UBYTE *image);
void OLED_PW(int value);

#endif
