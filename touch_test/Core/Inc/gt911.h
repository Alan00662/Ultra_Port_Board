#ifndef __GT911_H
#define __GT911_H

#ifdef __cplusplus
extern "C"
{
#endif
	
#include "stm32h7xx.h"



#define GT_CMD_WR    	0XBA     //写命令
#define GT_CMD_RD     	0XBB     //读命令
  
#define GT_CTRL_REG 	0X8040   	//GT911控制寄存器
#define GT_CFGS_REG 	0X8047   	//GT911配置起始地址寄存器
#define GT_CHECK_REG 	0X80FF   	//GT911校验和寄存器
#define GT_PID_REG 		0X8140   	//GT911产品ID寄存器
#define GT_GSTID_REG 	0X814E   	//GT911当前检测到的触摸情况
#define GT_TP1_REG 		0X8150  	//第一个触摸点数据地址
#define GT_TP2_REG 		0X8158		//第二个触摸点数据地址
#define GT_TP3_REG 		0X8160		//第三个触摸点数据地址
#define GT_TP4_REG 		0X8168		//第四个触摸点数据地址
#define GT_TP5_REG 		0X8170		//第五个触摸点数据地址
	
typedef struct
{
  uint16_t TouchDetected;
  uint16_t X;
  uint16_t Y;
} TS_StateTypeDef;

void gt911_init(void);
void gt911_get_state(TS_StateTypeDef* state);

#ifdef __cplusplus
}
#endif

#endif

