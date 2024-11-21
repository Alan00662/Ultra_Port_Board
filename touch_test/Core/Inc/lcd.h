#ifndef __LCD_H
#define __LCD_H

#include "main.h"

#define WHITE                       0xFFFF  /* ��ɫ */
#define BLACK                       0x0000  /* ��ɫ */
#define RED                         0xF800  /* ��ɫ */
#define GREEN                       0x07E0  /* ��ɫ */
#define BLUE                        0x001F  /* ��ɫ */
#define MAGENTA                     0xF81F  /* Ʒ��ɫ */
#define YELLOW                      0xFFE0  /* ��ɫ */
#define CYAN                        0x07FF  /* ��ɫ */
#define BROWN                       0xBC40  /* ��ɫ */
#define BRRED                       0xFC07  /* �غ�ɫ */
#define GRAY                        0x8430  /* ��ɫ */
#define DARKBLUE                    0x01CF  /* ����ɫ */
#define LIGHTBLUE                   0x7D7C  /* ǳ��ɫ */
#define GRAYBLUE                    0x5458  /* ����ɫ */
#define LIGHTGREEN                  0x841F  /* ǳ��ɫ */
#define LGRAY                       0xC618  /* ǳ��ɫ */
#define LGRAYBLUE                   0xA651  /* ǳ����ɫ */
#define LBBLUE                      0x2B12  /* ǳ����ɫ */

void SPI_WriteComm(uint16_t CMD);
void SPI_WriteData(uint16_t data);
void lcd_init(void);
void lcd_bl_on(void);

#endif
