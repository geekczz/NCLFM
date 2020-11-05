#ifndef __ELECTRONICSCALEDRIVER_H
#define __ELECTRONICSCALEDRIVER_H

#include "GLOBAL.h"
	
//DOUT PA0
//PD_SCK PA1

#define HX_DOUT GPIOA,GPIO_Pin_2

#define HX_SCK_L GPIO_ResetBits(GPIOA,GPIO_Pin_1)
#define HX_SCK_H GPIO_SetBits(GPIOA,GPIO_Pin_1)


typedef struct SCALE_DATA
{
	uint32_t ScaleDataZero;
	int32_t ScaleData;
}SCALE_DATA;

extern SCALE_DATA Scale_Data;

uint32_t ElectronicScaleGetData(void);
void ScaleInit(void);
void ScaleSetToZero(void);
void UpdateScaleData(void);

#endif