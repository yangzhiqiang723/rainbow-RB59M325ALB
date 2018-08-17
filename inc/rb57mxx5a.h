 /*
*********************************************************
*文 件 名：rb57mxx5a.h
*功能描述：头文件
*作    者：tate
*时    间：2017.1.12
*版    本：V0.1.0
*********************************************************
*/

#ifndef _rb57mxx5a_H
#define _rb57mxx5a_H

//#define assembly

#ifndef assembly
typedef volatile unsigned char xdata vxu8;
#endif

/****************************
*sys
*****************************/
sfr PCON   = 0x87;
sfr RSTFLG = 0xEF;	 
sfr SP     = 0x81;
sfr DPL    = 0x82;
sfr DPH    = 0x83;
sfr DPSEL  = 0x84;
sfr RIADRH = 0x85;
sfr PSW    = 0xD0;
sfr ACC    = 0xE0;							  
sfr B      = 0xF0;
#ifdef assembly
  SYSCFG    EQU    0FF00H
  PDCFG     EQU    0FF02H
#else
  #define SYSCFG  (*(vxu8 *)0xFF00)
  #define PDCFG   (*(vxu8 *)0xFF02)
#endif


/****************************
*INTC
*****************************/
sfr IE       = 0xA8;
sfr IP       = 0xB8;
sfr EIE1     = 0xE6;
sfr EIE2     = 0xF6;
sfr EIP1     = 0xE7;
sfr EIP2     = 0xF7;
sfr INTCON0 = 0xE4;
sfr INTCON1 = 0xE5;

sbit EA  = IE^7;
sbit ET2 = IE^5;
sbit ES  = IE^4;
sbit ET1 = IE^3;
sbit EX1 = IE^2;
sbit ET0 = IE^1;
sbit EX0 = IE^0;

sbit PT2 = IP^5;
sbit PS  = IP^4;
sbit PT1 = IP^3;
sbit PX1 = IP^2;
sbit PT0 = IP^1;
sbit PX0 = IP^0;

/****************************
*CKMU
*****************************/
sfr CKEN   = 0xB1;
sfr CKSS   = 0xB2;
sfr CKDS   = 0xB4;
sfr IHRCTD   = 0xB3;
sfr IHRCCON  = 0xB5;
sfr IHRCTD1  = 0XB6;
sfr SPKEY  = 0xB7;

#ifdef assembly
  PCGCFG0    EQU    0FF20H
  PCGCFG1    EQU    0FF21H
  CKOCFG    EQU    0FF3FH
#else
  #define PCGCFG0  (*(vxu8 *)0xFF20)
  #define PCGCFG1  (*(vxu8 *)0xFF21)
  #define CKOCFG  (*(vxu8 *)0xFF3F)
#endif


/****************************
*PMU
*****************************/
sfr  LVDCON = 0xED;
sfr  BORCON = 0xEE;
sfr  VBGTD  = 0xD1;
/****************************
*TS
*****************************/
sfr TSCON = 0xD2;
sfr TSSTA = 0xD3;
sfr CHENL = 0xD4;
sfr CHENH = 0xD5;
sfr TSDATH = 0xD7;
sfr TSDATL = 0xD6;
#ifdef assembly
  TSWDT   EQU    0FF45H	
  TSCNT0  EQU    0FF46H
  TSCNT1  EQU    0FF47H
  TSVAL   EQU    0FF57H
  TSEXCNT EQU    0FF4FH
  CH0SR   EQU    0FF88H
  CH1SR   EQU    0FF89H
  CH2SR   EQU    0FF8AH
  CH3SR   EQU    0FF8BH
  CH4SR   EQU    0FF8CH
  CH5SR   EQU    0FF8DH
  CH6SR   EQU    0FF8EH
  CH7SR   EQU    0FF8FH
  CH8SR   EQU    0FF90H
  CH9SR   EQU    0FF91H
  CH10SR   EQU    0FF92H
  CH11SR   EQU    0FF93H
  CH12SR   EQU    0FF94H
  CH13SR   EQU    0FF95H
  CH14SR   EQU    0FF96H
  CH15SR   EQU    0FF97H
