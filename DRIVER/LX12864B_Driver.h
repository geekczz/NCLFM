#ifndef __LX12864B_DRIVER_H
#define __LX12864B_DRIVER_H

#include "GLOBAL.h"

	//LCD_RST PA3
	//LCD_A0 PA4
	//LCD_SCL PA5
	//LCD_SDA PA6
	//LCD_CS PA7

#define LCD_SDA_L GPIO_ResetBits(GPIOA,GPIO_Pin_6)
#define LCD_SDA_H GPIO_SetBits(GPIOA,GPIO_Pin_6)

#define LCD_SCK_L GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define LCD_SCK_H GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define LCD_A0_L GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define LCD_A0_H GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define LCD_RST_L GPIO_ResetBits(GPIOA,GPIO_Pin_3)
#define LCD_RST_H GPIO_SetBits(GPIOA,GPIO_Pin_3)

#define LCD_CS_L GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define LCD_CS_H GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define IO_ST7565_SCK_0 LCD_SCK_L
#define IO_ST7565_SCK_1 LCD_SCK_H
#define IO_ST7565_CS_0 LCD_CS_L
#define IO_ST7565_CS_1 LCD_CS_H
#define IO_ST7565_RS_0 LCD_A0_L
#define IO_ST7565_RS_1 LCD_A0_H
#define IO_ST7565_SDA_0 LCD_SDA_L
#define IO_ST7565_SDA_1 LCD_SDA_H
#define IO_ST7565_RST_0 LCD_RST_L
#define IO_ST7565_RST_1 LCD_RST_H

#define ST7565_DELAY_IO 20

void WriteCommand(uint8_t CommandByte);
void WriteData(uint8_t DataByte);
void LcmClear(uint8_t FillData);
void LcmInit( void );
void LcmPutChar(uint8_t col,uint8_t page,uint8_t Order);
void LcmPutBlackChar(uint8_t col,uint8_t page,uint8_t Order);
void LcmPutTurnChar(uint8_t col,uint8_t page,uint8_t Order,uint8_t Black);
void LcmPutStr(uint8_t col,uint8_t page,uint8_t *puts);
void LcmPutChineseChar(uint8_t col,uint8_t page,uint8_t Order);
void LcmPutChineseBlackChar(uint8_t col,uint8_t page,uint8_t Order);
void LcmPutChineseTurnChar(uint8_t col,uint8_t page,uint8_t Order,uint8_t Black);
void LcmPutChineseStr(uint8_t col,uint8_t page,uint8_t *puts,uint8_t black);
void LcmPutNum(uint8_t col,uint8_t page,int32_t Num,uint8_t List);
void LcmPutTime(uint8_t col,uint8_t page,uint16_t Num);
void LcmPutBmp( uint8_t *puts );
void ST7565_IO_Delay(uint16_t t);
void ST7565_Write(uint8_t dat,uint8_t rs);
void writec(unsigned char com);
void writed(unsigned char dat);
void LCD7565_FullFill( uint8_t FillData );
void LCD_Init(void);
void display(unsigned char dat1,unsigned char dat2);
void displaychar(unsigned char *p);
void LcmPutNum2(uint8_t col,uint8_t page,uint32_t Num,uint8_t List);

#endif
