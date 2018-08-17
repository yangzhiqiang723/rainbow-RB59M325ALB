
//#include <REGX52.H>
#include "intrins.h"		// for _nop_
#include "rb57mxx5a.h"
#include "qst_sw_i2c.h"

#if defined(USE_SW_IIC)
//#define QST_SW_IIC_1
#define QST_SW_IIC_2
#endif

#if defined(QST_SW_IIC_1)
static unsigned char QST_SW_I2C_ADDR_W = 0x24;
static unsigned char  QST_SW_I2C_ADDR_R = 0x25;

#define I2C_SCL_PIN		P25			//P2_1
#define I2C_SDA_PIN		P24			//P2_0

#define I2C_SCL_OUTPUT()	(P2DPH = 0x0B)
#define I2C_SCL_INPUT()			
#define I2C_SDA_OUTPUT()	(P2DPH = 0x0B)		
#define I2C_SDA_INPUT()		(P2DPH = 0x09)	
#define I2C_SCL_1()  			P25 = 0x01	//(P2|0x20)	//I2C_SCL_PIN=1		/* SCL = 1 */
#define I2C_SCL_0()  			P25 = 0x00	//(P2&0xEF)	//I2C_SCL_PIN=0		/* SCL = 0 */
#define I2C_SDA_1()  			P24	= 0x01	//P2 = (P2|0x10)	//I2C_SDA_PIN=1		/* SDA = 1 */
#define I2C_SDA_0()  			P24 = 0x00	//P2 = (P2&0xEF)	//I2C_SDA_PIN=0		/* SDA = 0 */
#define I2C_SDA_READ()  	(P24&0x01)	//I2C_SDA_PIN		/* SDA data */


void i2c_GPIO_Config(void);
void i2c_Ack(void);
void i2c_NAck(void);

/*
*********************************************************************************************************
*	? ? ?: i2c_Delay
*	????: I2C?????,??400KHz
*	?    ?:?
*	? ? ?: ?
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
#if 1//defined(MCLK_DIV_1)
	//volatile unsigned short i;

	/* 
	 	???????????AX-Pro???????????
		CPU??72MHz?,???Flash??, MDK?????
		?????10?,SCL?? = 205KHz 
		?????7?,SCL?? = 347KHz, SCL?????1.5us,SCL?????2.87us 
	 	?????5?,SCL?? = 421KHz, SCL?????1.25us,SCL?????2.375us 
        
    IAR???????,?????7
	*/
	//for(i=0; i<10; i++)
	//{}
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();

	//_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	//_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	//_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	//_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	//_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	//_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	//_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	//_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	//_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
#else
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
#endif
}

/*
*********************************************************************************************************
*	? ? ?: i2c_Start
*	????: CPU??I2C??????
*	?    ?:?
*	? ? ?: ?
*********************************************************************************************************
*/
void i2c_Start(void)
{
	/* ?SCL????,SDA?????????I2C?????? */
	
	I2C_SCL_OUTPUT();
	I2C_SDA_OUTPUT();
	
	I2C_SDA_1();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}

/*
*********************************************************************************************************
*	? ? ?: i2c_Start
*	????: CPU??I2C??????
*	?    ?:?
*	? ? ?: ?
*********************************************************************************************************
*/
void i2c_Stop(void)
{
	I2C_SCL_OUTPUT();
	I2C_SDA_OUTPUT();

	/* ?SCL????,SDA?????????I2C?????? */
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
}

/*
*********************************************************************************************************
*	? ? ?: i2c_SendByte
*	????: CPU?I2C??????8bit??
*	?    ?:_ucByte : ???????
*	? ? ?: ?
*********************************************************************************************************
*/
void i2c_SendByte(unsigned char _ucByte)
{
	unsigned char i;

	/* ????????bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		i2c_Delay();
		I2C_SCL_1();
		i2c_Delay();	
		I2C_SCL_0();
		if (i == 7)
		{
			 I2C_SDA_1(); // ????
		}
		_ucByte <<= 1;	/* ????bit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	? ? ?: i2c_ReadByte
*	????: CPU?I2C??????8bit??
*	?    ?:?
*	? ? ?: ?????
*********************************************************************************************************
*/
unsigned char i2c_ReadByte(unsigned char ack)
{
	unsigned char i;
	unsigned char value;

	/* ???1?bit????bit7 */
	I2C_SDA_INPUT();	// set data input	
	i2c_Delay();
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_1();
		i2c_Delay();
		if (I2C_SDA_READ())
		{
			value++;
		}
		//I2C_SCL_1();
		//i2c_Delay();
		I2C_SCL_0();
		i2c_Delay();
	}
	
	I2C_SDA_OUTPUT();	// set data output	
	i2c_Delay();
	if(ack==0)
		i2c_NAck();
	else
		i2c_Ack();
	return value;
}

