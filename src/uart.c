/*
*********************************************************
*工 程 名：uart
*功能描述：uart模块使用
*作    者：PDY
*时    间：2014.12.10
*版    本：V1.0.0
*********************************************************
*/
#include"rb57mxx5a.h"
#include "string.h"
#include "type.h"
#include "common.h"

#define LENGTH  20
u8 uart_r_flag=0;
u8 receive_data;
u8 receive_lenth=0;
u8 receivebuff[LENGTH];

/*
**********************************************************
*函 数 名：uart_config
*功能描述：uart模块配置，F=4M,波特率19200
*输    入：无
*输    出：无
* baudrate = (2^SMOD/32)*(1/(256-N))*PCLK
	baudrate(3M) = 3*1000*1000/16*10 = 18750
	baudrate(6M) = 6*1000*1000/16*10 = 37500
	baudrate(12M) = 12*1000*1000/16*13 = 57692.307692307692307692307692308
**********************************************************
*/
void uart_config(void)
{	
	//ES = 0; //关中断
// io config
	P3DPL = 0x56; //P3.0(TX)推挽输出  P3.1(RX)上拉输入
// io config
	IOMUXC0 |= 0x40;  //UART口输出复用
	SCON = 0x44;    //工作模式1,8位异步，波特率可调
	BRCON = 0xF0;   //波特率发生器开启，SMOD=1；F0
	#if defined(MCLK_DIV_1)
	BRTIM = 256-13;
	#else
	BRTIM = 256-10;
	#endif

	REN = 1;//开启串口数据接收
	RI = 0;
	ES = 1; //开中断
}


/*
**********************************************************
*函 数 名：send_str
*功能描述：发送一串字符
*输    入：*s，要发送的数据的首地址
*输    出：无
**********************************************************
*/
void uart_print(unsigned char *s)
{
	while(*s!=0)
	{
		SBUF = *s;
		while(TI == 0)
		{
		}
		TI = 0;
		s++;
	}
}

char putchar (char ch)
{
	SBUF = ch;
	while(TI == 0)
	{
	}
	TI = 0;
	
	return ch;
}


void uart_r_string(void)
{
	if(uart_r_flag)
	{
		uart_r_flag = 0;
		uart_print(receivebuff);
		memset(receivebuff, 0, LENGTH);
		receive_lenth = 0;
	}
}
/*
**********************************************************
*函 数 名：UartInterrupt
*功能描述：中断服务函数,将数据放入缓存数组receivebuff[]中
*输    入：无
*输    出：无
**********************************************************
*/

void UartInterrupt(void) interrupt 4
{
	if(RI)
	{
		RI = 0;
		receive_data=SBUF;
		if((receive_data==0x0a)|(receive_data==0x0d)|(receive_lenth==20))
		{
//检测接收数据超过20个或一帧数据结束标志“oxod oxoa”
//（即回车换行）都表示数据接收完成
			//flag=1;
			uart_r_flag = 1;
			receive_lenth=0;
		}
		else
		{
			receivebuff[receive_lenth]=receive_data;
			receive_lenth++;
		}
	}    
}

