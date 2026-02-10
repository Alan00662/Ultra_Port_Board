#include "main.h"
#include "BspQspiBoot.h"
extern QSPI_HandleTypeDef hqspi;

#define QSPI_CLK_ENABLE()               __HAL_RCC_QSPI_CLK_ENABLE()
#define QSPI_CLK_DISABLE()              __HAL_RCC_QSPI_CLK_DISABLE()
#define QSPI_CS_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define QSPI_CLK_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define QSPI_BK1_D0_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()
#define QSPI_BK1_D1_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()
#define QSPI_BK1_D2_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOE_CLK_ENABLE()
#define QSPI_BK1_D3_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()

#define QSPI_MDMA_CLK_ENABLE()          __HAL_RCC_MDMA_CLK_ENABLE()
#define QSPI_FORCE_RESET()              __HAL_RCC_QSPI_FORCE_RESET()
#define QSPI_RELEASE_RESET()            __HAL_RCC_QSPI_RELEASE_RESET()

#define QSPI_CS_PIN                     GPIO_PIN_11
#define QSPI_CS_GPIO_PORT               GPIOC
#define QSPI_CS_GPIO_AF                 GPIO_AF9_QUADSPI

#define QSPI_CLK_PIN                    GPIO_PIN_2
#define QSPI_CLK_GPIO_PORT              GPIOB
#define QSPI_CLK_GPIO_AF                GPIO_AF9_QUADSPI

#define QSPI_BK1_D0_PIN                 GPIO_PIN_7
#define QSPI_BK1_D0_GPIO_PORT           GPIOE
#define QSPI_BK1_D0_GPIO_AF             GPIO_AF9_QUADSPI

#define QSPI_BK1_D1_PIN                 GPIO_PIN_8
#define QSPI_BK1_D1_GPIO_PORT           GPIOE
#define QSPI_BK1_D1_GPIO_AF             GPIO_AF9_QUADSPI

#define QSPI_BK1_D2_PIN                 GPIO_PIN_9
#define QSPI_BK1_D2_GPIO_PORT           GPIOE
#define QSPI_BK1_D2_GPIO_AF             GPIO_AF9_QUADSPI

#define QSPI_BK1_D3_PIN                 GPIO_PIN_10
#define QSPI_BK1_D3_GPIO_PORT           GPIOE
#define QSPI_BK1_D3_GPIO_AF             GPIO_AF9_QUADSPI

int BspQspiBoot_Init(void)
{
  uint32_t i;
  char *p;
  
  /* 将句柄手动清零，防止作为全局变量的时候没有清零 */
  p = (char *)&hqspi;
  for (i = 0; i < sizeof(QSPI_HandleTypeDef); i++)
  {
    *p++ = 0;
  }
  
  /* 复位QSPI */
  hqspi.Instance = QUADSPI;   
  
  if (HAL_QSPI_DeInit(&hqspi) != HAL_OK)
  {
    return 1;
  }
  
  /* 设置时钟速度，QSPI clock = 200MHz / (ClockPrescaler+1) = 100MHz */
  hqspi.Init.ClockPrescaler = 1;
  
  /* 设置FIFO阀值，范围1 - 32 */
  hqspi.Init.FifoThreshold = 1;
  
  /* 
      QUADSPI在FLASH驱动信号后过半个CLK周期才对FLASH驱动的数据采样。
      在外部信号延迟时，这有利于推迟数据采样。
  */
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
  
  /* Flash大小是2^(FlashSize + 1) = 2^23 = 8MB */
  hqspi.Init.FlashSize = QSPI_FLASH_SIZE; //QSPI_FLASH_SIZE - 1; 2020-03-04, 需要扩大一倍，否则内存映射方位最后1个地址时，会异常
  
  /* 命令之间的CS片选至少保持1个时钟周期的高电平 */
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_5_CYCLE;
  
  /*
     MODE0: 表示片选信号空闲期间，CLK时钟信号是低电平
     MODE3: 表示片选信号空闲期间，CLK时钟信号是高电平
  */
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  
  /* QSPI有两个BANK，这里使用的BANK1 */
  hqspi.Init.FlashID = QSPI_FLASH_ID_2;
  
  /* 使用了BANK1，这里是禁止双BANK */
  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
  
  /* 初始化配置QSPI */
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    return 1;
  }
  
  return 0;
}


