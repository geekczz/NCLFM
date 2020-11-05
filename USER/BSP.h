#ifndef __BSP_H
#define __BSP_H

#include "GLOBAL.h"

typedef struct BSP_DATA
{
	unsigned int systime;
	unsigned char USART1_BUFFER[10];
}BSP_DATA;

typedef enum
{
	KEY_NONE =0x00,
  KEY1 = 0x01,
	KEY2 = 0x02,
	KEY3 = 0x04,
	KEY4 = 0x08,
	KEY5 = 0x10,
	KEY6 = 0x20,
	KEY7 = 0x40,
	KEY8 = 0x80
}KEY_TypeDef;

extern BSP_DATA bspdata;

void BSP_GPIO_Init(void);
void BSP_USART_Init(void);
void BSP_DMA_Init(void);

extern void bsp_delayms(unsigned int counter);
extern void BSP_Init(void);
void bsp_TIM_Init(void);

void SetKeyOutput(uint8_t KeyInput);
uint8_t MY_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint8_t GetKey(void);
uint8_t GetFootSwitch(void);
void BSP_Bee(void);

#endif
