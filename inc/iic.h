 /*
*********************************************************
*文 件 名：rb57mxx5a.h
*功能描述：头文件
*作    者：tate
*时    间：2016.12.08
*版    本：V0.0.3
*********************************************************
*/

#ifndef _IIC_H
#define _IIC_H

//#include "variable.h"

#define  START_STATE      0x08
#define  R_START_STATE    0x10
#define  SLA_W_ASTATE     0x18
#define  SLA_W_NASTATE    0x20
#define  DATA_W_ASTATE    0x28
#define  DATA_W_NASTATE   0x30
#define  SLA_R_ASTATE     0x40
#define  SLA_R_NASTATE    0x48
#define  DATA_R_ASTATE    0x50
#define  DATA_R_NASTATE   0x58
#define  S_SlAW_ASTATE    0x60
#define  MS_SlAW_ASTATE   0x68
#define  GC_ADD_ASTATE    0x70
#define  MGC_ADD_ASTATE   0x78
#define  S_DATAR_ASTATE   0x80
#define  S_DATAR_NASTATE  0x88
#define  GC_DATAR_ASTATE  0x90
#define  GC_DATAR_NASTATE 0x98
#define  SLA_STOP_STATE   0xA0
#define  S_SlAR_ASTATE    0xA8
#define  MS_SlAR_ASTATE   0xB0
#define  S_DATAW_ASTATE   0xB8
#define  S_DATAW_NASTATE  0xC0
#define  LS_DATAW_ASTATE  0xC8


extern void MIIC_Init(u8 Iomux3_data,u8 Clk_Cfg);
extern void MIIC_Write(u8 Slave_Address, u8 Reg_address, u8 *W_Data, u8 number);
extern void MIIC_Read(u8 Slave_Address, u8 Reg_address, u8 *R_Data, u8 number);
extern void SIIC_Init(u8 Slave_Address);
//extern void SIIC_Write(u8 *W_Data);
//extern void SIIC_Read(u8 *R_Data);
extern void MIIC_Read_Current(u8 Slave_Address, u8 *R_Data, u8 number);
#endif