uint32_t BspQspiBoot_ReadID(void)
{
  QSPI_CommandTypeDef sCommand = {0};
  uint32_t ulID;
  uint8_t ucaBuff[3] = {0};

  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;    
  sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;       
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;  
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;      
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;   
  
  sCommand.Instruction       = BQB_Cmd_ReadID;  
  sCommand.AddressMode       = QSPI_ADDRESS_NONE; 
  sCommand.DataMode          = QSPI_DATA_1_LINE;    
  sCommand.NbData            = 3;
  sCommand.DummyCycles       = 0;                  
  
  if(HAL_QSPI_Command(&hqspi, &sCommand, 10000) != HAL_OK)
  {
    Error_Handler();
  } 
  
  if(HAL_QSPI_Receive(&hqspi, ucaBuff, 10000) != HAL_OK) 
  {
    Error_Handler();
  }

  ulID = (ucaBuff[0] << 16) | (ucaBuff[1] << 8 ) | ucaBuff[2];

  return ulID;
}

static int BspQspiBoot_AutoPollingMemReady(QSPI_HandleTypeDef *p_hqspi)
{
	QSPI_CommandTypeDef     sCommand = {0};
	QSPI_AutoPollingTypeDef sConfig = {0};
  
	/* 基本配置 */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;    /* 1线方式发送指令 */
	sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;       /* 24位地址 */
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;  /* 无交替字节 */
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;      /* W25Q64JV不支持DDR */
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  /* DDR模式，数据输出延迟 */
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;	 /* 每次传输都发指令 */
	
	/* 读取状态*/
	sCommand.Instruction       = BQB_Cmd_ReadStatus1;        /* 读取状态命令 */
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;          /* 无需地址 */
	sCommand.DataMode          = QSPI_DATA_1_LINE;           /* 1线数据 */
	sCommand.DummyCycles       = 0;                          /* 无需空周期 */

	/* 屏蔽位设置的bit0，匹配位等待bit0为0，即不断查询状态寄存器bit0，等待其为0 */
	sConfig.Mask               = 0x01;
	sConfig.Match              = 0x00;
	sConfig.MatchMode          = QSPI_MATCH_MODE_AND;
	sConfig.StatusBytesSize    = 1;
	sConfig.Interval           = 0x10;
	sConfig.AutomaticStop      = QSPI_AUTOMATIC_STOP_ENABLE;

	if (HAL_QSPI_AutoPolling(p_hqspi, &sCommand, &sConfig, 10000) != HAL_OK)
	{
		return 1;
	}

	return 0;
}

static int BspQspiBoot_WriteEnable(QSPI_HandleTypeDef *p_hqspi)
{
  QSPI_CommandTypeDef sCommand = {0};

  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;    
  sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;       
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;  
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;      
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;   
  
  sCommand.Instruction       = BQB_Cmd_WriteEnable;  
  sCommand.AddressMode       = QSPI_ADDRESS_NONE; 
  sCommand.DataMode          = QSPI_DATA_NONE;    
  sCommand.NbData            = 0;
  sCommand.DummyCycles       = 0;                 
  
  if(HAL_QSPI_Command(p_hqspi, &sCommand, 10000) != HAL_OK)
  {
    return 1;
  } 

  return 0;
}

int BspQspiBoot_EraseSector(uint32_t ulSectorAddr)
{
  QSPI_CommandTypeDef sCommand = {0};
  
  BspQspiBoot_WriteEnable(&hqspi);

  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;    
  sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;       
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;  
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;      
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;   
  
  sCommand.Instruction       = BQB_Cmd_SectorErase;  
  sCommand.AddressMode       = QSPI_ADDRESS_1_LINE; 
  sCommand.Address           = ulSectorAddr;
  sCommand.DataMode          = QSPI_DATA_NONE;    
  sCommand.DummyCycles       = 0;                 

  if(HAL_QSPI_Command(&hqspi, &sCommand, 10000) != HAL_OK)
  {
    return 1;
  } 

  if(BspQspiBoot_AutoPollingMemReady(&hqspi) == 1)
  {
    return 1;
  }

  return 0;
}

