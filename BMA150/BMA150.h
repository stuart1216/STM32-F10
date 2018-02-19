#ifndef  _BMA150_H_
#define  _BMA150_H_
#include "common.h"
//寄存器读写
#define  BMA150_READ   0x71
#define  BMA150_WRITE  0x70
//管脚PG13
#define  READ_INT   PGin(13)
//加速度传感器地址
#define  IDEVID      0x00
#define  IVERSION    0x01
#define  IACC_X0     0x02
#define  IACC_X1     0x03
#define  IACC_Y0     0x04
#define  IACC_Y1     0x05
#define  IACC_Z0     0x06
#define  IACC_Z1     0x07
#define  ISTATUS_SF  0x09
#define  ISTATUS_OR  0x0A
#define  IINTERPUT   0x0B
#define  ILG_THRES   0x0C
#define  ILG_DUR     0x0D
#define  IHG_THRES   0x0E
#define  IHG_DUR     0x0F
#define  IANYMOTION  0x10
#define  IMOTION     0x11
#define  IACC_DK     0x14
#define  IPOWER_OR   0x15
#define  IOFFSET_X   0x1A
#define  IOFFSET_Y   0x1B
#define  IOFFSET_Z   0x1C

#define  EINTERPUT   0x2B
#define  EMOTION     0x31
#define  ECUSTOMER1  0x32
#define  ECUSTOMER2  0x33
#define  EACC_DK     0x34
#define  EPOWER_0R   0x35
#define  EOFFSET_X   0x3C
#define  EOFFSET_Y   0x3B
#define  EOFFSET_Z   0x3A
//设置中断模式
#define BMA150_ENABLE_LG         1
#define BMA150_ENABLE_HG         2
#define BMA150_ENABLE_ANY_MOTION 3
#define BMA150_ENABLE_ALART      4
//设置模式
#define BMA150_WAKE_UP 0
#define  BMA150_SLEEP   1
//设置唤醒时间
#define BMA150_PAUSE_20MS     0
#define BMA150_PAUSE_80MS     1
#define BMA150_PAUSE_320MS    2
#define BMA150_PAUSE_2560MS   3
//结构体类型
struct bma150acc{
    s16 x,
    y,
    z;
};

//初始化函数 返回0 初始化成功 返回1 失败
u8  BMA150Init(GPIO_TypeDef* GPIOx);
//获取加速度的算法
void  BMA150ReadAccelXYZ(GPIO_TypeDef* GPIOx,struct bma150acc *acc);
//角度转换
short  BMA150GetAngle(float x,float y,float z,u8 dir);
#endif
