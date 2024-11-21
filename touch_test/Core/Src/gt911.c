#include "gt911.h"
#include <stdio.h>
#include "string.h" 
#include "main.h"

extern I2C_HandleTypeDef hi2c4;
#define TP_I2C hi2c4
//INT引脚浮空
static void set_float(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = TP_INT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(TP_INT_GPIO_Port, &GPIO_InitStruct);
}

//写寄存器
static void GT911_WR_Reg_One_byte(uint16_t reg,uint8_t data)
{
	uint8_t temp[3]={0};
	temp[0]=reg>>8;
	temp[1]=reg&0xff;
	temp[2]=data;
	if(HAL_I2C_Master_Transmit(&TP_I2C,GT_CMD_WR,temp,3,0xff)!=HAL_OK)
	{
//		printf("write reg one byte error\r\n");
	}
}

//从GT911读出一次数据		  
static void GT911_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	uint8_t temp[2]={0};
	temp[1]=reg&0xff;
	temp[0]=reg>>8;
	if(HAL_I2C_Master_Transmit(&TP_I2C,GT_CMD_WR,temp,2,0xff)!=HAL_OK)
	{
//		printf("read reg error\r\n");
	}
	if(HAL_I2C_Master_Receive(&TP_I2C,GT_CMD_RD,buf,len,0xff)!=HAL_OK)
	{
//		printf("read reg error\r\n");
	}
}

//初始化
void gt911_init(void)
{
	HAL_Delay(200); 
	uint8_t temp[5]={0};
	HAL_GPIO_WritePin(TP_RST_GPIO_Port,TP_RST_Pin,GPIO_PIN_RESET);				//复位
	HAL_Delay(10); 
	HAL_GPIO_WritePin(TP_INT_GPIO_Port,TP_INT_Pin,GPIO_PIN_RESET);	
	HAL_Delay(1);
 	HAL_GPIO_WritePin(TP_RST_GPIO_Port,TP_RST_Pin,GPIO_PIN_SET);				//释放复位		    
	HAL_Delay(60); 
	set_float();																													//INT浮空
	temp[4]=0;
	GT911_RD_Reg(GT_PID_REG,temp,4);
//	printf("CTP ID:%s\r\n",temp);
	temp[0]=0X02;			
	GT911_WR_Reg_One_byte(GT_CTRL_REG,temp[0]);//软复位GT911
	GT911_RD_Reg(GT_CFGS_REG,temp,1);//读取GT_CFGS_REG寄存器
//	printf("TP firmware version: %d\r\n",temp[0]);
	
}

//读取数据
void gt911_get_state(TS_StateTypeDef* state)
{
	static uint8_t counts=0;
	uint8_t flag=0;
	uint8_t buf[4]={0};
	
	GT911_RD_Reg(GT_GSTID_REG,&flag,1);
	if(flag&0x80)																		//数据是否准备好
	{
		//清标志
		GT911_WR_Reg_One_byte(GT_GSTID_REG,0);
		if(flag&0xf)																	//有数据点
		{
			//只需读一个点
			GT911_RD_Reg(GT_TP1_REG,buf,4);
			state->X=(uint16_t)((buf[1]<<8)|buf[0]);
			state->Y=(uint16_t)((buf[3]<<8)|buf[2]);
			state->TouchDetected=1;
			GT911_WR_Reg_One_byte(GT_GSTID_REG,0);
		}
		else
			state->TouchDetected=0;
	}
	else
		state->TouchDetected=0;
	
	counts++;
}

