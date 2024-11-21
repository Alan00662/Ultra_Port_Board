/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ltdc.c
  * @brief   This file provides code for the configuration
  *          of the LTDC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "ltdc.h"

uint16_t lcd_framebuf[320][480] = {0}; //直接用内部RAM
uint16_t *g_ltdc_framebuf[2];
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

LTDC_HandleTypeDef hltdc;

/* LTDC init function */
void MX_LTDC_Init(void)
{

  /* USER CODE BEGIN LTDC_Init 0 */

  /* USER CODE END LTDC_Init 0 */

  LTDC_LayerCfgTypeDef pLayerCfg = {0};

  /* USER CODE BEGIN LTDC_Init 1 */

  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IIPC;
  hltdc.Init.HorizontalSync = 1;
  hltdc.Init.VerticalSync = 1;
  hltdc.Init.AccumulatedHBP = 4;
  hltdc.Init.AccumulatedVBP = 4;
  hltdc.Init.AccumulatedActiveW = 324;
  hltdc.Init.AccumulatedActiveH = 484;
  hltdc.Init.TotalWidth = 339;
  hltdc.Init.TotalHeigh = 499;
  hltdc.Init.Backcolor.Blue = 255;
  hltdc.Init.Backcolor.Green = 255;
  hltdc.Init.Backcolor.Red = 255;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  
  g_ltdc_framebuf[0] = (uint16_t *)lcd_framebuf;
  
//  pLayerCfg.WindowX0 = 0;
//  pLayerCfg.WindowX1 = 320;
//  pLayerCfg.WindowY0 = 0;
//  pLayerCfg.WindowY1 = 480;
//  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
//  pLayerCfg.Alpha = 255;
//  pLayerCfg.Alpha0 = 0;
//  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
//  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
//  pLayerCfg.FBStartAdress =  (uint32_t)g_ltdc_framebuf[0];
//  pLayerCfg.ImageWidth = 320;
//  pLayerCfg.ImageHeight = 480;
//  pLayerCfg.Backcolor.Blue = 255;
//  pLayerCfg.Backcolor.Green = 0;
//  pLayerCfg.Backcolor.Red = 0;
//  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN LTDC_Init 2 */
//    HAL_LTDC_SetWindowPosition(&hltdc, 0, 0, 0);
//    HAL_LTDC_SetWindowSize(&hltdc, 320, 480, 0);
  /* USER CODE END LTDC_Init 2 */

}

