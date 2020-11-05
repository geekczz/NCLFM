#include "BSP.h"

BSP_DATA bspdata;

void BSP_Init(void)
{
	SysTick_Config(SystemCoreClock/1000);
	
	BSP_GPIO_Init();
	bsp_TIM_Init();
}

void BSP_GPIO_Init(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	GPIO_InitTypeDef gpio;
	
	/****************LCD Interface********************/
	//LCD_RST PA3
	//LCD_A0 PA4
	//LCD_SCL PA5
	//LCD_SDA PA6
	//LCD_CS PA7
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	gpio.GPIO_PuPd=GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
	/****************LCD Interface********************/
	
	/****************Foot Switch Input********************/
	//PA2
	gpio.GPIO_Mode=GPIO_Mode_IN;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Pin=GPIO_Pin_0;
	gpio.GPIO_PuPd=GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&gpio);
	/****************Foot Switch Input********************/
	
	/****************Electronic Scale Interface********************/
	//DOUT PA0
	//PD_SCK PA1
	gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Pin = GPIO_Pin_2;
	gpio.GPIO_PuPd=GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed=GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA,&gpio);
	
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Pin = GPIO_Pin_1;
	gpio.GPIO_PuPd=  GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	/****************Electronic Scale Interface********************/
	
	
	/*************************KeyInput Pin*************************/
	gpio.GPIO_Mode=GPIO_Mode_IN;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Pin = GPIO_Pin_7;
	gpio.GPIO_PuPd = GPIO_PuPd_DOWN;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio);
	
	gpio.GPIO_Mode=GPIO_Mode_IN;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Pin = GPIO_Pin_11;
	gpio.GPIO_PuPd = GPIO_PuPd_DOWN;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
	
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Pin =GPIO_Pin_10|GPIO_Pin_15;
	gpio.GPIO_PuPd=GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
	
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_6;
	gpio.GPIO_PuPd=GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio);
	/*************************KeyInput Pin*************************/
	
	/*************************Buzzer Pin*************************/
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Pin=GPIO_Pin_5;
	gpio.GPIO_PuPd=GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio);
	/*************************Buzzer Pin*************************/
	
	/*************************Pump Control Pin*************************/
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_1);
	/*************************Pump Control Pin*************************/
	
	/*************************LED Pin*************************/
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
	gpio.GPIO_PuPd=GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&gpio);
	
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Pin=GPIO_Pin_4;
	gpio.GPIO_PuPd=GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&gpio);
	/*************************LED Pin*************************/
}

void bsp_TIM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TBIT;
	TIM_OCInitTypeDef TIM_OCITD;
	NVIC_InitTypeDef NVIC_ITD;
	
	TIM_TBIT.TIM_ClockDivision = 2-1;
	TIM_TBIT.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TBIT.TIM_Period = 1000;
	TIM_TBIT.TIM_Prescaler = 48 - 1;
	TIM_TBIT.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_TBIT);
	
	TIM_OCITD.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCITD.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCITD.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCITD.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCITD.TIM_Pulse = 1;
	TIM_OC3Init(TIM3,&TIM_OCITD);
	
	TIM_OCITD.TIM_Pulse = 1;
	TIM_OC4Init(TIM3,&TIM_OCITD);
	TIM_Cmd(TIM3,ENABLE);
	TIM_ARRPreloadConfig(TIM3,ENABLE);
//	TIM_CtrlPWMOutputs(TIM3,ENABLE);

	TIM_TBIT.TIM_ClockDivision = 2-1;
	TIM_TBIT.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TBIT.TIM_Period = 50000-1;
	TIM_TBIT.TIM_Prescaler = 480 - 1;
	TIM_TBIT.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM_TBIT);
	
	TIM_OCITD.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCITD.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCITD.TIM_Pulse = 40691 - 1;
	TIM_OCITD.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM1,&TIM_OCITD);
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Disable);
	
	TIM_ITConfig(TIM1,TIM_IT_CC1,ENABLE);
	TIM_Cmd(TIM1,DISABLE);
	
	NVIC_ITD.NVIC_IRQChannel = TIM1_CC_IRQn;
	NVIC_ITD.NVIC_IRQChannelPriority = 0;
	NVIC_ITD.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_ITD);
}

void bsp_delayms(unsigned int counter)
{
	unsigned int temp = counter + bspdata.systime;
	while(bspdata.systime<temp);
//	bspdata.systime = 0;
}

void SetKeyOutput(uint8_t KeyInput)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_6,((KeyInput&0x01)?Bit_SET:Bit_RESET));
	GPIO_WriteBit(GPIOB,GPIO_Pin_3,((KeyInput&0x02)?Bit_SET:Bit_RESET));
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,((KeyInput&0x04)?Bit_SET:Bit_RESET));
	GPIO_WriteBit(GPIOA,GPIO_Pin_10,((KeyInput&0x08)?Bit_SET:Bit_RESET));
}

uint8_t MY_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	uint8_t cachebitstatus = 0x00;
	uint8_t bitstatus = 0x00;

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
	
	cachebitstatus = GPIO_ReadInputDataBit(GPIOx,GPIO_Pin);
	if(cachebitstatus == (uint8_t)Bit_SET)
	{
		bsp_delayms(1);
		cachebitstatus = GPIO_ReadInputDataBit(GPIOx,GPIO_Pin);
		if(cachebitstatus == (uint8_t)Bit_SET)
		{
			bitstatus = (uint8_t)Bit_SET;
		}
		else bitstatus = (uint8_t)Bit_RESET;
	}
	else bitstatus = (uint8_t)Bit_RESET;
	
	return bitstatus;
}

uint8_t GetKey(void)
{ 
	uint8_t a,b = 0;
	uint8_t getdata = 0;
	uint8_t shiftData = 0x01;
	uint8_t i;
	for(i=0;i<4;i++)
	{
		SetKeyOutput(shiftData);
		a = MY_ReadInputDataBit(GPIOB,GPIO_Pin_7);
		b = MY_ReadInputDataBit(GPIOA,GPIO_Pin_11);
		if(a==1) getdata |= (0x01<<i);
		if(b==1) getdata |= (0x01<<(i+4));
		shiftData<<=1;
	}
	return getdata;
}

uint8_t GetFootSwitch(void)
{
	uint8_t DataTemp = 0;
	DataTemp = MY_ReadInputDataBit(GPIOA,GPIO_Pin_0);
	return DataTemp;
}

void BSP_Bee(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	bsp_delayms(100);
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
}

