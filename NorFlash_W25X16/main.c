#include"stm32f10x.h"
#include"user_usart1.h"
#include"user_led.h"
#include"user_beep.h"
#include"user_spi_w25x16.h"
#include

#define  FLASH_ID1              0xef3015
#define  FLASH_ID2              0xef4015
#define  FLASH_WriteAddress     0x000000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorErase      FLASH_WriteAddress

#define CountOf(a)          (sizeof(a)/sizeof(*(a)))
#define DataSize    (CountOf(TxDataTable)-1)

u8 TxDataTable[] =
	"\n\rIt is a test.\n\r";
u8 RxDataTable[DataSize];

//=============================================
#ifdef __GNUC__

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
//=============================================

TestStatus User_SPI_DataCompare(u8 * TxData, u8 * RxData, u8 Num)
{
	while (Num--)
	{
		if (*TxData != *RxData)
		{
			return Failed;
		}
		RxData++;
		TxData++;
	}

	return Successed;
}

void User_SPI_Test(void)
{
	vu32 ID, DeviceID;
	u8 Buffer;
	volatile TestStatus Test1 = Failed;
	volatile TestStatus Test2 = Successed;	//存放两次测试的结果

	printf("\n\rReading ID.....");
	User_LedSpark(Led1, 2);
	ID = User_SPI_W25X16_ReadID();
	printf("flashID:%x\n\r", ID);

	printf("\n\rReading DeviceID......");
	User_LedSpark(Led1, 2);
	DeviceID = User_SPI_W25X16_ReadDeviceID();
	printf("flashDeviceID:%x\n\r", DeviceID);

	if ((ID == FLASH_ID1) || (ID == FLASH_ID2))
	{

		printf("\n\rARM erase......");
		User_LedSpark(Led1, 2);
		User_SPI_W25X16_SectorErase(FLASH_SectorErase);
		printf("Done\n\r");

		printf("\n\rdata to be writen: :%s\n\r", TxDataTable);
		printf("\n\rWriting......");
		User_LedSpark(Led1, 2);
		User_SPI_W25X16_ChipWrite(TxDataTable, FLASH_WriteAddress, DataSize);
		printf("Done\n\r");

		printf("\n\rReading from chip......");
		User_LedSpark(Led1, 2);
		User_SPI_W25X16_ChipRead(RxDataTable, FLASH_ReadAddress, DataSize);
		printf("Done\n\r");
		User_LedSpark(Led1, 2);
		printf("\n\rthe data:%s\n\r", RxDataTable);

		printf("\n\rTesting......");
		User_LedSpark(Led1, 2);
		Test1 = User_SPI_DataCompare(RxDataTable, TxDataTable, DataSize);
		if (Test1 == Successed)
		{
			printf("Same\n\r");
		} else
		{
			printf("Error\n\r");
			User_LedSpark(Led2, 2);
			printf
				("Error, please check\n\r");
		}

		printf("\n\rErase......");
		User_LedSpark(Led1, 2);
		User_SPI_W25X16_SectorErase(FLASH_SectorErase);
		printf("Done\n\r");

		printf("\n\rRead......");
		User_LedSpark(Led1, 2);
		User_SPI_W25X16_ChipRead(RxDataTable, FLASH_ReadAddress, DataSize);
		printf("Done\n\r");

		printf("\n\rErase ?......");
		User_LedSpark(Led1, 1);
		for (Buffer = 0; Buffer <100; Buffer++){
			 if (RxDataTable[Buffer] != 0xff) {
			    Test2 = Failed;
            }
		}
		if (Test2 == Failed)  {
			 printf("Falure\n\r");
        } 
        else  {
			printf("earase OK\n\r");}
			printf("\n\r------------Result------------\n\r");
			if ((Test1 == Successed) && ((Test2 == Successed))) {
			    User_LedSpark(Led1, 2);
			    printf("\n\rTest sucesse\n\r");
            } else  {
			 User_LedSpark(Led2, 2);
			 printf("\n\rSPI Failure\n\r");
            }
		} else {
			 User_LedSpark(Led2, 2);
			 printf("\n\rID reading is wrong\n\r");
        }
}

int main(void) {
	User_USART1Config();
	printf("\n\r串口1配置......");
	printf("Done\n\r");
	printf("\n\r蜂鸣器初始化...");
	User_BeepConfig();
	printf("\n\r蜂鸣器测试......");
    User_BeepStatus(BeepStatus_TurnOn);
    printf("Done\n\r");
    printf("\n\rLED初始化...");
    User_LedConfig();
    printf("\n\rLED测试......");
    User_LedSpark(Led0, 2);
    printf("Done\n\r");
    printf("\n\rSPI初始化...");
    User_SPI_Config();
    User_LedSpark(Led0, 2);
    printf("Done\n\r"); User_SPI_Test(); while (1);
}

//==================================================
PUTCHAR_PROTOTYPE {
    USART_SendData(USART1, (uint8_t) ch);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {
	
    }
    return ch;}
//==================================================
