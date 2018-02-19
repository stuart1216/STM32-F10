#include"stm32f10x.h"
#include"led.h"

static void Delay(vu32 Time)
{
	for (; Time != 0; Time--) ;
}

void Led_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(Led1_RCC, ENABLE);	//all led use GPIOF

#ifdef Degub					//在led.h中定义
	GPIO_DeInit(Led1_Port);		//GPIO外设设置为缺省值
	GPIO_StructInit(&GPIO_InitStructure);	//将GPIO_InitStructure按缺省值填入
	GPIO_PinLockConfig(Led1_Port, Led1_Pin);	//锁定LED1使用的IO引脚
#endif

#if 1

	GPIO_InitStructure.GPIO_Pin = Led1_Pin | Led2_Pin | Led3_Pin | Led4_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Led1_Port, &GPIO_InitStructure);	//all led use GPIOF
	LedAll_On;
	Delay(0x6fffff);
	LedAll_Off;
#endif

#if 0
	GPIO_InitStructure.GPIO_Pin = Led1_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Led1_Port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = Led2_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Led2_Port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = Led3_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Led3_Port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = Led4_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Led4_Port, &GPIO_InitStructure);
	LedAll_On;
	Delay(0x6fffff);
	LedAll_Off;
#endif
}

void Led_Spark(LedOrder Led_Order, u8 TurnOn_Time, LedState Led_State)
{
	u8 i;
	switch (Led_Order)
	{
	case LedAll:				//选中所有LED
		if (Led_State == LedOn){	//点亮
		
			for (i = 0; i < 100; i++) {
				 LedAll_On; 
                 Delay(0x5fffff); 
                 LedAll_Off;}
				 } else if (Led_State == LedOff)	//熄灭
				 {
				 LedAll_Off;}
	    break; 
    case Led1:			//选中LED1
		if (Led_State == LedOn){	//点亮
			for (i = 0; i < 100; i++){
				Led1_On; 
                Delay(0x5fffff); 
                Led1_Off;}
			} else if (Led_State == LedOff)	 {//熄灭
				Led1_Off;
            }
	    break; 
    case Led2:			//选中LED2
		if (Led_State == LedOn)	 {//点亮
			for (i = 0; i < 100; i++){
				Led2_On; 
                Delay(0x5fffff); 
                Led2_Off;}
			} else if (Led_State == LedOff)	{//熄灭
				Led2_Off;
            }
	    break; 
    case Led3:			//选中LED3
		if (Led_State == LedOn){	//点亮
			for (i = 0; i < 100; i++){
					Led3_On; 
                    Delay(0x5fffff); 
                    Led3_Off;}
			} else if (Led_State == LedOff)	{//熄灭
				Led3_Off;
            }
	    break; 
    case Led4:			//选中LED4
		if (Led_State == LedOn)	{//点亮
			for (i = 0; i < 100; i++){
				Led4_On; Delay(0x5fffff); Led4_Off;}
		} else if (Led_State == LedOff)	{//熄灭
			Led4_Off;
        }
	    break; 
    default:
		break;}
}