/*
*********************************************************************************************************
*	? ? ?: i2c_WaitAck
*	????: CPU??????,??????ACK????
*	?    ?:?
*	? ? ?: ??0??????,1???????
*********************************************************************************************************
*/
unsigned char i2c_WaitAck(void)
{
	unsigned char re=0;
	unsigned char count=0;

	I2C_SDA_1();	/* CPU??SDA?? */
	I2C_SDA_INPUT();	//set data input
	i2c_Delay();
	I2C_SCL_1();	/* CPU??SCL = 1, ???????ACK?? */
	i2c_Delay();
	if (I2C_SDA_READ())	/* CPU??SDA???? */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	while((re)&&(count<3))
	{
		re = I2C_SDA_READ();
		count++;
	}
	I2C_SCL_0();
	I2C_SDA_OUTPUT();	//set data input
	i2c_Delay();
	return re;
}

/*
*********************************************************************************************************
*	? ? ?: i2c_Ack
*	????: CPU????ACK??
*	?    ?:?
*	? ? ?: ?
*********************************************************************************************************
*/
void i2c_Ack(void)
{
	I2C_SDA_0();	/* CPU??SDA = 0 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU??1??? */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();	/* CPU??SDA?? */
}

/*
*********************************************************************************************************
*	? ? ?: i2c_NAck
*	????: CPU??1?NACK??
*	?    ?:?
*	? ? ?: ?
*********************************************************************************************************
*/
void i2c_NAck(void)
{
	I2C_SDA_1();	/* CPU??SDA = 1 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU??1??? */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();	
}

/*
*********************************************************************************************************
*	? ? ?: i2c_GPIO_Config
*	????: ??I2C???GPIO,????IO?????
*	?    ?:?
*	? ? ?: ?
*********************************************************************************************************
*/
void i2c_GPIO_Config(void)
{
	i2c_Stop();
}

/*
*********************************************************************************************************
*	? ? ?: i2c_CheckDevice
*	????: ??I2C????,CPU???????,??????????????????
*	?    ?:_Address:???I2C????
*	? ? ?: ??? 0 ????, ??1??????
*********************************************************************************************************
*/
unsigned char i2c_CheckDevice(unsigned char _Address)
{
	unsigned char ucAck;

	//QST_SW_I2C_ADDR_W = _Address;
	qst_sw_set_i2c_addr(_Address);
	i2c_GPIO_Config();		/* ??GPIO */
	i2c_Start();		/* ?????? */
	/* ??????+????bit(0 = w, 1 = r) bit7 ?? */
	i2c_SendByte(_Address|I2C_WR);
	ucAck = i2c_WaitAck();	/* ?????ACK?? */
	i2c_Stop();			/* ?????? */

	return ucAck;
}


unsigned char qst_sw_writereg(unsigned char reg_add,unsigned char reg_dat)
{
	i2c_Start();
	i2c_SendByte(QST_SW_I2C_ADDR_W);
	if(i2c_WaitAck())
	{
		return 0;
	}
	i2c_SendByte(reg_add);	
	if(i2c_WaitAck())
	{
		return 0;
	}
	i2c_SendByte(reg_dat);	
	if(i2c_WaitAck())
	{
		return 0;
	}
	i2c_Stop();

	return 1;
}

unsigned char qst_sw_readreg(unsigned char reg_add,unsigned char *buf,unsigned char num)
{
	//unsigned char ret;
	unsigned char i;

	i2c_Start();
	i2c_SendByte(QST_SW_I2C_ADDR_W);
	if(i2c_WaitAck())
	{
		return 0;
	}
	i2c_SendByte(reg_add);
	if(i2c_WaitAck())
	{
		return 0;
	}

	i2c_Start();
	i2c_SendByte(QST_SW_I2C_ADDR_W+1);		// QST_SW_I2C_ADDR_R
	if(i2c_WaitAck())
	{
		return 0;
	}

	for(i=0;i<(num-1);i++){
		*buf=i2c_ReadByte(1);
		buf++;
	}
	*buf=i2c_ReadByte(0);
	i2c_Stop();

	return 1;
}

