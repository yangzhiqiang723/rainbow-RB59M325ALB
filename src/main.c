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
#include"string.h"
#include"stdio.h"
#include"stdlib.h"

#include "type.h"
#include "common.h"
#include "qst_sw_i2c.h"
#include "iic.h"
#include "qmaX981.h"

enum
{
	MODE_PUC,
	MODE_WORK,
	MODE_PD,
	MODE_IDL,

	MODE_TOTAL
};

static s32 exi0_flag = 0;
static s32 exi1_flag = 0;
static u32 loop_count = 0;

/*
**********************************************************
*函 数 名：mclk_config
*功能描述：主时钟配置
*输    入：无
*输    出：无
**********************************************************
*/
void mclk_config(void)
{
    SPKEY = 0x36;
// bit 0:1
// 00:MCLK=SYSCLK/16 
// 01:MCLK=SYSCLK/ 4
// 10:MCLK=SYSCLK/2 
// 11:MCLK=SYSCLK/1
#if defined(MCLK_DIV_16)
	CKDS = 0x00;
#elif defined(MCLK_DIV_4)
	CKDS = 0x01;	// div 4, 12/4=3MHz
#elif defined(MCLK_DIV_2)
	CKDS = 0x02;	// div 2, 12/2=6MHz
#elif defined(MCLK_DIV_1)
	CKDS = 0x03;	// div 1, 12/1=12MHz
#else
	CKDS = 0x02;	// div 2, 12/2=6MHz
#endif
// 选择输出的时钟源
// 000:IHRCCLK/4;001:IHRCCLK; 010:HCRYCLK;
// 011:ILRCCLK;100:LCRYCLK; 101:LFCLK; 110/111:MCU MCLK
	CKOCFG = 0X0F;	// MCLK
	//P0DPL = 0XAA;
}

#define INT1_EN		0x20
#define INT0_EN		0x10
#define INT1_PS		0x80
#define INT0_PS		0x00	//0x40

void exi_config(void)
{
	P1DPL = 0xA0;		// p1.0 p1.1float input
	// bit0 (0:INT0 ->P1.0,  1:INT0->P3.0)
	// bit1 (0:INT1 ->P1.1,  1:INT2->P3.1)
	IOMUXC0 &= 0xFC;
	EX0 = 0x01;
	PX0 = 0x01;
	IT0 = 0x01;	// TCON bit 0:IT0 (0:level low  1:trigger low)

	//EX1 = 0x01;
	//PX1 = 0x01;
	//IT1 = 0x01;	// TCON bit 2:IT1 (0:level low  1:trigger low)
	
	// INTCON0
	// INT0 bit4(INT0EN), bit2(INT0PS)
	// INTS0=0  IT0= 0 INT0PS=0	level low
	// INTS0=0  IT0= 0 INT0PS=1	level high
	// INTS0=0  IT0= 1 INT0PS=0	trigger low
	// INTS0=0  IT0= 1 INT0PS=1	trigger high
	INTCON0 = INT0_EN|INT0_PS;//|INT1_EN|INT1_PS;

}

void delay(int num)
{
	while(num>0)
	{
		num--;
	}
}

void enter_mode(int mode)
{
	if(mode == MODE_PD)
	{
		SPKEY = 0x36;
		PCON |= 0x02;
	}
	else if(mode == MODE_IDL)
	{
		SPKEY = 0x36;
		PCON |= 0x01;
	}
}

void main(void)
{
	//u8 chipid;
	s32 acc_data[3];
	EA = 0;  //关总中断
	mclk_config();
	uart_config();
	exi_config();
    //IOMUXC3 = 0x20;  //允许I2C管脚复用，30配置 P3.1(SCL) P3.0(SDA)口  20配置 P2.5(SCL) P2.4(SDA)口
    //P2DPH = 0xAA;

	EA = 1;
#if defined(USE_SW_IIC)
	i2c_CheckDevice(0x24);
#else
	MIIC_Init(0x20, 0x81);
#endif
	printf("MCU tsst start\r\n");
	qmaX981_init();
	while(1)
	{
		//if(loop_count < 500)
		{			
			qmaX981_read_acc(acc_data);
			printf("acc data [%d %d %d] \r\n", acc_data[0],acc_data[1],acc_data[2]);
			uart_r_string();
			loop_count++;
			if(loop_count == 100)
			{
				printf("enter PD mode!! \r\n");
				//qmaX981_writereg(0x11, 0x00);
				enter_mode(MODE_PD);
			}
		}
		if(exi0_flag == 1)
		{
			printf("exi0_flag = %d \r\n", exi0_flag);
			printf("acc data [%d %d %d] \r\n", acc_data[0],acc_data[1],acc_data[2]);
			exi0_flag = 0;
			loop_count = 0;
			
			//qmaX981_writereg(0x11, 0x80);
			delay(1000);
		}
		if(exi1_flag == 1)
		{
			printf("exi1_flag = %d \r\n", exi0_flag);
			printf("acc data [%d %d %d] \r\n", acc_data[0],acc_data[1],acc_data[2]);
			exi1_flag = 0;
			loop_count = 0;
		}
			
		delay(5000);
	}
}


void exi0_handle(void) interrupt 0
{
	//IE0 = 0;
	EX0=0;
	//exi_config();
	exi0_flag = 1;
	EX0=1;
}

void exi1_handle(void) interrupt 2
{
	EX1=0;
	exi1_flag = 1;
	EX1=1;
}