#else
  #define TSWDT  (*(vxu8 *)0xFF45)
  #define TSCNT0 (*(vxu8 *)0xFF46)
  #define TSCNT1 (*(vxu8 *)0xFF47)
  #define TSVAL  (*(vxu8 *)0xFF57)
  #define TSEXCNT (*(vxu8 *)0xFF4F)
  #define CH0SR  (*(vxu8 *)0xFF88)
  #define CH1SR  (*(vxu8 *)0xFF89)
  #define CH2SR  (*(vxu8 *)0xFF8A)
  #define CH3SR  (*(vxu8 *)0xFF8B)
  #define CH4SR  (*(vxu8 *)0xFF8C)
  #define CH5SR  (*(vxu8 *)0xFF8D)
  #define CH6SR  (*(vxu8 *)0xFF8E)
  #define CH7SR  (*(vxu8 *)0xFF8F)
  #define CH8SR  (*(vxu8 *)0xFF90)
  #define CH9SR  (*(vxu8 *)0xFF91)
  #define CH10SR  (*(vxu8 *)0xFF92)
  #define CH11SR  (*(vxu8 *)0xFF93)
  #define CH12SR  (*(vxu8 *)0xFF94)
  #define CH13SR  (*(vxu8 *)0xFF95)
  #define CH14SR  (*(vxu8 *)0xFF96)
  #define CH15SR  (*(vxu8 *)0xFF97)
#endif
/****************************
*IO PORT
*****************************/
sfr P0  =  0x80;
sfr P1  =  0x90;
sfr P2  =  0xA0;
sfr P3  =  0xB0;
sfr P4  =  0xD8;
sfr P5  =  0xF8;
sfr P6  =  0xe1;

sfr P0DPL  =  0xA4;
sfr P1DPL  =  0xA5;
sfr P2DPL  =  0xA6;
sfr P3DPL  =  0xA7;
sfr P0DPH  =  0xF1;
sfr P1DPH  =  0xF2;
sfr P2DPH  =  0xF3;
sfr P3DPH  =  0xF4;
sfr P4DPL  =  0xD9;
sfr P4DPH  =  0xDA;
sfr P5DPL  =  0xF9;
sfr P5DPH  =  0xFA;
sfr P6DPL  =  0xFB;
sfr P6DPH  =  0xFC;

#ifdef assembly
  P0AMEN   EQU    0FF30H	
  P1AMEN   EQU    0FF31H
  P2AMEN   EQU    0FF32H
  P3AMEN   EQU    0FF33H
  P4AMEN   EQU    0FF34H
  P5AMEN   EQU    0FF35H  
  
  IOMUXC0 EQU    0FF48H
  IOMUXC1 EQU    0FF49H
  IOMUXC2 EQU    0FF4AH
  IOMUXC3 EQU   0FF4BH
  IOMUXC4 EQU   0FF4CH
#else
  #define P0AMEN  (*(vxu8 *)0xFF30)
  #define P1AMEN  (*(vxu8 *)0xFF31)
  #define P2AMEN  (*(vxu8 *)0xFF32)
  #define P3AMEN  (*(vxu8 *)0xFF33)
  #define P4AMEN  (*(vxu8 *)0xFF34)
  #define P5AMEN  (*(vxu8 *)0xFF35)
  #define IOMUXC0 (*(vxu8 *)0xFF48)
  #define IOMUXC1 (*(vxu8 *)0xFF49)
  #define IOMUXC2 (*(vxu8 *)0xFF4A)
  #define IOMUXC3 (*(vxu8 *)0xFF4B)
  #define IOMUXC4 (*(vxu8 *)0xFF4C)
#endif

sbit P00 = P0^0;
sbit P01 = P0^1;
sbit P02 = P0^2;
sbit P03 = P0^3;
sbit P04 = P0^4;
sbit P05 = P0^5;
sbit P06 = P0^6;
sbit P07 = P0^7;

sbit P10 = P1^0;
sbit P11 = P1^1;
sbit P12 = P1^2;
sbit P13 = P1^3;
sbit P14 = P1^4;
sbit P15 = P1^5;
sbit P16 = P1^6;
sbit P17 = P1^7;

sbit P20 = P2^0;
sbit P21 = P2^1;
sbit P22 = P2^2;
sbit P23 = P2^3;
sbit P24 = P2^4;
sbit P25 = P2^5;
sbit P26 = P2^6;
sbit P27 = P2^7;

sbit P30 = P3^0;
sbit P31 = P3^1;
sbit P32 = P3^2;
sbit P33 = P3^3;
sbit P34 = P3^4;
sbit P35 = P3^5;
sbit P36 = P3^6;
sbit P37 = P3^7;


sbit P40 = P4^0;
sbit P41 = P4^1;
sbit P42 = P4^2;
sbit P43 = P4^3;
sbit P44 = P4^4;
sbit P45 = P4^5;
sbit P46 = P4^6;
sbit P47 = P4^7;

sbit P50 = P5^0;
sbit P51 = P5^1;
sbit P52 = P5^2;
sbit P53 = P5^3;
sbit P54 = P5^4;
sbit P55 = P5^5;
sbit P56 = P5^6;
sbit P57 = P5^7;



/****************************
*wdt
*****************************/
sfr  WDTEN0  =  0xFD;
sfr  WDTEN1  =  0xFE;
sfr  WDTCON  =  0xFF;

