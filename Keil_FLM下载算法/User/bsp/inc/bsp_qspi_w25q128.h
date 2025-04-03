



#ifndef _BSP_QSPI_W25Q128_H
#define _BSP_QSPI_W25Q128_H

#include "stm32h7xx_hal.h"
#define QSPI_FLASH_MEM_ADDR         			  0x90000000 //W25Qxx系列芯片的首地址！！


/* W25Q128JV基本信息 */
//W25Q128JV是16M的FLASH
#define QSPI_FLASH_SIZE     						  24                       /* Flash大小，2^24 = 16MB*/
#define QSPI_SECTOR_SIZE    						  (4 * 1024)               /* 扇区大小，4KB */
#define QSPI_PAGE_SIZE      						  128                      /* 页大小，128字节 */
#define QSPI_END_ADDR       						  (1 << QSPI_FLASH_SIZE)   /* 末尾地址 */
#define QSPI_FLASH_SIZES                       16 * 1024 * 1024         /* Flash大小，16 * 1024 * 1024 = 16MB*/


/* W25Q128JV相关命令 */
#define CMD_WRITE_ENABLE	                      0x06    /* 写使能指令,对应手册上的指令为: Write Enable */
#define CMD_GET_REG1		                        0x05    /* 读取状态命令,对应手册上的指令为: Read Status Register-1 */
#define CMD_WRITE                               0x32    /* 对应手册上的指令是: Quad Input Page Program   */ 
#define CMD_READ                                0xEB    /* 对应手册上的指令是: Fast Read Quad I/O  */ 
#define CMD_ERASE_BLOCK_64K                     0xD8    /* 清除扇区，64K扇区,对应手册上的指令为: Block Erase (64KB) */
#define CMD_ERASE_CHIP                          0xC7    /* 清除整块芯片，耗时长,对应手册上的指令为: Chip Erase */


uint8_t QSPI_WriteEnable(void);//写使能
uint8_t QSPI_AutoPollingMemReady(void);//等待完成
uint8_t bsp_InitQSPI_W25Q128(void);//初始化QSPI硬件
uint8_t QSPI_Erase_Bluck_64K(uint32_t address);//擦除某个扇区 以64K为单位
uint8_t QSPI_WriteBuffer(uint8_t *_pBuf, uint32_t _uiWriteAddr, uint16_t _usWriteSize);//往特定地址写数据
uint8_t QSPI_EraseChip(void);//抹除整片芯片
uint8_t QSPI_MemoryMapped(void);//内存映射，下载算法中需要，在后面的项目工程中也需要调用！！


#endif

