#include"stm32f10x.h"
#include"user_usart1.h"
#include

void User_USART1Config(void)
{

	USART_InitTypeDef USART_InitStructure;

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//使能时钟

	USART_InitStructure.USART_BaudRate = 115200;	//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//8位字长
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//1位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;	//无奇偶效验位
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//发送接收模式
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件流控

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);	//使能GPIO与复用功能时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	//USART1 TX使用引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推免式输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50MHZ输出速度

	GPIO_Init(GPIOA, &GPIO_InitStructure);	//发送端口初始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//USART1 RX使用引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
	//GPIO_InitStructure.GPIO_Speed      =GPIO_Speed_50MHz;     //输入一般不需要配置速度

	GPIO_Init(GPIOA, &GPIO_InitStructure);	//接收端初始化

	USART_Init(USART1, &USART_InitStructure);

	USART_Cmd(USART1, ENABLE);

	printf("\n\r串口1初始化完成啦");
}