void HAL_LTDC_MspInit(LTDC_HandleTypeDef* ltdcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(ltdcHandle->Instance==LTDC)
  {
  /* USER CODE BEGIN LTDC_MspInit 0 */

  /* USER CODE END LTDC_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLL3.PLL3M = 1;
    PeriphClkInitStruct.PLL3.PLL3N = 6;
    PeriphClkInitStruct.PLL3.PLL3P = 2;
    PeriphClkInitStruct.PLL3.PLL3Q = 2;
    PeriphClkInitStruct.PLL3.PLL3R = 15;
    PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_3;
    PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOMEDIUM;
    PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* LTDC clock enable */
    __HAL_RCC_LTDC_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**LTDC GPIO Configuration
    PC0     ------> LTDC_R5
    PA3     ------> LTDC_B5
    PA4     ------> LTDC_VSYNC
    PA5     ------> LTDC_R4
    PA6     ------> LTDC_G2
    PB0     ------> LTDC_R3
    PB1     ------> LTDC_R6
    PE11     ------> LTDC_G3
    PE12     ------> LTDC_B4
    PE13     ------> LTDC_DE
    PE14     ------> LTDC_CLK
    PE15     ------> LTDC_R7
    PB10     ------> LTDC_G4
    PB11     ------> LTDC_G5
    PD10     ------> LTDC_B3
    PC6     ------> LTDC_HSYNC
    PC7     ------> LTDC_G6
    PD3     ------> LTDC_G7
    PB8     ------> LTDC_B6
    PB9     ------> LTDC_B7
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_LTDC;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* LTDC interrupt Init */
    HAL_NVIC_SetPriority(LTDC_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(LTDC_IRQn);
  /* USER CODE BEGIN LTDC_MspInit 1 */

  /* USER CODE END LTDC_MspInit 1 */
  }
}

void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef* ltdcHandle)
{

  if(ltdcHandle->Instance==LTDC)
  {
  /* USER CODE BEGIN LTDC_MspDeInit 0 */

  /* USER CODE END LTDC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_LTDC_CLK_DISABLE();

    /**LTDC GPIO Configuration
    PC0     ------> LTDC_R5
    PA3     ------> LTDC_B5
    PA4     ------> LTDC_VSYNC
    PA5     ------> LTDC_R4
    PA6     ------> LTDC_G2
    PB0     ------> LTDC_R3
    PB1     ------> LTDC_R6
    PE11     ------> LTDC_G3
    PE12     ------> LTDC_B4
    PE13     ------> LTDC_DE
    PE14     ------> LTDC_CLK
    PE15     ------> LTDC_R7
    PB10     ------> LTDC_G4
    PB11     ------> LTDC_G5
    PD10     ------> LTDC_B3
    PC6     ------> LTDC_HSYNC
    PC7     ------> LTDC_G6
    PD3     ------> LTDC_G7
    PB8     ------> LTDC_B6
    PB9     ------> LTDC_B7
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0|GPIO_PIN_6|GPIO_PIN_7);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_8|GPIO_PIN_9);

    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_10|GPIO_PIN_3);

    /* LTDC interrupt Deinit */
    HAL_NVIC_DisableIRQ(LTDC_IRQn);
  /* USER CODE BEGIN LTDC_MspDeInit 1 */

  /* USER CODE END LTDC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void ltdc_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color)
{

    uint32_t timeout = 0;
    uint16_t offline =0 ; 
    uint32_t addr;
	

#if 1
	addr = ((uint32_t)g_ltdc_framebuf[0] + 2 * (320 *sy  + sx)) ;
	offline = 480 - (ex - sx + 1); //行偏移
#else
	addr = ((uint32_t)g_ltdc_framebuf[0] + 2 * (320 * sy + sx));
	offline = 320 - (ex - sx + 1);
#endif



    /* 配置DMA2D */
    DMA2D->CR &= ~(DMA2D_CR_START); //停止DMA2D
    DMA2D->CR = DMA2D_R2M; //寄存器到内存
    DMA2D->OPFCCR = LTDC_PIXEL_FORMAT_RGB565;
    DMA2D->OOR = offline;
    DMA2D->OMAR = addr;
    DMA2D->NLR = (ey - sy + 1) | ((ex - sx + 1) << 16); //像素数量，高16位是行数
//    DMA2D->NLR = (ex - sx + 1) | ((ey - sy + 1) << 16); //像素数量，高16位是行数
    DMA2D->OCOLR = color;
    
    /* 启动DMA2D */
    DMA2D->CR |= DMA2D_CR_START;
    
    /* 等待DMA2D传输结束 */
    while ((DMA2D->ISR & (DMA2D_FLAG_TC)) == 0)
    {
        if (++timeout > 0x1FFFFF)
        {
            break;
        }
    } 
    
    /* 清除DMA2D传输完成标志 */
    DMA2D->IFCR |= DMA2D_FLAG_TC;
}


/**
 * @brief   LTDC清屏
 * @param   color: 清屏的颜色
 * @retval  无
 */
void ltdc_clear(uint32_t color)
{
	#if 1 
    ltdc_fill(0, 0, 320, 480, color);
	#else
	ltdc_fill(0, 0, lcdltdc.width - 1, lcdltdc.height - 1, color);
	#endif
}
/* USER CODE END 1 */
