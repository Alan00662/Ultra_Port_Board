
#include "bsp.h"




/*
*********************************************************************************************************
*	�� �� ��: HAL_InitTick
*	����˵��: �ض��򣬲�ʹ��
*	��    ��: TickPriority
*	�� �� ֵ: ��
*********************************************************************************************************
*/
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
	return HAL_OK;
}


int SystemClock_Config(void)
{
RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	HAL_StatusTypeDef ret = HAL_OK;

	/* ��סSCU(Supply configuration update) */
	MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);

	/* 
      1��оƬ�ڲ���LDO��ѹ������ĵ�ѹ��Χ����ѡVOS1��VOS2��VOS3����ͬ��Χ��Ӧ��ͬ��Flash���ٶȣ�
         ���鿴�ο��ֲ��Table 12�ı��
      2������ѡ��ʹ��VOS1����ѹ��Χ1.15V - 1.26V��
    */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

	/* ʹ��HSE����ѡ��HSE��ΪPLLʱ��Դ */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
	RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
		
	RCC_OscInitStruct.PLL.PLLM = 5;
	RCC_OscInitStruct.PLL.PLLN = 192;
	RCC_OscInitStruct.PLL.PLLP = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLQ = 2;		
		
	RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
	RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;	
	ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
	if(ret != HAL_OK)
	{
        return 1;        
	}

	/* 
       ѡ��PLL�������Ϊϵͳʱ��
       ����RCC_CLOCKTYPE_SYSCLKϵͳʱ��
       ����RCC_CLOCKTYPE_HCLK ʱ�ӣ���ӦAHB1��AHB2��AHB3��AHB4����
       ����RCC_CLOCKTYPE_PCLK1ʱ�ӣ���ӦAPB1����
       ����RCC_CLOCKTYPE_PCLK2ʱ�ӣ���ӦAPB2����
       ����RCC_CLOCKTYPE_D1PCLK1ʱ�ӣ���ӦAPB3����
       ����RCC_CLOCKTYPE_D3PCLK1ʱ�ӣ���ӦAPB4����     
    */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
								 RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;  
	RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2; 
	RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2; 
	RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2; 
	
	/* �˺��������SystemCoreClock������������HAL_InitTick */
	ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
	if(ret != HAL_OK)
	{
     return 1;
	}

    /*
      ʹ��IO�ĸ���ģʽ��Ҫʹ��IO������������������������ 
      ��1��ʹ��CSI clock
      ��2��ʹ��SYSCFG clock
      ��3��ʹ��I/O������Ԫ�� ����SYSCFG_CCCSR�Ĵ�����bit0
    */
	  __HAL_RCC_CSI_ENABLE() ;
	  __HAL_RCC_SYSCFG_CLK_ENABLE() ;
	  HAL_EnableCompensationCell();

    return 0;
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
