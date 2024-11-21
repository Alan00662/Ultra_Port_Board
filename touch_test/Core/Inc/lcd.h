#ifndef __LCD_H
#define __LCD_H

#include "main.h"

#define WHITE                       0xFFFF  /* 白色 */
#define BLACK                       0x0000  /* 黑色 */
#define RED                         0xF800  /* 红色 */
#define GREEN                       0x07E0  /* 绿色 */
#define BLUE                        0x001F  /* 蓝色 */
#define MAGENTA                     0xF81F  /* 品红色 */
#define YELLOW                      0xFFE0  /* 黄色 */
#define CYAN                        0x07FF  /* 青色 */
#define BROWN                       0xBC40  /* 棕色 */
#define BRRED                       0xFC07  /* 棕红色 */
#define GRAY                        0x8430  /* 灰色 */
#define DARKBLUE                    0x01CF  /* 深蓝色 */
#define LIGHTBLUE                   0x7D7C  /* 浅蓝色 */
#define GRAYBLUE                    0x5458  /* 灰蓝色 */
#define LIGHTGREEN                  0x841F  /* 浅绿色 */
#define LGRAY                       0xC618  /* 浅灰色 */
#define LGRAYBLUE                   0xA651  /* 浅灰蓝色 */
#define LBBLUE                      0x2B12  /* 浅棕蓝色 */

void SPI_WriteComm(uint16_t CMD);
void SPI_WriteData(uint16_t data);
void lcd_init(void);
void lcd_bl_on(void);

#endif
