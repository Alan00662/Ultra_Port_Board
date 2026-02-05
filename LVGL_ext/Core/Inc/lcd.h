#ifndef __LCD_H
#define __LCD_H

#include "main.h"

void SPI_WriteComm(uint16_t CMD);
void SPI_WriteData(uint16_t data);
void lcd_init(void);

#endif
