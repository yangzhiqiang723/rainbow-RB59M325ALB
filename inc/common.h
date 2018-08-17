
#ifndef _COMMON_H
#define _COMMON_H

//#define USE_SW_IIC

#define MCLK_DIV_1
//#define MCLK_DIV_2
//#define MCLK_DIV_4
//#define MCLK_DIV_16

void uart_config(void);
void uart_print(unsigned char *s);
void uart_r_string(void);

//void MIIC_Init(u8 Iomux3_data, u8 Clk_Cfg);
//void MIIC_Write(u8 Slave_Address, u8 Reg_address, u8 *W_Data, u8 number);
//void MIIC_Read(u8 Slave_Address, u8 Reg_address, u8 *R_Data, u8 number);

#endif
