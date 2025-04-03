#ifndef _BSP_QSPI_W25Q128_H
#define _BSP_QSPI_W25Q128_H
#include "stm32h7xx_hal_qspi.h"
enum
{
    SST25VF016B_ID  =   0xBF2541,
    MX25L1606E_ID   = 0xC22015,
    W25Q64_ID       = 0xEF4017,
    W25Q128_ID      = 0xEF4018,
    W25Q256_ID      = 0xEF4019
};

#define QSPI_FLASH_MEM_ADDR         0x90000000

/* W25Q256JV基本信息 */

#define QSPI_FLASH_SIZE     24                      /* Flash大小，2^24 = 16MB*/
#define QSPI_SECTOR_SIZE    (4 * 1024)              /* 扇区大小，4KB */
#define QSPI_PAGE_SIZE      256                     /* 页大小，256字节 */
#define QSPI_END_ADDR       (1 << QSPI_FLASH_SIZE)  /* 末尾地址 */
#define QSPI_FLASH_SIZES    16 * 1024 * 1024         /* Flash大小，2^25 = 32MB*/

/* W25Q128相关命令 */
#define WRITE_ENABLE_CMD                        0x06    /* 写使能指令 */
#define READ_ID_CMD2                            0x9F    /* 读取ID命令 */
#define READ_STATUS_REG_CMD                     0x05    /* 读取状态命令 */
#define QUAD_INOUT_FAST_READ_4_BYTE_ADDR_CMD    0xEC    /* 32bit地址的4线快速读取命令 */
#define CMD_ERASE_CHIP 													0xC7
#define QUAD_IN_FAST_PROG_CMD 									0x32
#define QUAD_OUT_FAST_READ_CMD 									0x6B
#define QUAD_INOUT_FAST_READ_CMD 								0xEB
#define CMD_ERASE_BLOCK_64K                     0xD8    /* 清除扇区，64K扇区,对应手册上的指令为: Block Erase (64KB) */

int bsp_InitQSPI_W25Q256(void);
int QSPI_EraseSector(uint32_t address);
int QSPI_WriteBuffer(uint8_t *_pBuf, uint32_t _uiWriteAddr, uint16_t _usWriteSize);
int QSPI_ReadBuffer(uint8_t *_pBuf, uint32_t _uiReadAddr, uint32_t _uiSize);
uint32_t QSPI_ReadID(void);
int QSPI_EraseChip(void);

int QSPI_MemoryMapped(void);
int QSPI_QuitMemoryMapped(void);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