/****************************
*T0/T1
*****************************/
sfr TCON = 0x88;
sfr TMOD = 0x89;
sfr TL0  = 0x8A;
sfr TL1  = 0x8B;
sfr TH0  = 0x8C;
sfr TH1  = 0x8D;
#ifdef assembly
  TIMPRS    EQU    0FF1BH
  TIMOEN    EQU    0FF1EH
#else
  #define TIMPRS  (*(vxu8 *)0xFF1B)
  #define TIMOEN   (*(vxu8*)0xFF1E)
#endif


sbit TF1 = TCON^7;
sbit TR1 = TCON^6;
sbit TF0 = TCON^5;
sbit TR0 = TCON^4;
sbit IE1 = TCON^3;
sbit IT1 = TCON^2;
sbit IE0 = TCON^1;
sbit IT0 = TCON^0;

/****************************
*T2
*****************************/
sfr T2CON1 = 0xC8;
sfr RCAP2L = 0xCA;
sfr RCAP2H = 0xCB;
sfr TL2    = 0xCC;
sfr TH2    = 0xCD;
sfr T2DH   = 0xCE;
sfr T2DL   = 0xCF;
#ifdef assembly
  T2CON2    EQU    0FF1CH
  T2DEAD    EQU    0FF1DH
#else
  #define T2CON2  (*(vxu8 *)0xFF1C)
  #define T2DEAD  (*(vxu8 *)0xFF1D)
#endif
sbit TF2   = T2CON1^7;
sbit EXF2  = T2CON1^6;

sbit FCPEN = T2CON1^3;
sbit TR2   = T2CON1^2;
sbit CT2   = T2CON1^1;
sbit CPRL2 = T2CON1^0;

/****************************
*UART
*****************************/
sfr SCON = 0x98;
sfr SBUF = 0x99;
sfr SCON1= 0x9A;
sfr SBUF1 = 0x9B;
sfr BRCON = 0x8E;
sfr BRCON1= 0x96;
sfr BRTIM = 0x8F;
sfr BRTIM1= 0x97;

sbit SM0 = SCON^7;
sbit SM1 = SCON^6;
sbit SM2 = SCON^5;
sbit REN = SCON^4;
sbit TB8 = SCON^3;
sbit RB8 = SCON^2;
sbit TI  = SCON^1;
sbit RI  = SCON^0;

/****************************
*I2C
*****************************/
sfr I2CCON = 0XC0;
sfr I2CDAT = 0XC1;
sfr I2CADR = 0XC2;
sfr I2CSTA = 0XC3;

sbit  CR0 = I2CCON^0;
sbit  CR1 = I2CCON^1;
sbit  AA = I2CCON^2;
sbit  SI = I2CCON^3;
sbit  STO = I2CCON^4;
sbit  STA = I2CCON^5;
sbit  I2CEN = I2CCON^6;
sbit  CR2 = I2CCON^7;




/****************************
*SPI
*****************************/
sfr SPICON = 0xDC;
sfr SPISTA = 0xDD;
sfr	SPIDAT = 0xDE;


/****************************
*keyin
*****************************/
sfr KINFLG = 0xAF;

#ifdef assembly
  KINEN0    EQU    0FF2BH
  KINEN1    EQU    0FF29H
  KINPS0    EQU    0FF2CH
  KINPS1    EQU    0FF2AH
#else
  #define KINEN0  (*(vxu8 *)0xFF2B)
  #define KINEN1  (*(vxu8 *)0xFF29)
  #define KINPS0  (*(vxu8 *)0xFF2C)
  #define KINPS1  (*(vxu8 *)0xFF2A)
#endif


//#ifdef rb57p165a
/****************************
*PWM
*****************************/
  sfr PWMP = 0x93;
  sfr PWMD = 0x95;
  #ifdef assembly
  PWMCON    EQU    0FF80H
  PWMPSC    EQU    0FF86H
  #else
  #define PWMCON  (*(vxu8 *)0xFF80)
  #define PWMPSC  (*(vxu8 *)0xFF86)
  #endif

//#else
/****************************
*RTC
*****************************/
sfr WTCON = 0xC9;

  #ifdef assembly
  WTCNT    EQU    0FF0BH
  #else
  #define WTCNT  (*(vxu8 *)0xFF0B)
  #endif
//#endif
//
//#ifndef  rb57p165a
//
//
//#endif
/****************************
*adc
*****************************/
sfr ADCON = 0xE8;
sfr ADRL  = 0xE9;
sfr ADRH  = 0xEA;

