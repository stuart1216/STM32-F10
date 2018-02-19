#ifndef _USER_SPI_W25X16_H
#define _USER_SPI_W25X16_H

#include"stm32f10x.h"

typedef enum { Failed = 0, Successed = !Failed } TestStatus;	//声明定义枚举型变量 用于表示测试失败与成功

#define SPI_W25X16_SectorSize      4096
#define SPI_W25X16_PageSize       256	//W25X16每页数据长度
#define SPI_W25X16_PerPageWriteSize     256	//每页最多写入的数据个数

#define SPI_W25X16         SPI1
#define SPI_W25X16_Clock       RCC_APB2Periph_SPI1

#define SPI_W25X16_CS_Clock       RCC_APB2Periph_GPIOC
#define SPI_W25X16_CS_Port       GPIOC
#define SPI_W25X16_CS_Pin       GPIO_Pin_4

#define SPI_W25X16_SCK_Clock      RCC_APB2Periph_GPIOA
#define SPI_W25X16_SCK_Port       GPIOA
#define SPI_W25X16_SCK_Pin       GPIO_Pin_5

#define SPI_W25X16_MISO_Clock         RCC_APB2Periph_GPIOA
#define SPI_W25X16_MISO_Port      GPIOA
#define SPI_W25X16_MISO_Pin       GPIO_Pin_6

#define SPI_W25X16_MOSI_Clock      RCC_APB2Periph_GPIOA
#define SPI_W25X16_MOSI_Port      GPIOA
#define SPI_W25X16_MOSI_Pin       GPIO_Pin_7

#define SPI_24G_CS_Clock       RCC_APB2Periph_GPIOB
#define SPI_24G_CS_Port        GPIOB
#define SPI_24G_CS_Pin        GPIO_Pin_2

#define SPI_VS1003B_CS_Clock      RCC_APB2Periph_GPIOB
#define SPI_VS1003B_CS_Port       GPIOB
#define SPI_VS1003B_CS_Pin       GPIO_Pin_0

#define SPI_W25X16_CS_Select      GPIO_ResetBits(SPI_W25X16_CS_Port,SPI_W25X16_CS_Pin)

#define SPI_W25X16_CS_DisSelect      GPIO_SetBits(SPI_W25X16_CS_Port,SPI_W25X16_CS_Pin)

u8 User_SPI_W25X16_SendByte(u8 SendByteData);	//send data that is byte
u8 User_SPI_W25X16_ReadByte(void);	//Read ByteData from chip W25X16
vu16 User_SPI_W25X16_SendHalfWord(u16 HalfWord);	//send data ,is halfword
void User_SPI_W25X16_WriteEnable(void);	//write enable for W25X16
void User_SPI_W25X16_WaitForWriteEnd(void);	//wait the end about write for chip W25X16

void User_SPI_Config(void);		//SPI1 init configuration
void User_SPI_W25X16_SectorErase(vu32 SectorAddress);
void User_SPI_W25X16_BulkErase(void);	//erase the W25X16
void User_SPI_W25X16_PageWrite(u8 * DataTable, vu32 WriteAddress,
							   vu16 NumberOfWrite);
void User_SPI_W25X16_ChipWrite(u8 * DataTable, vu32 WriteAddress,
							   vu16 NumberOfWrite);
void User_SPI_W25X16_ChipRead(u8 * DataTable, vu32 ReadAddress,
							  vu16 NumberOfRead);
vu32 User_SPI_W25X16_ReadID(void);	//read chip ID
vu32 User_SPI_W25X16_ReadDeviceID(void);	//read manufacture device ID
void User_SPI_W25X16_StartReadSequence(vu32 ReadAddress);
void User_SPI_W25X16_PowerDown(void);
void User_SPI_W25X16_WakeUp(void);

#endif
