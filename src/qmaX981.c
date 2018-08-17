 
//#include <REGX52.H>

#include"rb57mxx5a.h"
#include "qmaX981.h"
//#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "common.h"
#include "qst_sw_i2c.h"
#include "iic.h"
	
#define QMAX981_LOG		printf
#define QMAX981_ERR		printf
//#define QMAX981_USE_SPI

#if defined(USE_SW_IIC)
#define qmaX981_writereg		qst_sw_writereg
#define qmaX981_readreg			qst_sw_readreg
#else
u8 qmaX981_readreg(u8 reg_add,u8 *buf,u8 num)
{
	MIIC_Read(0x24,reg_add,buf,num);
	return 1;
}


u8 qmaX981_writereg(u8 reg_add,u8 reg_dat)
{
	MIIC_Write(0x24,reg_add,&reg_dat,1);
	return 1;
}
#endif

#if 0

const unsigned char xdata qma6981_init_tbl[][2] = 
{
#if defined(QMAX981_STEP_COUNTER)
	{0x11, 0x80},
	{0x36, 0xb6},
	{0xff, 5},
	{0x36, 0x00},
	{0x11, 0x80},
	{0x0f, QMAX981_RANGE_8G},
	{0x10, 0x2a},
	{0x12, 0x8f},
	{0x13, 0x10},
	{0x14, 0x14},
	{0x15, 0x10},	
	{0x16, 0x0c},
#if defined(QMAX981_STEP_DEBOUNCE_IN_INT)
	{0x19, 0x08},
#endif
	{0x32, 0x02},
	{0x27, QMA6981_OFFSET},
	{0x28, QMA6981_OFFSET},
	{0x29, QMA6981_OFFSET},
#else
	{0x11, 0x80},
	{0x36, 0xb6},
	{0xff, 5},
	{0x36, 0x00},
	{0x11, 0x80},
	//{0x36, 0xb6},
	{0xff, 5},
	//{0x36, 0x00},
	//{0x11, 0x80},
	{0x0f, QMAX981_RANGE_4G},
	{0x10, QMA6981_ODR_125HZ},
#endif
#if defined(QMAX981_FIFO_FUNC)
	{0x10, QMA6981_ODR_250HZ},
	{0x11, 0x8b},
	{0x3E, 0x40},
	{0x17, 0x20},
	#if defined(QMAX981_FIFO_USE_INT)
	{0x1a, 0x20},	// fifo int map to int1
	#endif
#endif
#if defined(QMAX981_TAP_FUNC)
	{0x10, 0x05},
	{0x11, 0x80},	// 0x85 {0x2a, 0x80},	
	{0x2b, 0x03},	//0x14	125*7
	{0x16, 0x20},	
	{0x19, 0x20},
	//{0x1b, 0x20},
#endif
#if defined(QMAX981_INT_LATCH_MODE)
	{0x21, 0x01},
#endif
#if 1
	{0x20, 0x00},		// ÏÂ½µÑØ´¥·¢
#endif
	{0xff, 1}
};

#endif




void qmaX981_delay(unsigned int delay)
{
	int i,j;
	for(i=0;i<delay;i++)
	{
		for(j=0;j<1000;j++)
		{
			;
		}
	}
}


#if 0
u8 qmaX981_writereg(u8 reg_add,u8 reg_dat)
{
	u8 ret=0;

	ret = qst_sw_writereg(reg_add,reg_dat);
	return 1;
}

u8 qmaX981_readreg(u8 reg_add,u8 *buf,u8 num)
{
	u8 ret;

	ret = qst_sw_readreg(reg_add,buf,num);
	return ret;
}
#endif


s32 qmaX981_init(void)
{
	int ret = 0;
	u8 chipid = 0;
	
	qmaX981_readreg(0x00, &chipid, 1);
	if(chipid == 0xb0)
	{
		printf("qma6981 found chipid=%x \r\n", (int)chipid);
	}
	qmaX981_writereg(0x11, 0x80);
	qmaX981_writereg(0x36, 0xb6);
	qmaX981_writereg(0xff, 5);
	qmaX981_writereg(0x36, 0x00);
	qmaX981_writereg(0x11, 0x80);
	qmaX981_delay(5),
	qmaX981_writereg(0x0f, QMAX981_RANGE_4G);
	qmaX981_writereg(0x10, QMA6981_ODR_125HZ);

	qmaX981_writereg(0x10, 0x05);
	qmaX981_writereg(0x11, 0x80);	// 0x85 {0x2a, 0x80},	
	qmaX981_writereg(0x2b, 0x03);	//0x14	125*7
	qmaX981_writereg(0x16, 0x20);	
	qmaX981_writereg(0x19, 0x20);
	qmaX981_writereg(0x20, 0x00);

	return ret;
}


static int qma6981_read_raw_xyz(int *data_buf)
{
	//int res;	
	unsigned char databuf[6] = {0};		
	unsigned char i;
	int ret;

	ret = qmaX981_readreg(QMAX981_XOUTL, databuf, 6);
	if(ret == 0){
		QMAX981_ERR("read xyz error!!!");
		return 0;	
	}
 	data_buf[0]  = (short)((databuf[1]<<2) |( databuf[0]>>6));
	data_buf[1]  = (short)((databuf[3]<<2) |( databuf[2]>>6));
	data_buf[2]  = (short)((databuf[5]<<2) |( databuf[4]>>6));

	for(i=0;i<3;i++)				
	{	//because the data is store in binary complement number formation in computer system
		if ( data_buf[i] == 0x0200 )	//so we want to calculate actual number here
			data_buf[i]= -512;		//10bit resolution, 512= 2^(10-1)
		else if ( data_buf[i] & 0x0200 )	//transfor format
		{					//printk("data 0 step %x \n",data[i]);
			data_buf[i] -= 0x1;			//printk("data 1 step %x \n",data[i]);
			data_buf[i] = ~data_buf[i];		//printk("data 2 step %x \n",data[i]);
			data_buf[i] &= 0x01ff;		//printk("data 3 step %x \n\n",data[i]);
			data_buf[i] = -data_buf[i];		
		}
#if defined(QMAX981_STEP_COUNTER)
		data_buf[i] -= QMA6981_OFFSET;
#endif
	}

	//QMAX981_LOG("yzqaccraw	%d	%d	%d\n", data_buf[0], data_buf[1], data_buf[2]);
	return 1;
}



s32 qmaX981_read_acc(s32 *accData)
{
	int ret;
	s32 rawData[3];

	qma6981_read_raw_xyz(rawData);

	accData[0] = (s32)(rawData[0]*(GRAVITY_EARTH_1000/128));
	accData[1] = (s32)(rawData[1]*(GRAVITY_EARTH_1000/128));
	accData[2] = (s32)(rawData[2]*(GRAVITY_EARTH_1000/128));

	return ret;

}