sbit ADCHS3 = ADCON^7;
sbit ADCHS2 = ADCON^6;
sbit ADCHS1 = ADCON^5;
sbit ADCHS0 = ADCON^4;
sbit ADEN = ADCON^3;
sbit ADGO = ADCON^2;
sbit EOC  = ADCON^2;
sbit ADIF = ADCON^1;
sbit ADFM = ADCON^0;

#ifdef assembly
  ADCFG    EQU    0FF3EH
  ADTRG    EQU    0FF4EH

#else
  #define ADCFG  (*(vxu8 *)0xFF3E)
  #define ADTRG  (*(vxu8 *)0xFF4E)
 
#endif


/****************************
*CMP
*****************************/
sfr CPCON = 0x9E;
sfr VRCON = 0x9F; 
sfr CPSTA = 0X9D;
/****************************
*LCD
*****************************/
#ifdef assembly
  LCDCON    EQU    0FF50H
  LCDPCON	EQU    0FF51H
  TKCON     EQU    0FF52H
  TKADDR    EQU    0FF53H
  LCDM0     EQU    0FF60H
  LCDM1     EQU    0FF61H
  LCDM2     EQU    0FF62H
  LCDM3     EQU    0FF63H
  LCDM4     EQU    0FF64H
  LCDM5     EQU    0FF65H
  LCDM6     EQU    0FF66H
  LCDM7     EQU    0FF67H
  LCDM8     EQU    0FF68H
  LCDM9     EQU    0FF69H
  LCDM10    EQU    0FF6AH
  LCDM11    EQU    0FF6BH
  LCDM12    EQU    0FF6CH
  LCDM13    EQU    0FF6DH
  LCDM14    EQU    0FF6EH
  LCDM15    EQU    0FF6FH
  LCDM16    EQU    0FF70H
  LCDM17    EQU    0FF71H
  LCDM18    EQU    0FF72H
  LCDM19    EQU    0FF73H
  LCDM20    EQU    0FF74H
  LCDM21    EQU    0FF75H
  LCDM22    EQU    0FF76H
  LCDM23    EQU    0FF77H
  LCDM24    EQU    0FF78H
  LCDM25    EQU    0FF79H
  LCDM26    EQU    0FF7AH
  LCDPC0    EQU	   0FF40H
  LCDPC1    EQU	   0FF41H
  LCDPC2    EQU	   0FF42H
  LCDPC3    EQU	   0FF43H
  LCDPC4    EQU	   0FF44H

#else
  #define LCDCON   (*(vxu8 *)0xFF50)
  #define LCDPCON  (*(vxu8 *)0xFF51)
  #define TKCON    (*(vxu8 *)0xFF52)
  #define TKADDR   (*(vxu8 *)0xFF53)
  #define LCDM0    (*(vxu8 *)0xFF60)
  #define LCDM1    (*(vxu8 *)0xFF61)
  #define LCDM2    (*(vxu8 *)0xFF62)
  #define LCDM3    (*(vxu8 *)0xFF63)
  #define LCDM4    (*(vxu8 *)0xFF64)
  #define LCDM5    (*(vxu8 *)0xFF65)
  #define LCDM6    (*(vxu8 *)0xFF66)
  #define LCDM7    (*(vxu8 *)0xFF67)
  #define LCDM8    (*(vxu8 *)0xFF68)
  #define LCDM9    (*(vxu8 *)0xFF69)
  #define LCDM10   (*(vxu8 *)0xFF6A)
  #define LCDM11   (*(vxu8 *)0xFF6B)
  #define LCDM12   (*(vxu8 *)0xFF6C)
  #define LCDM13   (*(vxu8 *)0xFF6D)
  #define LCDM14   (*(vxu8 *)0xFF6E)
  #define LCDM15   (*(vxu8 *)0xFF6F)
  #define LCDM16   (*(vxu8 *)0xFF70)
  #define LCDM17   (*(vxu8 *)0xFF71)
  #define LCDM18   (*(vxu8 *)0xFF72)
  #define LCDM19   (*(vxu8 *)0xFF73)
  #define LCDM20   (*(vxu8 *)0xFF74)
  #define LCDM21   (*(vxu8 *)0xFF75)
  #define LCDM22   (*(vxu8 *)0xFF76)
  #define LCDM23   (*(vxu8 *)0xFF77)
  #define LCDM24   (*(vxu8 *)0xFF78)
  #define LCDM25   (*(vxu8 *)0xFF79)
  #define LCDM26   (*(vxu8 *)0xFF7A)
  #define LCDPC0   (*(vxu8 *)0xFF40)
  #define LCDPC1   (*(vxu8 *)0xFF41)
  #define LCDPC2   (*(vxu8 *)0xFF42)
  #define LCDPC3   (*(vxu8 *)0xFF43)
  #define LCDPC4   (*(vxu8 *)0xFF44)
#endif

#endif
