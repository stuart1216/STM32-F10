#include"stm32f10x.h"
#include"user_spi_w25x16.h"
#include

#define WriteEnable    0x06
#define WriteDisable   0x04
#define ReadStatusRegister  0x05
#define WriteStatusRegister  0x01
#define ReadData    0x03
#define FastRead    0x0b
#define FsatReadDualOutput  0x3b
#define PageProgram    0x02
#define BlukErase    0xd8
#define SectorErase    0x20
#define ChipErase    0xc7
#define PowerDown    0xb9
#define WakeUp     0xab
#define DeviceID    0xab
#define ManufatureID   0x90
#define JedecID     0x9f

#define JudgeCode    0x01		//用于判断通讯是否结束用
#define NoneCode    0xa5		//无意义的指令，用于:接收数据时，发送这个质量来产生接收时候的时钟

void User_SPI_Config(void)
{

	SPI_InitTypeDef SPI_InitStructure;

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(SPI_24G_CS_Clock, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SPI_24G_CS_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_24G_CS_Port, &GPIO_InitStructure);
	GPIO_SetBits(SPI_24G_CS_Port, SPI_24G_CS_Pin);

	RCC_APB2PeriphClockCmd(SPI_VS1003B_CS_Clock, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SPI_VS1003B_CS_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_VS1003B_CS_Port, &GPIO_InitStructure);
	GPIO_SetBits(SPI_VS1003B_CS_Port, SPI_VS1003B_CS_Pin);

	RCC_APB2PeriphClockCmd(SPI_W25X16_Clock, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	RCC_APB2PeriphClockCmd(SPI_W25X16_CS_Clock, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SPI_W25X16_CS_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_W25X16_CS_Port, &GPIO_InitStructure);
	SPI_W25X16_CS_DisSelect;

	RCC_APB2PeriphClockCmd(SPI_W25X16_SCK_Clock, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SPI_W25X16_SCK_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_W25X16_SCK_Port, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(SPI_W25X16_MISO_Clock, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SPI_W25X16_MISO_Pin;
	GPIO_Init(SPI_W25X16_MISO_Port, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(SPI_W25X16_MOSI_Clock, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SPI_W25X16_MOSI_Pin;
	GPIO_Init(SPI_W25X16_MOSI_Port, &GPIO_InitStructure);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//通讯模式:双向全双工模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	//主从:主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	//数据帧长度:8bits
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;	//空闲时置高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//第二个时钟采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	//NSS控制选择:软件控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;	//波特率分配系数:2分频
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//数据帧格式:MSB在前
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC效验多项式

	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, ENABLE);
}

u8 User_SPI_W25X16_SendByte(u8 SendByteData)
{
	u8 ReceiveData;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET) ;

	SPI_I2S_SendData(SPI1, SendByteData);

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET) ;

	ReceiveData = SPI_I2S_ReceiveData(SPI1);

	return ReceiveData;
}

u8 User_SPI_W25X16_ReadByte(void)
{
	u8 ReceiveData;

	ReceiveData = User_SPI_W25X16_SendByte(NoneCode);

	return ReceiveData;
}

vu16 User_SPI_W25X16_SendHalfWord(u16 HalfWord)
{
	vu16 ReceiveData;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET) ;

	SPI_I2S_SendData(SPI1, HalfWord);

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET) ;

	ReceiveData = SPI_I2S_ReceiveData(SPI1);

	return ReceiveData;
}

void User_SPI_W25X16_WriteEnable(void)
{

	SPI_W25X16_CS_Select;

	User_SPI_W25X16_SendByte(WriteEnable);

	SPI_W25X16_CS_DisSelect;
}

void User_SPI_W25X16_WaitForWriteEnd(void)
{
	u8 ReceiveStatus;			//用于存放W25X16返回的状态 非零就是操作结束

	SPI_W25X16_CS_Select;

	User_SPI_W25X16_SendByte(ReadStatusRegister);

	do
	{
		ReceiveStatus = User_SPI_W25X16_SendByte(NoneCode);
	}
	while (ReceiveStatus & JudgeCode == SET);

	SPI_W25X16_CS_DisSelect;
}

