/*
*********************************************************
*文 件 名：IIC.c
*功能描述：
*作    者：
*时    间：2016.12.12
*版    本：V1.0.0
*注    意：清中断标志位SI必须放在事件处理的最后
*********************************************************
*/

#include "type.h"
#include "IIC.h"
#include "rb57mxx5a.h"

static u8 I2C_FLAG = 0;
u8  SR_Date[10];
u8  RW_Data[10] = {0xB8, 0xB8, 0xB8, 0xB8};
/*
**********************************************************
*函 数 名：IIC初始化  主机
*功能描述：
*输    入：无
*输    出：无
**********************************************************
*/
void MIIC_Init(u8 Iomux3_data, u8 Clk_Cfg)
{
    //  P2DPH |= 0x0f;     //配置I2C接口   P2.5(SCL) P2.4(SDA)为开漏输出   输出波形会出现一个跌落  设置为浮空输入
    //    IOMUXC3 = 0x20;      //允许I2C管脚复用，配置 P2.5(SCL) P2.4(SDA)口
    IOMUXC3 = Iomux3_data;  //允许I2C管脚复用，30配置 P3.1(SCL) P3.0(SDA)口  20配置 P2.5(SCL) P2.4(SDA)口
    I2CCON = Clk_Cfg;      //60分频  IIC时钟
    I2CEN = 1;             //使能I2C模块
    EIE1 |= 0X02;          //开启I2C中中断

}
/*
**********************************************************
*函 数 名：IIC写函数  主机
*功能描述：
*输    入：Slave_Address 设备地址(已经向右移一位)  Reg_address 寄存器地址  W_Data 写数据  number 数量
*输    出：无
**********************************************************
*/
void MIIC_Write(u8 Slave_Address, u8 Reg_address, u8 *W_Data, u8 number)
{
    u8 i = 0;
    STO = 0;
    STA = 1;                              //start 信号
    I2C_FLAG = 0;
    while(I2C_FLAG != START_STATE);           //判断start信号是否发送完成
    I2CDAT = Slave_Address;                   //写设备地址 + W
    I2C_FLAG = 0;
    SI = 0;
    while(I2C_FLAG != SLA_W_ASTATE);           //判断设备地址和写信号是否发送完成
    I2CDAT = Reg_address;                      //写寄存器数据
    I2C_FLAG = 0;
    STA = 0;
    SI = 0;
    while(I2C_FLAG != DATA_W_ASTATE);           //判断寄存器数据是否发送完成
    for(i = 0; i < number; i++)
    {
        I2CDAT = W_Data[i];                     //写数据
        //       I2CSTAR = 0;这句增加低频部分会有问题，I2C中线不能空闲
        I2C_FLAG = 0;
        SI = 0;
        while(I2C_FLAG != DATA_W_ASTATE);       //判断数据是否发送完成
        if(i == number - 1)
        {
            //            I2CSTAR = 0;
            STO  = 1;                        //写stop
            SI = 0;
        }
    }
}
/*
**********************************************************
*函 数 名：IIC读函数随机地址  主机
*功能描述：
*输    入：Slave_Address 设备地址(已经向右移一位) Reg_address 寄存器地址  number 数量
*输    出：*R_Data  读取数据
**********************************************************
*/
void MIIC_Read(u8 Slave_Address, u8 Reg_address, u8 *R_Data, u8 number)
{
    u8 i = 0;
    STO = 0;
    STA = 1;                              //start 信号
    I2C_FLAG = 0;
    while(I2C_FLAG != START_STATE);           //判断start信号是否发送完成
    I2CDAT = Slave_Address;                   //写设备地址 + W
    I2C_FLAG = 0;
    SI = 0;
    while(I2C_FLAG != SLA_W_ASTATE);          //判断设备地址和写信号是否发送完成
    I2CDAT = Reg_address;                     //写寄存器地址
    I2C_FLAG = 0;
    STA = 0;
    SI = 0;
    while(I2C_FLAG != DATA_W_ASTATE);         //判断设备寄存器地址是否发送完成
    STA = 1;                              //重复 start 信号
    I2C_FLAG = 0;
    SI = 0;
    while(I2C_FLAG != R_START_STATE);
    I2CDAT = Slave_Address | 0x01;           //写设备地址 R
    I2C_FLAG = 0;
    SI = 0;
    while(I2C_FLAG != SLA_R_ASTATE);         //判断设备地址 R是否发送完成
    STA = 0;
    /************修改读取一个数据时候**************/
	if(number > 1)
	AA = 1;
	else
    AA = 0;
    /************修改读取一个数据时候**************/
    SI = 0;
    I2C_FLAG = 0;
    for(i = 0; i < number; i++)
    {
        if(i < number - 1)
        {
            while(I2C_FLAG != DATA_R_ASTATE);  //判断数据是否接收完成
            R_Data[i] = I2CDAT;
            if(i == number - 2)
            {
                AA = 0;
            }
            else
            {
                AA = 1;
            }
            SI = 0;
            I2C_FLAG = 0;
        }
        else
        {
            while(I2C_FLAG != DATA_R_NASTATE); //判断数据是否是最后一个
            R_Data[i] = I2CDAT;
            STA = 0;
            STO  =  1;                      //写stop
            SI = 0;
        }

    }
}
/*
**********************************************************
*函 数 名：IIC读函数当前数据  主机
*功能描述：
*输    入：Slave_Address 设备地址(已经向右移一位)  number 数量
*输    出：*R_Data  读取数据
**********************************************************
*/
//void MIIC_Read_Current(u8 Slave_Address, u8 *R_Data, u8 number)
//{
//    u8 i = 0;
//    STO = 0;
//    STA= 1;                              //start 信号
//    I2C_FLAG = 0;
//    while(I2C_FLAG != START_STATE);           //判断start信号是否发送完成
//    I2CDAT = Slave_Address;           //写设备地址 R
//    I2C_FLAG = 0;
//    SI = 0;
//    while(I2C_FLAG != SLA_R_ASTATE);         //判断设备地址 R是否发送完成
//    STA = 0;
    /***********修改读取一个数据时候***************/
