#ifndef _LED_H
#define _LED_H

#include"stm32f10x.h"


#define Debug

#define Led1_RCC    RCC_APB2Periph_GPIOF
#define Led1_Port    GPIOF
#define Led1_Pin    GPIO_Pin_7
#define Led1_On     GPIO_ResetBits(Led1_Port,Led1_Pin)
#define Led1_Off    GPIO_SetBits(Led1_Port,Led1_Pin)

#define Led2_RCC    RCC_APB2Periph_GPIOF
#define Led2_Port    GPIOF
#define Led2_Pin    GPIO_Pin_8
#define Led2_On     GPIO_ResetBits(Led2_Port,Led2_Pin)
#define Led2_Off    GPIO_SetBits(Led2_Port,Led2_Pin)

#define Led3_RCC    RCC_APB2Periph_GPIOF
#define Led3_Port    GPIOF
#define Led3_Pin    GPIO_Pin_9
#define Led3_On     GPIO_ResetBits(Led3_Port,Led3_Pin)
#define Led3_Off    GPIO_SetBits(Led3_Port,Led3_Pin)

#define Led4_RCC    RCC_APB2Periph_GPIOF
#define Led4_Port    GPIOF
#define Led4_Pin    GPIO_Pin_10
#define Led4_On     GPIO_ResetBits(Led4_Port,Led4_Pin)
#define Led4_Off    GPIO_SetBits(Led4_Port,Led4_Pin)


#define LedAll_On    GPIO_ResetBits(Led1_Port,Led1_Pin|Led2_Pin|Led3_Pin|Led4_Pin)
#define LedAll_Off    GPIO_SetBits(Led1_Port,Led1_Pin|Led2_Pin|Led3_Pin|Led4_Pin)

typedef enum{LedAll=0,Led1=1,Led2=2,Led3=3,Led4=4}LedOrder;
typedef enum{LedOff=0,LedOn=!LedOff}LedState;


void Led_Init(void);    

void Led_Spark(LedOrder Led_Order,u8 TurnOn_Time,LedState Led_State); //选中LED 点亮 闪烁次数

#endif