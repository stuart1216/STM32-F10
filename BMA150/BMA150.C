//定义函数
#include "BMA150.h"
#include "math.h"

//BMA150中指定地址读数据
u8  BMA150ReadReg(GPIO_TypeDef* GPIOx,u8 addr)
{
  u8 temp;
  IICStart(GPIOx);    //IIC开始
  IICSendByte(GPIOx,BMA150_WRITE); //IIC发送写命令
  temp=IICWaitAck(GPIOx); //等待应答
  IICSendByte(GPIOx,addr);              //发送指定的地址
  temp=IICWaitAck(GPIOx); //等待应答
  IICStart(GPIOx);    //IIC开始
  IICSendByte(GPIOx,BMA150_READ); //IIC发送读命令
  temp=IICWaitAck(GPIOx); //等待应答
  temp=IICReadByte(GPIOx,0); //读到的数据
  IICStop(GPIOx); //IIC停止
  return temp;
}

//指定地址连续读多个数据
u8   BMA150ConReadReg(GPIO_TypeDef* GPIOx,u8 addr,u8 *data,u8 len)
{
  u8 temp,i;
  IICStart(GPIOx);    //IIC开始
  IICSendByte(GPIOx,BMA150_WRITE); //IIC发送写命令
  temp=IICWaitAck(GPIOx); //等待应答
  IICSendByte(GPIOx,addr);              //发送指定的地址
  temp=IICWaitAck(GPIOx); //等待应答
  IICStart(GPIOx);    //IIC开始
  IICSendByte(GPIOx,BMA150_READ); //IIC发送读命令
  temp=IICWaitAck(GPIOx); //等待应答
  for(i=0;i<len; i++) {//读取长度为len的数据
    if(i
      data[i]=IICReadByte(GPIOx,1);
    else
     data[i]=IICReadByte(GPIOx,0);  
  }
  IICStop(GPIOx);                       //IIC停止
  return temp; //返回数值
}

//指定地址写入数据
u8  BMA150WriteReg(GPIO_TypeDef* GPIOx,u8 addr,u8 val)
{
   u8 temp;
   IICStart(GPIOx); //IIC开始时
   IICSendByte(GPIOx,BMA150_WRITE); //IIC发送写命令
   temp=IICWaitAck(GPIOx); //等待应答
   IICSendByte(GPIOx,addr); //发送要写的地址
   temp=IICWaitAck(GPIOx); //等待应答
   IICSendByte(GPIOx,val); //发送要写的数据
   temp=IICWaitAck(GPIOx); //等待应答
   IICStop(GPIOx);                      //IIC停止
   return temp;
}

//自我测试指令 返回1 自我测试成功 返回0 自测失败
u8  BMA150SelfTest(GPIO_TypeDef* GPIOx)
{
  u8 data;
  BMA150ConReadReg(GPIOx,ISTATUS_OR,&data,1); //读取寄存器0x0a的数值
  data|=0x04;
  BMA150WriteReg(GPIOx,ISTATUS_OR,data);       //写入自我测试位
  BMA150ConReadReg(GPIOx,ISTATUS_SF,&data,1); //读取寄存器0x09的数值
  data&=0x80;
  return data;

}

//软件复位寄存器
void BMA150SoftReset(GPIO_TypeDef* GPIOx)
{
  u8 data;
  BMA150ConReadReg(GPIOx,ISTATUS_OR,&data,1); //读取寄存器0x0a的数值
  data|=0x02;
  BMA150WriteReg(GPIOx,ISTATUS_OR,data);       //写入
}

//中断复位函数
void BMA150ResetInit(GPIO_TypeDef* GPIOx)
{
  u8 data;
  BMA150ConReadReg(GPIOx,ISTATUS_OR,&data,1); //读取寄存器0x0a的数值
  data|=0x40;  
  BMA150WriteReg(GPIOx,ISTATUS_OR,data);       //写入寄存器0x0a
}  

//设置精度范围
void BMA150SetBandRange(GPIO_TypeDef* GPIOx,u8 range)
{
  u8 data;
  BMA150ConReadReg(GPIOx,IACC_DK,&data,1);       //读取寄存器0x14的数值
  data&=range;                                   //设置带宽为375Hz精度为+-2g 
  BMA150WriteReg(GPIOx,IACC_DK,data);       //写入寄存器0x14

}

//设置各种中断模式
void  BMA150InterruptMode(GPIO_TypeDef* GPIOx,u8 mode)
{
  u8 data;
  BMA150ConReadReg(GPIOx,IPOWER_OR, &data,1); //读取寄存器0x15的数值
  data |= 0x40;
  BMA150WriteReg(GPIOx,IPOWER_OR, data);   //设置enable_adv_INT位不屏蔽中断
  if(mode>0 && mode<5)  {
    if(mode == BMA150_ENABLE_LG)
    BMA150WriteReg(GPIOx,IINTERPUT ,0x01);         //配置寄存器0x0B
    else if(mode == BMA150_ENABLE_HG) 
    BMA150WriteReg(GPIOx,IINTERPUT ,0x02);         //配置寄存器0x0B
    else if(mode == BMA150_ENABLE_ANY_MOTION)
    BMA150WriteReg(GPIOx,IINTERPUT ,0x40);         //配置寄存器0x0B
    else if(mode == BMA150_ENABLE_ALART)
    BMA150WriteReg(GPIOx,IINTERPUT ,0x80);         //配置寄存器0x0B
  } 
}

//设置持续时间值
void BMA150SetDur(GPIO_TypeDef* GPIOx,u8 int_type,u8 dur)
{
  u8 data;
  if(int_type == BMA150_ENABLE_LG) {    //低阈值持续时间
    BMA150WriteReg(GPIOx,ILG_DUR, dur);
  } else if(int_type == BMA150_ENABLE_HG){ //高阈值持续时间
    BMA150WriteReg(GPIOx,IHG_DUR, dur);
  } else if(int_type == BMA150_ENABLE_ANY_MOTION){//确定中断最小持续时间
    BMA150ConReadReg(GPIOx,IMOTION, &data,1);
    dur <<= 6;
    data |= dur;
    BMA150WriteReg(GPIOx,IMOTION, data);
  }
}

//设置中断锁存
void  BMA150IntTrigMode(GPIO_TypeDef* GPIOx,u8 mode)
{
  u8 data;
  BMA150ConReadReg(GPIOx,IPOWER_OR, &data,1); //读取寄存器0x15的数值
  if(mode == 1)  {
    data |= 0x10;
    BMA150WriteReg(GPIOx,IPOWER_OR, data); //设置中断锁存
  }  else if(mode == 0){
    data &= ~0x10;
    BMA150WriteReg(GPIOx,IPOWER_OR, data);
  }
}

//设置阈值
void  BMA150SetThres(GPIO_TypeDef* GPIOx,u8 int_type,u8 thres)
{
  if(int_type == BMA150_ENABLE_LG)  {
  BMA150WriteReg(GPIOx,ILG_THRES, thres);  // 设置低阈值的时间
  } else if(int_type == BMA150_ENABLE_HG){ // 设置高阈值的时间
    MA150WriteReg(GPIOx,IHG_THRES, thres);
  } else if(int_type == BMA150_ENABLE_ANY_MOTION) {// 设置监控时间
    BMA150WriteReg(GPIOx,IANYMOTION, thres);
  }  
}

//设置模式
void  BMA150SetMode(GPIO_TypeDef* GPIOx,u8 mode)
{
  u8 data1,data2;
  if (mode == BMA150_WAKE_UP){   //选择唤醒模式
    BMA150ConReadReg(GPIOx,IPOWER_OR, &data1,1);
    data1 |=0x01;
    BMA150WriteReg(GPIOx,IPOWER_OR, data1);
  } else if(mode == BMA150_SLEEP)  { //选择睡眠模式
    BMA150ConReadReg(GPIOx,ISTATUS_OR, &data2,1);
    data1 |=0x01;
    BMA150WriteReg(GPIOx,ISTATUS_OR, data2);
  }
}

 //设置唤醒时间值
void  BMA150SetWakeUpPause(GPIO_TypeDef* GPIOx,u8 pause)
{
  u8 data;
  BMA150ConReadReg(GPIOx,IPOWER_OR, &data,1);
  if(pause == BMA150_PAUSE_20MS) {//设置唤醒时间20MS
    data |= 0x00;
    BMA150WriteReg(GPIOx,IPOWER_OR, data);
  } else if(pause == BMA150_PAUSE_80MS) {//设置唤醒时间80MS
    data |= 0x02;
    BMA150WriteReg(GPIOx,IPOWER_OR, data);
  } else if(pause == BMA150_PAUSE_320MS) {//设置唤醒时间320MS
    data |= 0x04;
    BMA150WriteReg(GPIOx,IPOWER_OR, data);
  } else if(pause == BMA150_PAUSE_2560MS) {//设置唤醒时间2560MS
    data |= 0x06;
    BMA150WriteReg(GPIOx,IPOWER_OR, data);
  }
}

//获取加速度的算法
void  BMA150ReadAccelXYZ(GPIO_TypeDef* GPIOx,struct bma150acc *acc)
{
  u8 data[6];
  BMA150ConReadReg(GPIOx,IACC_X0,data,6);   //连续读取6个数值
  acc->x=(short)(((u16)data[1]<<2)|(data[0]>>6));
  acc->x = acc->x << 6;   //左移6位
  acc->x = acc->x >> 6;   //右移6位 去除0
  acc->y=(short)(((u16)data[3]<<2)|(data[2]>>6));
  acc->y = acc->y << 6;
  acc->y = acc->y >> 6;
  acc->z=(short)(((u16)data[5]<<2)|(data[4]>>6));
  acc->z = acc->z << 6;
  acc->z = acc->z >> 6;
}

//角度转换
short  BMA150GetAngle(float x,float y,float z,u8 dir)
{
  float temp;
  switch(dir)  {
   case 0:                             //与自然Z轴的角度
     temp=atan(sqrt((x*x+y*y))/z);
    break;
   case 1:                             //与自然x轴的角度
     temp=atan(x/sqrt((y*y+z*z)));
    break;
   case 2:                             //与自然y轴的角度
     temp=atan(y/sqrt((x*x+z*z)));
      break;
    defalut:
      break;
  }
  return temp*1800/3.14;
}

//初始化函数 返回0 初始化成功 返回1 失败
u8  BMA150Init(GPIO_TypeDef* GPIOx)
{
  u8 data;
  IICInit(GPIOx); //IIC初始化
  BMA150ConReadReg(GPIOx,IDEVID,&data,1);    //读取寄存器0x00的数
  if(data == 0x02) {//检查期间ID是否正确
    BMA150SoftReset(GPIOx); //1,软件复位
    delay_ms(10);
    BMA150ResetInit(GPIOx);                    //2,中断复位
    BMA150SetBandRange(GPIOx,0x40);            //3，设置带宽为375HZ精度+-2g
    BMA150InterruptMode(GPIOx,BMA150_ENABLE_ANY_MOTION); //4，设置中断模式
    BMA150SetDur(GPIOx,BMA150_ENABLE_ANY_MOTION,1);      //5，设置中断持续时间1                                                  
    BMA150IntTrigMode(GPIOx,1);  //6、设定中断触发类型（是否保持）                                                
    BMA150SetThres(GPIOx,BMA150_ENABLE_ANY_MOTION,2);  //7、设置中断阀值                                                 
    BMA150SetMode(GPIOx,BMA150_WAKE_UP);      //8、设置模式（唤醒模式）
    BMA150SetWakeUpPause(GPIOx,BMA150_PAUSE_20MS);       //9、设置轮询时间
    return 0; 
  }
  return 1;
}


#include  "common.h"
//角度转换
void AdxlShowNum(u16 x,u16 y,short num,u8 mode)
{
  if(mode == 0) {
    if(num<0) {
     LcdShowChar(x,y,'-',16,0);
      num=-num;
    } else 
      LcdShowChar(x,y,' ',16,0);
    LcdShowNum(x+8,y,num,4,16);
  } else {
    if(num<0) {
      LcdShowChar(x,y,'-',16,0);
      num=-num;
    } else 
      LcdShowChar(x,y,' ',16,0);
    LcdShowNum(x+8,y,num/10,2,16); //显示整数部分
    LcdShowChar(x+24,y,'.',16,0);   //显示小数点
    LcdShowNum(x+32,y,num,1,16); //显示小数部分  
  }
}

//主函数
int main()
{  
  u8 k=0;
  Init();
  POINT_COLOR=RED;            //设置字体颜色为红色  
  LcdShowString(30,50,200,16,16,"Shang Ji Cheng Dian Lu");
  LcdShowString(30,70,200,16,16,"IIC Test");
  LcdShowString(30,90,200,16,16,"Yan Fa Zhong Xin");  
  LcdShowString(30,110,200,16,16,"2014/5/5");  
  LcdShowString(30,130,200,16,16,"Xian Shi Shu Ju");
  while(BMA150Init(GPIOB)){ 
    LcdShowString(30,150,200,16,16,"BMA150 Error");
    delay_ms(200);
    LcdFill(30,150,239,150+16,WHITE);
    delay_ms(200);
  }
  LcdShowString(30,150,200,16,16,"BMA150 OK"); //在LCD上显示字符
  LcdShowString(30,170,200,16,16,"X VAL:");
  LcdShowString(30,190,200,16,16,"Y VAL:");
  LcdShowString(30,210,200,16,16,"Z VAL:");
  LcdShowString(30,230,200,16,16,"X ANG:");
  LcdShowString(30,250,200,16,16,"Y ANG:");
  LcdShowString(30,270,200,16,16,"Z ANG:");  
  POINT_COLOR=BLUE;        //设置字体为蓝色 
  while(1){          
    short x=0;
    short y=0;
    short z=0;
    short angx,angy,angz;
    if(READ_INT){  
      struct bma150acc acc;
      u8 i;
      for(i=0;i<10;i++){    
        BMA150ReadAccelXYZ(GPIOB,&acc);  //获取结构体的加速度数值
        x+=acc.x;
        y+=acc.y;
        z+=acc.z;
      }  
      angx=x/10;
      angy=y/10;
      angz=z/10;
      AdxlShowNum(30+48,170,angx,0);   //获取加速度的值
      AdxlShowNum(30+48,190,angy,0);
      AdxlShowNum(30+48,210,angz,0);
      angx=BMA150GetAngle(angx,angy,angz,1);        //通过转换得到角度
      angy=BMA150GetAngle(angx,angy,angz,2); 
      angz=BMA150GetAngle(angx,angy,angz,0);
      AdxlShowNum(30+48,230,angx,1);    //显示转换的角度
      AdxlShowNum(30+48,250,angy,1);
      AdxlShowNum(30+48,270,angz,1);
    }
    if(k==0) {//LED0的不断闪烁
      LedOn(LED0);
      k++;
      delay_ms(5);
    } else if(k==1){
      LedOff(LED0);
      k--;
      delay_ms(5);
    }       
  }  
}   