//		if(number > 1)
//		AA = 1;
//		else
//	    AA = 0;
    /************修改读取一个数据时候**************/
//    SI = 0;
//    I2C_FLAG = 0;
//    for(i = 0; i < number; i++)
//    {
//        if(i < number - 1)
//        {
//            while(I2C_FLAG != DATA_R_ASTATE);  //判断数据是否接收完成
//            R_Data[i] = I2CDAT;
//            if(i == number - 2)
//            {
//                AA = 0;
//            }
//            else
//            {
//                AA = 1;
//            }
//            SI = 0;
//            I2C_FLAG = 0;
//        }
//        else
//        {
//            while(I2C_FLAG != DATA_R_NASTATE); //判断数据是否是最后一个
//            R_Data[i] = I2CDAT;
//            STA = 0;
//            STO  =  1;                      //写stop
//            SI = 0;
//        }
//
//    }
//}
/*
**********************************************************
*函 数 名：IIC初始化  从机
*功能描述：
*输    入：无
*输    出：无
**********************************************************
*/
void SIIC_Init(u8 Slave_Address)
{
//    P2DPH |= 0x05;           //配置I2C接口   P2.5(SCL) P2.4(SDA)为上拉输入
    IOMUXC3 = 0x20;         //允许I2C管脚复用，配置 P2.5(SCL) P2.4(SDA)口
    I2CADR = Slave_Address | 0x01 ;
    AA = 1;
    I2CEN = 1;
    EIE1 |= 0X02;           //开启I2C中中断
//    EA = 1;
    //    I2CSAR = Slave_Address >> 1; //设置从机地址
    //I2CCTR = 0x19;          //使能I2C模块，开I2C中断,从机模式
}
/*
**********************************************************
*函 数 名：IIC写函数  从机
*功能描述：
*输    入：W_Data 写数据
*输    出：无
**********************************************************
*/
//void SIIC_Write(u8 *W_Data)
//{

//}
/*
**********************************************************
*函 数 名：IIC读函数  从机
*功能描述：
*输    入：Slave_Address 设备地址(已经向右移一位)    number 数量
*输    出：*R_Data  读取数据
**********************************************************
*/
//void SIIC_Read(u8 *R_Data)
//{

//}

/*
**********************************************************
*函 数 名：IIC中断服务函数
*功能描述：
*输    入：
*输    出：
**********************************************************
*/
void I2CInterrupt(void) interrupt 7  //中断服务函数
{
    static u8 i;
    EA = 0;
	P00 = ~P00;
	P00 = ~P00;


    I2C_FLAG = I2CSTA;
    
    switch(I2C_FLAG)
    {
    case START_STATE:
        //		I2CSTAR = 0;
        break;
    case R_START_STATE:
        break;
    case SLA_W_ASTATE:
        break;
    case SLA_W_NASTATE:
        break;
    case DATA_W_ASTATE:
        break;
    case DATA_W_NASTATE:
        break;
    case S_SlAR_ASTATE://判断主机发送的地址和读信号是否匹配，从机写
        I2CDAT = RW_Data[i];
        i++;
        I2C_FLAG = 0;
        AA = 1;
        SI = 0;
        break;
    case S_DATAW_ASTATE: //从机写数据
        I2CDAT = RW_Data[i];
        I2C_FLAG = 0;
        STO = 0;
        AA = 1;
        i++;
        SI = 0;
        break;
    case LS_DATAW_ASTATE:  //从机写最后一个数据
        I2CDAT = RW_Data[i];
        I2C_FLAG = 0;
        AA = 1;
        STA = 0;
        i = 0;
        SI = 0;
        break;
    case S_DATAW_NASTATE: //从机写数据发送完成，接收到NACK
        I2C_FLAG = 0;
        AA = 1;
        STA = 0;
		    i = 0;
        SI = 0;
        break;
    case S_SlAW_ASTATE:   //判断主机发送的地址和写信号是否匹配 从机读
        AA = 1;
        I2C_FLAG = 0;
        SI = 0;
        break;
    case S_DATAR_ASTATE:  //从机读接收到数据和ACK信号
        SR_Date[i] = I2CDAT;
        AA = 1 ;
        I2C_FLAG = 0;
        SI = 0;
        i++;
        break;
    case SLA_STOP_STATE: //从机读接收到stop信号，结束接收数据
        i = 0;
        AA= 1;
        STA = 0;
        STO = 0;
        SI = 0;
        break;
    case GC_ADD_ASTATE:
        AA = 1;
        I2C_FLAG = 0;
        SI = 0;
        break;
    case GC_DATAR_ASTATE:
        SR_Date[i] = I2CDAT;
        AA = 1 ;
        I2C_FLAG = 0;
        SI = 0;
        i++;
        break;

    }



    EA = 1;
}
