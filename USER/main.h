#ifndef  __MAIN_H
#define  __MAIN_H

#include "GLOBAL.H"

#define LED_START GPIOA,GPIO_Pin_8
#define LED_STOP GPIOA,GPIO_Pin_9
#define LED_SINGLEMODEON GPIOB,GPIO_Pin_4

typedef struct MAIN_DATA
{
	uint32_t uFillCounter;
	uint8_t  uFillSpeed;
	uint32_t uFillSetWeight;
	int32_t  sFillActWeight;
	uint8_t  uFillPeelOn;
	int32_t  sBottleWeight;
	uint8_t  uFillCountinue;
	int16_t  SystemK;
}MAIN_DATA;

typedef struct SETTING_DATA
{
	uint8_t uSETPeelOn;
	int8_t sSETSpeed[2];
	int8_t sSETSetWeight[5];
	int8_t sSETFillUpPID_P[2];
}SETTING_DATA;

extern MAIN_DATA maindata;
extern SETTING_DATA settingdata;

#endif