void User_SPI_W25X16_SectorErase(vu32 SectorAddress)
{

	User_SPI_W25X16_WriteEnable();

	SPI_W25X16_CS_Select;

	User_SPI_W25X16_SendByte(SectorErase);

	User_SPI_W25X16_SendByte((SectorAddress & 0xff0000) >> 16);	//发送最高8位
	User_SPI_W25X16_SendByte((SectorAddress & 0xff00) >> 8);	// 中间8位
	User_SPI_W25X16_SendByte(SectorAddress & 0xff);	//发送最低8位

	SPI_W25X16_CS_DisSelect;

	User_SPI_W25X16_WaitForWriteEnd();
}

void User_SPI_W25X16_BulkErase(void)
{

	User_SPI_W25X16_WriteEnable();

	SPI_W25X16_CS_Select;

	User_SPI_W25X16_SendByte(ChipErase);

	SPI_W25X16_CS_DisSelect;

	User_SPI_W25X16_WaitForWriteEnd();
}

void User_SPI_W25X16_PageWrite(u8 * DataTable, vu32 WriteAddress,
							   vu16 NumberOfWrite)
{

	User_SPI_W25X16_WriteEnable();

	SPI_W25X16_CS_Select;

	User_SPI_W25X16_SendByte(PageProgram);

	User_SPI_W25X16_SendByte((WriteAddress & 0xff0000) >> 16);	//最高8位地址
	User_SPI_W25X16_SendByte((WriteAddress & 0xff00) >> 8);	//中间8位地址
	User_SPI_W25X16_SendByte(WriteAddress & 0xff);	//最低8位地址

	if (NumberOfWrite > SPI_W25X16_PerPageWriteSize)	//W25X16采用的是页写入方式，最多一次性写入256个数据，然后内部地址指针归零
	{
		NumberOfWrite = SPI_W25X16_PerPageWriteSize;
		printf
			("\n\r哦偶,一次性写入的数据太多,不能超过256的啦,ARM将为你写入前256个数据\n\r");
	}

	while (NumberOfWrite--)
	{
		User_SPI_W25X16_SendByte(*DataTable);
		DataTable++;			//数组指针 +1
	}

	SPI_W25X16_CS_DisSelect;

	User_SPI_W25X16_WaitForWriteEnd();
}

void User_SPI_W25X16_ChipWrite(u8 * DataTable, vu32 WriteAddress,
							   vu16 NumberOfWrite)
{
	u8 AddressRemainder = 0;
	u8 NumberOfPage = 0;
	u8 Count = 0;				//存放地址所在页需要写入的最多数据
	u8 NumberOfSingle = 0;		//写入数据的最后一些需要写入的数据个数
	u8 Buffer = 0;				//保留

	AddressRemainder = WriteAddress % SPI_W25X16_PageSize;
	Count = SPI_W25X16_PageSize - AddressRemainder;
	NumberOfPage = NumberOfWrite / SPI_W25X16_PageSize;
	NumberOfSingle = NumberOfWrite % SPI_W25X16_PageSize;

	if (AddressRemainder == 0)
	{

		if (NumberOfPage == 0)	//NumberOfWrite < SPI_W25X16_PageSize
		{
			User_SPI_W25X16_PageWrite(DataTable, WriteAddress, NumberOfWrite);
		}

		else
		{

			while (NumberOfPage--)
			{
				User_SPI_W25X16_PageWrite(DataTable, WriteAddress, SPI_W25X16_PageSize);	//一次性写入256个
				DataTable += SPI_W25X16_PageSize;	//接着写下一个256数据
				WriteAddress += SPI_W25X16_PageSize;	//地址就移到下一页(256为单位)
			}

			User_SPI_W25X16_PageWrite(DataTable, WriteAddress, NumberOfSingle);
		}
	}

	else
	{

		if (NumberOfPage == 0)
		{

			if (NumberOfWrite < Count)
			{
				User_SPI_W25X16_PageWrite(DataTable, WriteAddress,
										  NumberOfWrite);
			}

			else
			{

				User_SPI_W25X16_PageWrite(DataTable, WriteAddress, Count);	//起始地址所在页只需要写入Count个
				Buffer = NumberOfWrite - Count;	//计算出下一页要写入的数据数量
				DataTable += Count;
				WriteAddress += Count;

				User_SPI_W25X16_PageWrite(DataTable, WriteAddress, Buffer);
			}
		}

		else
		{

			User_SPI_W25X16_PageWrite(DataTable, WriteAddress, Count);

			//之后需要重新计算以下参数  原因:数据量超过一页分两种情况:1、数据量不会覆盖完起始地址下一页；2、数据量会完全覆盖起始地址下一页
			//重新计算就是看是否会覆盖掉下一页，如果会就进入while()循环,否则就不进入
			DataTable += Count;
			WriteAddress += Count;
			NumberOfWrite -= Count;
			NumberOfPage = NumberOfWrite / SPI_W25X16_PageSize;
			NumberOfSingle = NumberOfWrite % SPI_W25X16_PageSize;

			while (NumberOfPage--)
			{
				User_SPI_W25X16_PageWrite(DataTable, WriteAddress,
										  SPI_W25X16_PageSize);
				DataTable += SPI_W25X16_PageSize;
				WriteAddress += SPI_W25X16_PageSize;
			}

			if (NumberOfSingle != 0)
			{
				User_SPI_W25X16_PageWrite(DataTable, WriteAddress,
										  NumberOfSingle);
			}
		}
	}
}

