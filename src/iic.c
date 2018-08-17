/*
*********************************************************
*�� �� ����IIC.c
*����������
*��    �ߣ�
*ʱ    �䣺2016.12.12
*��    ����V1.0.0
*ע    �⣺���жϱ�־λSI��������¼���������
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
*�� �� ����IIC��ʼ��  ����
*����������
*��    �룺��
*��    ������
**********************************************************
*/
void MIIC_Init(u8 Iomux3_data, u8 Clk_Cfg)
{
    //  P2DPH |= 0x0f;     //����I2C�ӿ�   P2.5(SCL) P2.4(SDA)Ϊ��©���   ������λ����һ������  ����Ϊ��������
    //    IOMUXC3 = 0x20;      //����I2C�ܽŸ��ã����� P2.5(SCL) P2.4(SDA)��
    IOMUXC3 = Iomux3_data;  //����I2C�ܽŸ��ã�30���� P3.1(SCL) P3.0(SDA)��  20���� P2.5(SCL) P2.4(SDA)��
    I2CCON = Clk_Cfg;      //60��Ƶ  IICʱ��
    I2CEN = 1;             //ʹ��I2Cģ��
    EIE1 |= 0X02;          //����I2C���ж�

}
/*
**********************************************************
*�� �� ����IICд����  ����
*����������
*��    �룺Slave_Address �豸��ַ(�Ѿ�������һλ)  Reg_address �Ĵ�����ַ  W_Data д����  number ����
*��    ������
**********************************************************
*/
void MIIC_Write(u8 Slave_Address, u8 Reg_address, u8 *W_Data, u8 number)
{
    u8 i = 0;
    STO = 0;
    STA = 1;                              //start �ź�
    I2C_FLAG = 0;
    while(I2C_FLAG != START_STATE);           //�ж�start�ź��Ƿ������
    I2CDAT = Slave_Address;                   //д�豸��ַ + W
    I2C_FLAG = 0;
    SI = 0;
    while(I2C_FLAG != SLA_W_ASTATE);           //�ж��豸��ַ��д�ź��Ƿ������
    I2CDAT = Reg_address;                      //д�Ĵ�������
    I2C_FLAG = 0;
    STA = 0;
    SI = 0;
    while(I2C_FLAG != DATA_W_ASTATE);           //�жϼĴ��������Ƿ������
    for(i = 0; i < number; i++)
    {
        I2CDAT = W_Data[i];                     //д����
        //       I2CSTAR = 0;������ӵ�Ƶ���ֻ������⣬I2C���߲��ܿ���
        I2C_FLAG = 0;
        SI = 0;
        while(I2C_FLAG != DATA_W_ASTATE);       //�ж������Ƿ������
        if(i == number - 1)
        {
            //            I2CSTAR = 0;
            STO  = 1;                        //дstop
            SI = 0;
        }
    }
}
/*
**********************************************************
*�� �� ����IIC�����������ַ  ����
*����������
*��    �룺Slave_Address �豸��ַ(�Ѿ�������һλ) Reg_address �Ĵ�����ַ  number ����
*��    ����*R_Data  ��ȡ����
**********************************************************
*/
void MIIC_Read(u8 Slave_Address, u8 Reg_address, u8 *R_Data, u8 number)
{
    u8 i = 0;
    STO = 0;
    STA = 1;                              //start �ź�
    I2C_FLAG = 0;
    while(I2C_FLAG != START_STATE);           //�ж�start�ź��Ƿ������
    I2CDAT = Slave_Address;                   //д�豸��ַ + W
    I2C_FLAG = 0;
    SI = 0;
    while(I2C_FLAG != SLA_W_ASTATE);          //�ж��豸��ַ��д�ź��Ƿ������
    I2CDAT = Reg_address;                     //д�Ĵ�����ַ
    I2C_FLAG = 0;
    STA = 0;
    SI = 0;
    while(I2C_FLAG != DATA_W_ASTATE);         //�ж��豸�Ĵ�����ַ�Ƿ������
    STA = 1;                              //�ظ� start �ź�
    I2C_FLAG = 0;
    SI = 0;
    while(I2C_FLAG != R_START_STATE);
    I2CDAT = Slave_Address | 0x01;           //д�豸��ַ R
    I2C_FLAG = 0;
    SI = 0;
    while(I2C_FLAG != SLA_R_ASTATE);         //�ж��豸��ַ R�Ƿ������
    STA = 0;
    /************�޸Ķ�ȡһ������ʱ��**************/
	if(number > 1)
	AA = 1;
	else
    AA = 0;
    /************�޸Ķ�ȡһ������ʱ��**************/
    SI = 0;
    I2C_FLAG = 0;
    for(i = 0; i < number; i++)
    {
        if(i < number - 1)
        {
            while(I2C_FLAG != DATA_R_ASTATE);  //�ж������Ƿ�������
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
            while(I2C_FLAG != DATA_R_NASTATE); //�ж������Ƿ������һ��
            R_Data[i] = I2CDAT;
            STA = 0;
            STO  =  1;                      //дstop
            SI = 0;
        }

    }
}
/*
**********************************************************
*�� �� ����IIC��������ǰ����  ����
*����������
*��    �룺Slave_Address �豸��ַ(�Ѿ�������һλ)  number ����
*��    ����*R_Data  ��ȡ����
**********************************************************
*/
//void MIIC_Read_Current(u8 Slave_Address, u8 *R_Data, u8 number)
//{
//    u8 i = 0;
//    STO = 0;
//    STA= 1;                              //start �ź�
//    I2C_FLAG = 0;
//    while(I2C_FLAG != START_STATE);           //�ж�start�ź��Ƿ������
//    I2CDAT = Slave_Address;           //д�豸��ַ R
//    I2C_FLAG = 0;
//    SI = 0;
//    while(I2C_FLAG != SLA_R_ASTATE);         //�ж��豸��ַ R�Ƿ������
//    STA = 0;
    /***********�޸Ķ�ȡһ������ʱ��***************/
