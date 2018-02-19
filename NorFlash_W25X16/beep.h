#ifndef _BEEP_H
#define _BEEP_H

#include"stm32f10x.h"

#define Beep_RCC    RCC_APB2Periph_GPIOF
#define Beep_Port    GPIOF
#define Beep_Pin    GPIO_Pin_6

#define Beep_On     GPIO_ResetBits(Beep_Port,Beep_Pin)
#define Beep_Off    GPIO_SetBits(Beep_Port,Beep_Pin)

typedef enum{BeepOff=0,BeepOn=!BeepOff}BeepState;


void Beep_Init(void);    

void Beep_State(vu32 TurnOn_Time,BeepState Beep_State); //选中Beep发声还是无声

#endif