#ifndef __BSPQSPIBOOT_H__
#define __BSPQSPIBOOT_H__

#include <string.h>
#include "stdbool.h"
#include "stm32h7xx.h"
#define QSPI_FLASH_MEM_ADDR       0x90000000

#define QSPI_FLASH_SIZE           25

#define QSPI_FLASH_SECTOR_SIZE    (4*1024)
#define QSPI_FLASH_PAGE_SIZE      (256)
#define QSPI_FLASH_END_ADDR       (1<<QSPI_FLASH_SIZE)
#define QSPI_FLASH_BYTE_SIZE      (8*1024*1024)

typedef enum
{
  BQB_Cmd_ReadID = 0x9F,
  BQB_Cmd_ReadStatus1 = 0x05,
  BQB_Cmd_WriteEnable = 0x06,
  BQB_Cmd_SectorErase = 0x20,
  BQB_Cmd_ChipErase = 0xC7,
  BQB_Cmd_PageProgram_Quad = 0x32,
  BQB_Cmd_FastRead_Quad = 0xEB,
}BQB_Cmd_E;

int BspQspiBoot_Init(void);

uint32_t BspQspiBoot_ReadID(void);
int BspQspiBoot_EraseSector(uint32_t ulSectorAddr);
int BspQspiBoot_EraseChip(void);
int BspQspiBoot_WritePage(uint8_t *p_Buff,uint32_t ulWriteAddr,uint16_t usWriteSize);
int BspQspiBoot_ReadBuff(uint8_t *p_Buff,uint32_t ulReadAddr,uint32_t ulReadSize);
int BspQspiBoot_MemMapped(void);
int BspQspiBoot_QuitMemMapped(void);

void BspQspiBoot_Test(void);

void BspQspiBoot_JumpToApp(void);

#endif /**/