int BspQspiBoot_EraseChip(void)
{
  QSPI_CommandTypeDef sCommand = {0};
  
  BspQspiBoot_WriteEnable(&hqspi);

  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;    
  sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;       
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;  
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;      
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;   
  
  sCommand.Instruction       = BQB_Cmd_ChipErase;  
  sCommand.AddressMode       = QSPI_ADDRESS_NONE; 
  sCommand.DataMode          = QSPI_DATA_NONE;   
  sCommand.NbData            = 0;
  sCommand.DummyCycles       = 0;                 
  
  if(HAL_QSPI_Command(&hqspi, &sCommand, 10000) != HAL_OK)
  {
    return 1;
  } 

  if(BspQspiBoot_AutoPollingMemReady(&hqspi) == 1)
  {
    return 1;
  }

  return 0;
}

int BspQspiBoot_WritePage(uint8_t *p_Buff,uint32_t ulWriteAddr,uint16_t usWriteSize)
{
  QSPI_CommandTypeDef sCommand = {0};
  
  BspQspiBoot_WriteEnable(&hqspi);

  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;    
  sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;       
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;  
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;      
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  
  sCommand.SIOOMode          = QSPI_SIOO_INST_ONLY_FIRST_CMD;   
  
  sCommand.Instruction       = BQB_Cmd_PageProgram_Quad;  
  sCommand.AddressMode       = QSPI_ADDRESS_1_LINE; 
  sCommand.Address           = ulWriteAddr;
  sCommand.DataMode          = QSPI_DATA_4_LINES;    
  sCommand.NbData            = usWriteSize;
  sCommand.DummyCycles       = 0;                 

  if(HAL_QSPI_Command(&hqspi, &sCommand, 10000) != HAL_OK)
  {
    return 1;
  } 
  
  if (HAL_QSPI_Transmit(&hqspi, p_Buff, 10000) != HAL_OK) 
  {
    return 1;
  }

  if(BspQspiBoot_AutoPollingMemReady(&hqspi) == 1)
  {
    return 1;
  }

  return 0;
}

int BspQspiBoot_ReadBuff(uint8_t *p_Buff,uint32_t ulReadAddr,uint32_t ulReadSize)
{
	QSPI_CommandTypeDef sCommand = {0};
	
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;    	/* 1线方式发送指令 */
	sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;      	/* 24位地址 */
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;  	/* 无交替字节 */
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;      	/* W25Q64JV不支持DDR */
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  	/* DDR模式，数据输出延迟 */
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;		/* 每次传输要发指令 */	
 
	sCommand.Instruction       = BQB_Cmd_FastRead_Quad;       /* 24bit地址的4线快速读取命令 */
	sCommand.AddressMode       = QSPI_ADDRESS_4_LINES;        /* 4线地址 */
	sCommand.Address           = ulReadAddr;                  /* 读取数据的起始地址 */ 
	sCommand.DataMode          = QSPI_DATA_4_LINES;           /* 4线数据 */
	sCommand.NbData            = ulReadSize;                  /* 读取的数据大小 */ 
	sCommand.DummyCycles       = 6;                           /* 空周期 */
	
	if (HAL_QSPI_Command(&hqspi, &sCommand, 10000) != HAL_OK)
	{
		return 1;
	}

	if (HAL_QSPI_Receive(&hqspi, p_Buff, 10000) != HAL_OK)
	{
		return 1;
	}

	return 0;
}

