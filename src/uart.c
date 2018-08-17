/*
*********************************************************
*�� �� ����uart
*����������uartģ��ʹ��
*��    �ߣ�PDY
*ʱ    �䣺2014.12.10
*��    ����V1.0.0
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
*�� �� ����uart_config
*����������uartģ�����ã�F=4M,������19200
*��    �룺��
*��    ������
* baudrate = (2^SMOD/32)*(1/(256-N))*PCLK
	baudrate(3M) = 3*1000*1000/16*10 = 18750
	baudrate(6M) = 6*1000*1000/16*10 = 37500
	baudrate(12M) = 12*1000*1000/16*13 = 57692.307692307692307692307692308
**********************************************************
*/
void uart_config(void)
{	
	//ES = 0; //���ж�
// io config
	P3DPL = 0x56; //P3.0(TX)�������  P3.1(RX)��������
// io config
	IOMUXC0 |= 0x40;  //UART���������
	SCON = 0x44;    //����ģʽ1,8λ�첽�������ʿɵ�
	BRCON = 0xF0;   //�����ʷ�����������SMOD=1��F0
	#if defined(MCLK_DIV_1)
	BRTIM = 256-13;
	#else
	BRTIM = 256-10;
	#endif

	REN = 1;//�����������ݽ���
	RI = 0;
	ES = 1; //���ж�
}


/*
**********************************************************
*�� �� ����send_str
*��������������һ���ַ�
*��    �룺*s��Ҫ���͵����ݵ��׵�ַ
*��    ������
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
*�� �� ����UartInterrupt
*�����������жϷ�����,�����ݷ��뻺������receivebuff[]��
*��    �룺��
*��    ������
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
//���������ݳ���20����һ֡���ݽ�����־��oxod oxoa��
//�����س����У�����ʾ���ݽ������
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