void qst_sw_set_i2c_addr(unsigned char addr)
{
	QST_SW_I2C_ADDR_W = addr;
	QST_SW_I2C_ADDR_R = addr|0x01;
}
#endif

#if defined(QST_SW_IIC_2)
#define QMAX981_SLAVEADDR_W	0x24		// 0x26
#define QMAX981_SLAVEADDR_R	0x25		// ox27

#define MS_CLK_PIN_GPIO_MODE		(P2DPH = 0x0B)
#define	MS_DATA_PIN_GPIO_MODE		(P2DPH = 0x0B)
#define MS_I2C_CLK_OUTPUT			(P2DPH = 0x0B)
#define MS_I2C_DATA_OUTPUT			(P2DPH = 0x0B)
#define MS_I2C_DATA_INPUT		   	(P2DPH = 0x09)
#define MS_I2C_CLK_HIGH				P25 = 0x01
#define MS_I2C_CLK_LOW				P25 = 0x00
#define MS_I2C_DATA_HIGH			P24	= 0x01
#define MS_I2C_DATA_LOW				P24 = 0x00
#define MS_I2C_GET_BIT				(P24&0x01)

void SW_i2c_udelay(u32 delay)
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}

static void SW_i2c_start(void)
{
	MS_CLK_PIN_GPIO_MODE;
	MS_I2C_CLK_OUTPUT;

	MS_DATA_PIN_GPIO_MODE;
	MS_I2C_DATA_OUTPUT;
	
	MS_I2C_DATA_HIGH;
	MS_I2C_CLK_HIGH;
	SW_i2c_udelay(40);		//20
	MS_I2C_DATA_LOW;
	SW_i2c_udelay(20);		//10
	MS_I2C_CLK_LOW;
	SW_i2c_udelay(20);		//10
}

/******************************************
	software I2C stop bit
*******************************************/
static void SW_i2c_stop(void)
{
	MS_I2C_CLK_OUTPUT;
	MS_I2C_DATA_OUTPUT;
	
	SW_i2c_udelay(20);		//10
	MS_I2C_CLK_HIGH;
	SW_i2c_udelay(20);		//10
	MS_I2C_DATA_HIGH;
}

/******************************************
	software I2C one clock
*******************************************/
static void SW_i2c_one_clk(void)
{
	SW_i2c_udelay(10);		//5
	MS_I2C_CLK_HIGH;
	SW_i2c_udelay(20);		//10
	MS_I2C_CLK_LOW;
	SW_i2c_udelay(10);		//5
}

/******************************************
	software I2C read byte with ack
*******************************************/
static u8 ms_ReadByteAck(void)
{
	s8 i;
	u8 udata;

	MS_I2C_DATA_INPUT; 
	udata = 0; 
	
	for (i=7; i>=0; i--) 
	{
		if (MS_I2C_GET_BIT)
		{
			udata |= (0x01<<i);
		}
		SW_i2c_one_clk();
	}			                                

	MS_I2C_DATA_OUTPUT;                    
	MS_I2C_DATA_LOW;                       
	SW_i2c_one_clk();                         

	return udata;
}

/******************************************
	software I2C read byte without ack
*******************************************/
static u8 ms_ReadByteNAck(void)
{
	s8 i;
	u8 udata;

	MS_I2C_DATA_INPUT; 
	udata = 0; 
	
	for (i=7; i>=0; i--) 
	{
		if (MS_I2C_GET_BIT)
		{
			udata |= (0x01<<i);
		}
		SW_i2c_one_clk();
	}			                                

	MS_I2C_DATA_OUTPUT;                                           
	MS_I2C_DATA_HIGH;
	SW_i2c_one_clk();                         
	
	return udata;
}