void User_SPI_W25X16_ChipRead(u8 * DataTable, vu32 ReadAddress,
							  vu16 NumberOfRead)
{
	//虽然要发送地址，但是不需要说明有个写操作(I2C才是这样的时序)
	SPI_W25X16_CS_Select;

	User_SPI_W25X16_SendByte(ReadData);

	User_SPI_W25X16_SendByte((ReadAddress & 0xff0000) >> 16);	//最高8位地址
	User_SPI_W25X16_SendByte((ReadAddress & 0xff00) >> 8);	//中间8位
	User_SPI_W25X16_SendByte(ReadAddress & 0xff);	//最低8位

	while (NumberOfRead--)
	{
		*DataTable = User_SPI_W25X16_SendByte(NoneCode);
		DataTable++;
	}

	SPI_W25X16_CS_DisSelect;
}

vu32 User_SPI_W25X16_ReadID(void)
{
	vu32 ID = 0;
	vu32 IDBuffer1 = 0;
	vu32 IDBuffer2 = 0;
	vu32 IDBuffer3 = 0;

	SPI_W25X16_CS_Select;

	User_SPI_W25X16_SendByte(JedecID);

	IDBuffer1 = User_SPI_W25X16_SendByte(NoneCode);	//读取高位
	IDBuffer2 = User_SPI_W25X16_SendByte(NoneCode);	//读取中位
	IDBuffer3 = User_SPI_W25X16_SendByte(NoneCode);	//读取低位

	SPI_W25X16_CS_DisSelect;

	ID = (IDBuffer1 << 16) | (IDBuffer2 << 8) | IDBuffer3;

	return ID;
}

vu32 User_SPI_W25X16_ReadDeviceID(void)
{
	vu32 ID = 0;
	vu32 IDBuffer1 = 0;
	vu32 IDBuffer2 = 0;
	vu32 IDBuffer3 = 0;

	SPI_W25X16_CS_Select;

	User_SPI_W25X16_SendByte(ManufatureID);

	IDBuffer1 = User_SPI_W25X16_SendByte(NoneCode);	//读取高位
	IDBuffer2 = User_SPI_W25X16_SendByte(NoneCode);	//读取中位
	IDBuffer3 = User_SPI_W25X16_SendByte(NoneCode);	//读取低位

	SPI_W25X16_CS_DisSelect;

	ID = (IDBuffer1 << 16) | (IDBuffer2 << 8) | IDBuffer3;

	return ID;
}

void User_SPI_W25X16_StartReadSequence(vu32 ReadAddress)
{
	SPI_W25X16_CS_Select;

	User_SPI_W25X16_SendByte(ReadData);

	User_SPI_W25X16_SendByte((ReadAddress & 0xff0000) >> 16);	//最高8位地址
	User_SPI_W25X16_SendByte((ReadAddress & 0xff00) >> 8);	//中间8位
	User_SPI_W25X16_SendByte(ReadAddress & 0xff);	//最低8位

	//SPI_W25X16_DisSelect;
}

void User_SPI_W25X16_PowerDown(void)
{
	SPI_W25X16_CS_Select;

	User_SPI_W25X16_SendByte(PowerDown);

	SPI_W25X16_CS_DisSelect;
}

void User_SPI_W25X16_WakeUp(void)
{
	SPI_W25X16_CS_Select;

	User_SPI_W25X16_SendByte(WakeUp);

	SPI_W25X16_CS_DisSelect;
}