int BspQspiBoot_MemMapped(void)
{
  QSPI_CommandTypeDef s_command = {0};
  QSPI_MemoryMappedTypeDef s_mem_mapped_cfg = {0};

  s_command.InstructionMode          = QSPI_INSTRUCTION_1_LINE;     /* 1线方式发送指令 */ 
  s_command.AddressSize              = QSPI_ADDRESS_24_BITS;        /* 24位地址 */
  s_command.AlternateByteMode        = QSPI_ALTERNATE_BYTES_NONE;   /* 无交替字节 */
  s_command.DdrMode                  = QSPI_DDR_MODE_DISABLE;       /* W25Q64JV不支持DDR */
  s_command.DdrHoldHalfCycle         = QSPI_DDR_HHC_ANALOG_DELAY;   /* DDR模式，数据输出延迟 */
  s_command.SIOOMode                 = QSPI_SIOO_INST_EVERY_CMD;    /* 每次传输都发指令 */

  s_command.Instruction              = BQB_Cmd_FastRead_Quad;       /* 快速读取命令 */
  s_command.AddressMode              = QSPI_ADDRESS_4_LINES;        /* 4个地址线 */
  s_command.DataMode                 = QSPI_DATA_4_LINES;           /* 4个数据线 */
  s_command.DummyCycles              = 6;                           /* 空周期 */
  
  s_mem_mapped_cfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
  s_mem_mapped_cfg.TimeOutPeriod = 0;
  
  if(HAL_QSPI_MemoryMapped(&hqspi, &s_command, &s_mem_mapped_cfg) != HAL_OK)
  {
    return 1;
  }

  return 0;
}

int BspQspiBoot_QuitMemMapped(void)
{
  HAL_QSPI_Abort(&hqspi);

  return 0;
}

uint32_t ulFlashID;
uint8_t ucaTestBuff[256] = {0};

void BspQspiBoot_Test(void)
{
  bool bTestResult = true;
  uint8_t ucTestCnt;
  uint32_t ulTestAddr;
  uint32_t ulSectorAddr = 0x2000;
  
  ulFlashID = BspQspiBoot_ReadID();
  
  if(ulFlashID != 0x00EF4017)
  {
    bTestResult = false;
  }

  while(bTestResult == false);
  
  BspQspiBoot_EraseSector(ulSectorAddr);
  
  for(ucTestCnt=0;ucTestCnt<16;ucTestCnt++)
  {
    ulTestAddr = ulSectorAddr+(ucTestCnt*QSPI_FLASH_PAGE_SIZE);
    
    {
      uint16_t i;
    
      for(i=0;i<sizeof(ucaTestBuff);i++)
      {
        ucaTestBuff[i] = i;
      }
    }
  
    BspQspiBoot_WritePage(ucaTestBuff,ulTestAddr,sizeof(ucaTestBuff));
  
    memset(ucaTestBuff,0,sizeof(ucaTestBuff));
  
    BspQspiBoot_ReadBuff(ucaTestBuff,ulTestAddr,sizeof(ucaTestBuff));
  
    {
      uint16_t i;
    
      for(i=0;i<sizeof(ucaTestBuff);i++)
      {
        if(ucaTestBuff[i] != i)
        {
          bTestResult = false;
          break;
        }
      }
    }
  }

  while(bTestResult == false);

  while(1)
  {
    
  }
}

/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */

void BspQspiBoot_JumpToApp(void)
{
	uint32_t i=0;
	void (*AppJump)(void);         /* 声明一个函数指针 */
	__IO uint32_t AppAddr = 0x90000000;  /* APP 地址 */

    /* 关闭全局中断 */
	DISABLE_INT(); 
    
    /* 设置所有时钟到默认状态，使用HSI时钟 */
	HAL_RCC_DeInit();
    
	/* 关闭滴答定时器，复位到默认值 */
	SysTick->CTRL = 0;
  SysTick->LOAD = 0;
  SysTick->VAL = 0;

	/* 关闭所有中断，清除所有中断挂起标志 */
	for (i = 0; i < 8; i++)
	{
		NVIC->ICER[i]=0xFFFFFFFF;
		NVIC->ICPR[i]=0xFFFFFFFF;
	}	

	/* 使能全局中断 */
	ENABLE_INT();

	/* 跳转到应用程序，首地址是MSP，地址+4是复位中断服务程序地址 */
	AppJump = (void (*)(void)) (*((uint32_t *) (AppAddr + 4)));

	/* 设置主堆栈指针 */
	__set_MSP(*(uint32_t *)AppAddr);
	
	/* 在RTOS工程，这条语句很重要，设置为特权级模式，使用MSP指针 */
	__set_CONTROL(0);

	/* 跳转到系统BootLoader */
	AppJump(); 

	/* 跳转成功的话，不会执行到这里，用户可以在这里添加代码 */
	while (1)
	{

	}
}