/******************************************
	software I2C send byte
*******************************************/
static void ms_SendByte(u8 sData) 
{
	s8 i;
	MS_I2C_DATA_OUTPUT;       
	for (i=7; i>=0; i--) 
	{            
		if ((sData>>i)&0x01) 
		{               
			MS_I2C_DATA_HIGH;	              
		} 
		else 
		{ 
			MS_I2C_DATA_LOW;                  
		}
		SW_i2c_one_clk();                        
	}		
}
/******************************************
	software I2C check ack bit
*******************************************/
static u8 ms_ChkAck(void)//
{
	unsigned char ack;
	unsigned char count = 0;

	MS_I2C_DATA_INPUT;
	SW_i2c_udelay(10);		//5
	MS_I2C_CLK_HIGH;
	SW_i2c_udelay(10);		//5

#if 0
	if(MS_I2C_GET_BIT)		//Non-ack
	{
		SW_i2c_udelay(10);	//5
		MS_I2C_CLK_LOW;
		SW_i2c_udelay(10);	//5
		MS_I2C_DATA_OUTPUT;
		MS_I2C_DATA_LOW;
		
		return 0;
	}
	else					//Ack
	{
		SW_i2c_udelay(10);	//5
		MS_I2C_CLK_LOW;
		SW_i2c_udelay(10);	//5
		MS_I2C_DATA_OUTPUT;
		MS_I2C_DATA_LOW;

		return 1;
	}
#else
	ack = MS_I2C_GET_BIT;
	while(ack && (count<5))
	{
		count++;
		ack = MS_I2C_GET_BIT;
	}
	
	SW_i2c_udelay(10);	//5
	MS_I2C_CLK_LOW;
	SW_i2c_udelay(10);	//5
	MS_I2C_DATA_OUTPUT;
	MS_I2C_DATA_LOW;

	if(ack)
	{
		return 0;
	}
	else
	{
		return 1;
	}
#endif
}

/******************************************
	software I2C restart bit
*******************************************/
static void ms_Restart(void)
{
	MS_I2C_CLK_OUTPUT;
	MS_I2C_DATA_OUTPUT;

	SW_i2c_udelay(40);
	MS_I2C_DATA_HIGH;
	SW_i2c_udelay(20);		//10
	MS_I2C_CLK_HIGH;
	SW_i2c_udelay(40);
	MS_I2C_DATA_LOW;
	SW_i2c_udelay(20);		//10
	MS_I2C_CLK_LOW;
	SW_i2c_udelay(20);		//10
}


unsigned char qst_sw_readreg(unsigned char reg_add,unsigned char *buf,unsigned char num)
{
	u8* Data_ptr;
	u16 i;

	Data_ptr = buf;
	
	SW_i2c_start();						//start bit
	ms_SendByte(QMAX981_SLAVEADDR_W);		//slave address|write bit
	if(0 == ms_ChkAck())		//check Ack bit
	{
		//TO_DO: display ack check fail when send write id		
		SW_i2c_stop();
		return 0;
	}
		
	ms_SendByte(reg_add);				//send RegAddr
	if(0 == ms_ChkAck())		//check Ack bit
	{
		//TO_DO: display ack check fail when send RegAddr		
		SW_i2c_stop();
		return 0;
	}

	ms_Restart();						//restart bit

	ms_SendByte(QMAX981_SLAVEADDR_R);		//slave address|read bit
	if(0 == ms_ChkAck())
	{
		//TO_DO: display ack check fail when send read id		
		SW_i2c_stop();
		return 0;
	}

	for(i=num; i>1; i--)
	{
		*Data_ptr = ms_ReadByteAck();	//read byte with ack
		Data_ptr++;
	}
	
	*Data_ptr = ms_ReadByteNAck();		//read byte with non-ack to stop reading

	SW_i2c_stop();						//stop bit

	//TO_DO: add debug code to display the data received

	return 1;
}


unsigned char  qst_sw_writereg(unsigned char reg_add,unsigned char reg_dat)
{
	SW_i2c_start();						//start bit

	ms_SendByte(QMAX981_SLAVEADDR_W);		//slave address|write bit
	if(0 == ms_ChkAck())		//check Ack bit
	{
		//TO_DO: display check ack fail when send write id
		SW_i2c_stop();
		return 0;
	}

	ms_SendByte(reg_add);				//send RegAddr
	if(0 == ms_ChkAck())		//check Ack bit
	{
		//TO_DO: display check ack fail when send RegAddr
		SW_i2c_stop();
		return 0;
	}

	ms_SendByte(reg_dat);					//send parameter
	if(0 == ms_ChkAck())
	{
		//TO_DO: display check ack fail when send data
		SW_i2c_stop();
		return 0;
	}

	SW_i2c_stop();						//stop bit

	return 1;
}

unsigned char i2c_CheckDevice(unsigned char _Address)
{
	return 0;
}
#endif