//		if(number > 1)
//		AA = 1;
//		else
//	    AA = 0;
    /************�޸Ķ�ȡһ������ʱ��**************/
//    SI = 0;
//    I2C_FLAG = 0;
//    for(i = 0; i < number; i++)
//    {
//        if(i < number - 1)
//        {
//            while(I2C_FLAG != DATA_R_ASTATE);  //�ж������Ƿ�������
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
//            while(I2C_FLAG != DATA_R_NASTATE); //�ж������Ƿ������һ��
//            R_Data[i] = I2CDAT;
//            STA = 0;
//            STO  =  1;                      //дstop
//            SI = 0;
//        }
//
//    }
//}
/*
**********************************************************
*�� �� ����IIC��ʼ��  �ӻ�
*����������
*��    �룺��
*��    ������
**********************************************************
*/
void SIIC_Init(u8 Slave_Address)
{
//    P2DPH |= 0x05;           //����I2C�ӿ�   P2.5(SCL) P2.4(SDA)Ϊ��������
    IOMUXC3 = 0x20;         //����I2C�ܽŸ��ã����� P2.5(SCL) P2.4(SDA)��
    I2CADR = Slave_Address | 0x01 ;
    AA = 1;
    I2CEN = 1;
    EIE1 |= 0X02;           //����I2C���ж�
//    EA = 1;
    //    I2CSAR = Slave_Address >> 1; //���ôӻ���ַ
    //I2CCTR = 0x19;          //ʹ��I2Cģ�飬��I2C�ж�,�ӻ�ģʽ
}
/*
**********************************************************
*�� �� ����IICд����  �ӻ�
*����������
*��    �룺W_Data д����
*��    ������
**********************************************************
*/
//void SIIC_Write(u8 *W_Data)
//{

//}
/*
**********************************************************
*�� �� ����IIC������  �ӻ�
*����������
*��    �룺Slave_Address �豸��ַ(�Ѿ�������һλ)    number ����
*��    ����*R_Data  ��ȡ����
**********************************************************
*/
//void SIIC_Read(u8 *R_Data)
//{

//}

/*
**********************************************************
*�� �� ����IIC�жϷ�����
*����������
*��    �룺
*��    ����
**********************************************************
*/
void I2CInterrupt(void) interrupt 7  //�жϷ�����
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
    case S_SlAR_ASTATE://�ж��������͵ĵ�ַ�Ͷ��ź��Ƿ�ƥ�䣬�ӻ�д
        I2CDAT = RW_Data[i];
        i++;
        I2C_FLAG = 0;
        AA = 1;
        SI = 0;
        break;
    case S_DATAW_ASTATE: //�ӻ�д����
        I2CDAT = RW_Data[i];
        I2C_FLAG = 0;
        STO = 0;
        AA = 1;
        i++;
        SI = 0;
        break;
    case LS_DATAW_ASTATE:  //�ӻ�д���һ������
        I2CDAT = RW_Data[i];
        I2C_FLAG = 0;
        AA = 1;
        STA = 0;
        i = 0;
        SI = 0;
        break;
    case S_DATAW_NASTATE: //�ӻ�д���ݷ�����ɣ����յ�NACK
        I2C_FLAG = 0;
        AA = 1;
        STA = 0;
		    i = 0;
        SI = 0;
        break;
    case S_SlAW_ASTATE:   //�ж��������͵ĵ�ַ��д�ź��Ƿ�ƥ�� �ӻ���
        AA = 1;
        I2C_FLAG = 0;
        SI = 0;
        break;
    case S_DATAR_ASTATE:  //�ӻ������յ����ݺ�ACK�ź�
        SR_Date[i] = I2CDAT;
        AA = 1 ;
        I2C_FLAG = 0;
        SI = 0;
        i++;
        break;
    case SLA_STOP_STATE: //�ӻ������յ�stop�źţ�������������
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
