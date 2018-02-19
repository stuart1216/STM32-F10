#include"stm32f10x.h"
#include"beep.h"

static void Delay(vu32 Time)
{
	for (; Time != 0; Time--) ;
}

void Beep_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(Beep_RCC, ENABLE);	//Beep使用的GPIO时钟

	GPIO_InitStructure.GPIO_Pin = Beep_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(Beep_Port, &GPIO_InitStructure);

	Beep_On;
	Delay(0x5fffff);
	Beep_Off;
}

void Beep_State(vu32 TurnOn_Time, BeepState Beep_State)
{
	vu32 Time = 0x5fffff;
	switch (Beep_State)
	{
	case BeepOff:
		Beep_Off;
		break;

	case BeepOn:
		Beep_On;
		Delay(TurnOn_Time * Time);
		Beep_Off;
		break;

	default:
		break;
	}
}
