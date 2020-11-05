#include "ElectronicScaleDriver.h"

SCALE_DATA Scale_Data;

//SCK PA11
//DOUT PA8

uint32_t ElectronicScaleGetData(void)
{
	uint32_t DataCache = 0;
	uint8_t DataBit = 0;
	uint8_t Counter_I = 0;
	
	HX_SCK_L;
	
	while(GPIO_ReadInputDataBit(HX_DOUT))
	{
		
	}
	
	for(Counter_I = 0;Counter_I<24;Counter_I++)
	{
		DataCache = DataCache << 1;
		
		HX_SCK_H;
		__nop();
		__nop();
		__nop();
		
		DataBit = GPIO_ReadInputDataBit(HX_DOUT);
		DataCache = DataCache | DataBit;
		
		HX_SCK_L;
		__nop();
		__nop();
		__nop();
	}
	
	
	for(Counter_I = 0;Counter_I<3;Counter_I++)
	{
		HX_SCK_H;
		__nop();
		__nop();
		__nop();

		HX_SCK_L;
		__nop();
		__nop();
		__nop();
	}
	
	return DataCache;
}

void ScaleInit(void)
{
	uint8_t i = 0;
	
	for(i = 0;i<10;i++)
	{
		ElectronicScaleGetData();
	}
	
	ScaleSetToZero();
}

void ScaleSetToZero(void)
{
	uint32_t Scale_Data_Temp = 0;
	uint8_t i = 0;
	
	for(i = 0;i<10;i++)
	{
		Scale_Data_Temp += ElectronicScaleGetData();
	}
	Scale_Data.ScaleDataZero = Scale_Data_Temp / 10;
}

void UpdateScaleData(void)
{
	uint32_t ScaleDataAverage = 0;
	uint32_t Scale_Data_Temp = 0;
	uint8_t i = 0;
	
	for(i = 0;i<5;i++)
	{
		Scale_Data_Temp += ElectronicScaleGetData();
	}
	ScaleDataAverage = Scale_Data_Temp / 5;
//	ScaleDataAverage = ElectronicScaleGetData();
	
//	Scale_Data.ScaleData = ((int32_t)ScaleDataAverage - (int32_t)Scale_Data.ScaleDataZero);
	if(ScaleDataAverage < Scale_Data.ScaleDataZero)
	{
		Scale_Data.ScaleData = (int32_t)(((float)Scale_Data.ScaleDataZero - ScaleDataAverage)/maindata.SystemK+0.5);
	}
	else
	{
		Scale_Data.ScaleData = (int32_t)(((float)Scale_Data.ScaleDataZero - ScaleDataAverage)/maindata.SystemK-0.5);
	}
}
