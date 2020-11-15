#include "main.h"
#include "math.h"

/************************函数声明************************/
void MainDataInit(void);
void DataMain2Setting(void);
void DataSetting2Main(void);
void WaterPumpControl(uint16_t PumpSpeed);
void GetWeight(void);
/************************函数声明************************/

/************************文字显示数据************************/
uint8_t WELCOME_DisplayLine_1[3] = {17,18,0xff};//北京九应和智能控制
uint8_t WELCOME_DisplayLine_2[8] = {19,20,21,22,23,24,25,0xff};
uint8_t WELCOME_DisplayLine_3[3] = {26,27,0xff};

uint8_t USERMENU_DisplayLine_1[4] = {1,2,3,0xff};
uint8_t USERMENU_DisplayLine_2[4] = {4,5,6,0xff};
uint8_t USERMENU_DisplayLine_3[4] = {7,8,9,0xff};
uint8_t USERMENU_DisplayLine_4[4] = {10,11,9,0xff};

uint8_t SETTING_DisplayLine_1[6] = {12,13,14,45,46,0xff};
uint8_t SETTING_DisplayLine_2[4] = {4,5,6,0xff};
uint8_t SETTING_DisplayLine_3[4] = {7,8,9,0xff};
uint8_t SETTING_DisplayLine_4[8] = {31,31,31,31,31,31,31,0xff};
uint8_t SETTING_DisplayLine_5[5] = {45,46,47,2,0xff};

uint8_t WARNING_DisplayLine_1[6] = {32,33,34,35,36,0xff};
uint8_t WARNING_DisplayLine_2[2] = {37,0xff};
uint8_t WARNING_DisplayLine_3[7] = {38,35,36,39,40,41,0xff};
uint8_t WARNING_DisplayLine_4[6] = {42,43,44,45,46,0xff};

uint8_t WEIGHTCLE_DisplayLine_1[4] = {32,33,34,0xff};
uint8_t WEIGHTCLE_DisplayLine_2[4] = {31,31,31,0xff};

/************************文字显示数据************************/

/*************************变量数据**************************/
MAIN_DATA maindata;
SETTING_DATA settingdata;
KEY_TypeDef mark;
uint8_t FootMark = 1;

uint8_t BlackMark = 0;
int32_t sDataTemp = 0;
int8_t ST_InputPointer = 9;
uint16_t i = 0;
uint16_t Counter_j = 0;

int16_t BotleFillUpPID_P = 70;
uint8_t FillUpHoldCounter = 0;
int16_t BottleFillUp_Err = 0;
int16_t SpeedOutput = 0;

uint32_t EndUpTime = 0;
uint32_t TimeDispCache = 0;

int32_t WeightCache = 0;
uint8_t StateCache = 0;

uint32_t ScaleDataTemp = 0;
uint32_t ScaleData100Temp = 0;
int32_t  SignLongDataTemp = 0;

int16_t SystemKTemp = 0; 
/*************************变量数据**************************/

/*************************FLASH变量**************************/
#define SystemDataStoreAddrStart ((uint32_t)0x08006000)

#define SystemPrivateDataStoreAddrStart ((uint32_t)0x08006400)

uint32_t SystemDataStoreAddr = 0;
/*************************FLASH变量**************************/

int main(void)
{	
 	BSP_Init();
	
	IO_ST7565_RST_0;
	bsp_delayms(100);
	IO_ST7565_RST_1;
	bsp_delayms(100);
	LCD_Init();
	
	//开始界面
	display(0x00,0x00);
	LcmPutChineseStr(50,1,WELCOME_DisplayLine_1,0);
	LcmPutChineseStr(10,4,WELCOME_DisplayLine_2,0);
	LcmPutChineseStr(2, 7,WELCOME_DisplayLine_3,0);
	LcmPutChar(33,7,0);
	LcmPutNum2(40,7,135214,6);
	LcmPutNum2(89,7,83500,5);
	
	ScaleInit();
	MainDataInit();
	TIM_Cmd(TIM1,ENABLE);
	bsp_delayms(2000);

/**************************用户菜单程序段开始*************************/
USERMENU:
	display(0x00,0x00);
	LcmPutChineseStr(3,1,USERMENU_DisplayLine_1,0);
	LcmPutChar(51,1,0);
	LcmPutChineseStr(3,3,USERMENU_DisplayLine_2,0);
	LcmPutChar(51,3,0);
	LcmPutChineseStr(3,5,USERMENU_DisplayLine_3,0);
	LcmPutChar(51,5,0);
	LcmPutChineseStr(3,7,USERMENU_DisplayLine_4,0);
	LcmPutChar(51,7,0);
	
	LcmPutChineseChar(114,5,0);
	LcmPutChineseChar(114,7,0);

	LcmPutNum(66,1,maindata.uFillCounter,5);
	LcmPutNum(66,3,maindata.uFillSpeed,2);
	LcmPutNum(66,5,maindata.uFillSetWeight,5);
	LcmPutNum(66,7,maindata.sFillActWeight,5);
	
	while(1)
	{	
USERMENUSCANLOOP:
		Counter_j ++;
		if(Counter_j >= 10)
		{
			Counter_j = 0;
			GetWeight();
			LcmPutNum(66,7,maindata.sFillActWeight,5);
			LcmPutNum(66,1,maindata.uFillCounter,5);
		}
		
		GPIO_SetBits(LED_STOP);
		GPIO_ResetBits(LED_START);
		
		if(maindata.uFillCountinue == 1)GPIO_ResetBits(LED_SINGLEMODEON);
		else GPIO_SetBits(LED_SINGLEMODEON);
		
		mark = GetKey();
		FootMark = GetFootSwitch();
		if(mark == KEY8)
		{
			BSP_Bee();
			while(mark != KEY_NONE)
			{
				mark = GetKey();
			}
			goto SETTINGBEGIN;
		}
		else if(mark == KEY7)
		{
			BSP_Bee();
			for(i=0;i<200;i++)
			{
				mark = GetKey();
				bsp_delayms(10);
				if(mark != KEY7)
				{
					//称重归零
					ScaleSetToZero();
					maindata.sBottleWeight = 0;
					goto FUNCTIONKEY7END;
				}
			}
			//计数器归零
			maindata.uFillCounter = 0;
			LcmPutNum(66,1,maindata.uFillCounter,5);
			
			while(mark != KEY_NONE)
			{
				mark = GetKey();
			}
FUNCTIONKEY7END:			
			__nop();
		}
		else if(mark == KEY6)
		{
			BSP_Bee();
			while(mark != KEY_NONE)
			{
				mark = GetKey();
			}
			if(maindata.uFillCountinue == 1)maindata.uFillCountinue = 0;
			else maindata.uFillCountinue = 1;
		}
		else if(mark == KEY5 || FootMark == 0)
		{
			BSP_Bee();
			while(mark != KEY_NONE)
			{
				mark = GetKey();
			}
			
			while(FootMark == 0)
			{
				FootMark = GetFootSwitch();
			}
			
			GPIO_SetBits(LED_START);
			GPIO_ResetBits(LED_STOP);
			
			if((maindata.sFillActWeight == 0) && (maindata.sBottleWeight == 0))
			{
				display(0x00,0x00);
				LcmPutChineseStr(28,1,WARNING_DisplayLine_1,0);
				LcmPutChineseStr(60,3,WARNING_DisplayLine_2,0);
				LcmPutChineseStr(20,5,WARNING_DisplayLine_3,0);
				LcmPutChineseStr(28,7,WARNING_DisplayLine_4,0);
	
				bsp_delayms(3000);
				goto USERMENU;
			}
			
			goto FILLUPBEGIN;
		}
		
		else if(mark == KEY3)
		{
			for(i=0;i<500;i++)
			{
				mark = GetKey();
				bsp_delayms(10);
				if(mark != KEY3)
				{
					goto FUNCTIONKEY3END;
				}
			}
			goto WEIGHTCLEBEGIN;
			
FUNCTIONKEY3END:			
			__nop();
		}
		
		else{
		}
	}
/**************************用户菜单程序段结束*************************/

	
/**************************设置程序段开始*************************/
SETTINGBEGIN:
	DataMain2Setting();
	
	display(0x00,0x00);
	LcmPutChineseStr(3,1,SETTING_DisplayLine_1,0);
	LcmPutChar(83,1,0);
	LcmPutChineseStr(3,3,SETTING_DisplayLine_2,0);
	LcmPutChar(51,3,0);
	LcmPutChineseStr(3,5,SETTING_DisplayLine_3,0);
	LcmPutChar(51,5,0);
	LcmPutChineseStr(3,7,SETTING_DisplayLine_5,0);
	LcmPutChar(67,7,0);
	
	LcmPutChineseChar(114,5,0);
	
	LcmPutChineseChar(99,1,settingdata.uSETPeelOn?29:30);
	
	LcmPutNum(66,3,settingdata.sSETSpeed[1]*10+settingdata.sSETSpeed[0],2);
	LcmPutNum(66,5,settingdata.sSETSetWeight[4]*10000+settingdata.sSETSetWeight[3]*1000+settingdata.sSETSetWeight[2]*100+settingdata.sSETSetWeight[1]*10+settingdata.sSETSetWeight[0],5);
	LcmPutNum(82,7,settingdata.sSETFillUpPID_P[1]*10 + settingdata.sSETFillUpPID_P[0],2);
	
	while(1)
	{
		/******************************按键处理程序段******************************/
		mark = GetKey();
		if(mark == KEY8)
		{
			BSP_Bee();
			while(mark != KEY_NONE)
			{
				mark = GetKey();
			}
			DataSetting2Main();
			
			//擦除FLASH页
			FLASH_Unlock();
			FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR); 
			if (FLASH_ErasePage(SystemDataStoreAddrStart)!= FLASH_COMPLETE)
			{
				while (1)
				{
					
				}
			}
			
			SystemDataStoreAddr = SystemDataStoreAddrStart;
			if (FLASH_ProgramWord(SystemDataStoreAddr, (uint32_t)maindata.uFillCountinue) != FLASH_COMPLETE)
			{
				while(1)
				{
					
				}
			}
			SystemDataStoreAddr += 4;
			
			if (FLASH_ProgramWord(SystemDataStoreAddr, (uint32_t)maindata.uFillPeelOn) != FLASH_COMPLETE)
			{
				while(1)
				{
					
				}
			}
			SystemDataStoreAddr += 4;
			
			if (FLASH_ProgramWord(SystemDataStoreAddr, (uint32_t)maindata.uFillSetWeight) != FLASH_COMPLETE)
			{
				while(1)
				{
					
				}
			}
			SystemDataStoreAddr += 4;
			
			if (FLASH_ProgramWord(SystemDataStoreAddr, (uint32_t)maindata.uFillSpeed) != FLASH_COMPLETE)
			{
				while(1)
				{
					
				}
			}
			SystemDataStoreAddr += 4;
			
			FLASH_Lock(); 
			goto USERMENU;
		}
		if(mark == KEY1)
		{
			BSP_Bee();
			while(mark != KEY_NONE)
			{
				mark = GetKey();
			}
			ST_InputPointer = ST_InputPointer + 1;
			if(ST_InputPointer>9) ST_InputPointer = 0;
		}
		if(mark == KEY2)
		{
			BSP_Bee();
			while(mark != KEY_NONE)
			{
				mark = GetKey();
			}
			ST_InputPointer = ST_InputPointer - 1;
			if(ST_InputPointer<0)ST_InputPointer = 9;
		}
		if(mark == KEY3)
		{
			BSP_Bee();
			while(mark != KEY_NONE)
			{
				mark = GetKey();
			}
			switch(ST_InputPointer)
			{
				case 9 :
					if(settingdata.uSETPeelOn == 1)settingdata.uSETPeelOn = 0;
					else settingdata.uSETPeelOn = 1;
					break;
				case 8 :
				case 7 :
					settingdata.sSETSpeed[ST_InputPointer-7] -= 1;
					if(settingdata.sSETSpeed[ST_InputPointer-7] < 0)settingdata.sSETSpeed[ST_InputPointer-7] = 9;
					break;
				case 6 :
				case 5 :
				case 4 :
				case 3 :
				case 2 :
					settingdata.sSETSetWeight[ST_InputPointer-2] -= 1;
					if(settingdata.sSETSetWeight[ST_InputPointer-2] < 0)settingdata.sSETSetWeight[ST_InputPointer-2] = 9;
					break;
				case 1 :
				case 0 :
					settingdata.sSETFillUpPID_P[ST_InputPointer] -= 1;
					if(settingdata.sSETFillUpPID_P[ST_InputPointer] < 0)settingdata.sSETFillUpPID_P[ST_InputPointer] = 9;
					break;
				default :
					break;
			}
		}
		if(mark == KEY4)
		{
			BSP_Bee();
			while(mark != KEY_NONE)
			{
				mark = GetKey();
			}
			switch(ST_InputPointer)
			{
				case 9 :
					if(settingdata.uSETPeelOn == 1)settingdata.uSETPeelOn = 0;
					else settingdata.uSETPeelOn = 1;
					break;
				case 8 :
				case 7 :
					settingdata.sSETSpeed[ST_InputPointer-7] += 1;
					if(settingdata.sSETSpeed[ST_InputPointer-7] > 9)settingdata.sSETSpeed[ST_InputPointer-7] = 0;
					break;
				case 6 :
				case 5 :
				case 4 :
				case 3 :
				case 2 :
					settingdata.sSETSetWeight[ST_InputPointer-2] += 1;
					if(settingdata.sSETSetWeight[ST_InputPointer-2] > 9)settingdata.sSETSetWeight[ST_InputPointer-2] = 0;
					break;
				case 1 :
				case 0 :
					settingdata.sSETFillUpPID_P[ST_InputPointer] += 1;
					if(settingdata.sSETFillUpPID_P[ST_InputPointer] > 9)settingdata.sSETFillUpPID_P[ST_InputPointer] = 0;
					break;
				default :
					break;
			}
		}
		/******************************按键处理程序段******************************/
		
		/******************************屏幕刷新程序段******************************/
		switch(ST_InputPointer)
		{
			case 9 :
				LcmPutTurnChar(91,7,settingdata.sSETFillUpPID_P[0]+4,0);
				LcmPutChineseTurnChar(99,1,settingdata.uSETPeelOn?29:30,BlackMark);
				LcmPutTurnChar(66,3,settingdata.sSETSpeed[1]+4,0);
				break;
			case 8 :
				LcmPutChineseTurnChar(99,1,settingdata.uSETPeelOn?29:30,0);
				LcmPutTurnChar(66,3,settingdata.sSETSpeed[1]+4,BlackMark);
				LcmPutTurnChar(75,3,settingdata.sSETSpeed[0]+4,0);
				break;
			case 7 :
				LcmPutTurnChar(66,3,settingdata.sSETSpeed[1]+4,0);
				LcmPutTurnChar(75,3,settingdata.sSETSpeed[0]+4,BlackMark);
				LcmPutTurnChar(66,5,settingdata.sSETSetWeight[4]+4,0);
				break;
			case 6 :
				LcmPutTurnChar(75,3,settingdata.sSETSpeed[0]+4,0);
				LcmPutTurnChar(66,5,settingdata.sSETSetWeight[4]+4,BlackMark);
				LcmPutTurnChar(75,5,settingdata.sSETSetWeight[3]+4,0);
				break;
			case 5 :
				LcmPutTurnChar(66,5,settingdata.sSETSetWeight[4]+4,0);
				LcmPutTurnChar(75,5,settingdata.sSETSetWeight[3]+4,BlackMark);
				LcmPutTurnChar(84,5,settingdata.sSETSetWeight[2]+4,0);
				break;
			case 4 :
				LcmPutTurnChar(75,5,settingdata.sSETSetWeight[3]+4,0);
				LcmPutTurnChar(84,5,settingdata.sSETSetWeight[2]+4,BlackMark);
				LcmPutTurnChar(93,5,settingdata.sSETSetWeight[1]+4,0);
				break;
			case 3 :
				LcmPutTurnChar(84,5,settingdata.sSETSetWeight[2]+4,0);
				LcmPutTurnChar(93,5,settingdata.sSETSetWeight[1]+4,BlackMark);
				LcmPutTurnChar(102,5,settingdata.sSETSetWeight[0]+4,0);
				break;
			case 2 :
				LcmPutTurnChar(93,5,settingdata.sSETSetWeight[1]+4,0);
				LcmPutTurnChar(102,5,settingdata.sSETSetWeight[0]+4,BlackMark);
				LcmPutTurnChar(82,7,settingdata.sSETFillUpPID_P[1]+4,0);
				break;
			case 1 :
				LcmPutTurnChar(102,5,settingdata.sSETSetWeight[0]+4,0);
				LcmPutTurnChar(82,7,settingdata.sSETFillUpPID_P[1]+4,BlackMark);
				LcmPutTurnChar(91,7,settingdata.sSETFillUpPID_P[0]+4,0);
				break;
			case 0 :
				LcmPutTurnChar(82,7,settingdata.sSETFillUpPID_P[1]+4,0);
				LcmPutTurnChar(91,7,settingdata.sSETFillUpPID_P[0]+4,BlackMark);
				LcmPutChineseTurnChar(99,1,settingdata.uSETPeelOn?29:30,0);
				break;
			default:
				break;
		}
		/******************************屏幕刷新程序段******************************/
	}
/**************************设置程序段结束*************************/
	

/**************************灌装程序段开始*************************/
FILLUPBEGIN:
	//先去皮 （开始按键、倒计时、脚踏） 
	
	if(maindata.uFillPeelOn == 1)
	{
		UpdateScaleData();
		maindata.sBottleWeight = Scale_Data.ScaleData;
	}
	else
	{
		maindata.sBottleWeight = 0;
	}
	
	FillUpHoldCounter = 0;
	
	for(Counter_j = 0;Counter_j<maindata.uFillSpeed;Counter_j++)
	{
		WaterPumpControl(Counter_j*10);
		bsp_delayms(2);
	}
	
	//PID灌装
	while(1)
	{
		//更新重量并显示
		mark = GetKey();
		FootMark = GetFootSwitch();
		if(mark == KEY5 || FootMark == 0)
		{
			BSP_Bee();
			WaterPumpControl(0);
			while(mark != KEY_NONE)
			{
				mark = GetKey();
			}
			while(FootMark == 0)
			{
				FootMark = GetFootSwitch();
			}
			goto USERMENU;
		}
		
		GetWeight();
		LcmPutNum(66,7,maindata.sFillActWeight,4);
		
		BottleFillUp_Err = maindata.uFillSetWeight - maindata.sFillActWeight;
		if(BottleFillUp_Err < 1)
		{
			BottleFillUp_Err = 0;
			WaterPumpControl(0);
			maindata.uFillCounter++;
			LcmPutNum(66,1,maindata.uFillCounter,5);
			goto CHECKSTATEOK;
		}
		else if(BottleFillUp_Err > 15)BottleFillUp_Err = 15;
		
		SpeedOutput = BottleFillUp_Err * BotleFillUpPID_P;
		if(SpeedOutput > (maindata.uFillSpeed*10) ) SpeedOutput = maindata.uFillSpeed*10;
		if(SpeedOutput != 0 && SpeedOutput < 200)SpeedOutput = 200;
		
		WaterPumpControl(SpeedOutput);
		bsp_delayms(10);
	}
	
CHECKSTATEOK:
	//检测取瓶
	if(maindata.uFillCountinue == 1)
	{
		while(maindata.sFillActWeight > 0)
		{
			mark = GetKey();
			FootMark = GetFootSwitch();
			if(mark == KEY5 || FootMark == 0)
			{
				BSP_Bee();
				WaterPumpControl(0);
				while(mark != KEY_NONE)
				{
					mark = GetKey();
				}
				while(FootMark == 0)
				{
					FootMark = GetFootSwitch();
				}
				goto USERMENU;
			}
			
			GetWeight();
			LcmPutNum(66,7,maindata.sFillActWeight,4);
		}

		while(1)
		{
WAITFORBOTTLE:
			mark = GetKey();
			FootMark = GetFootSwitch();
			if(mark == KEY5 || FootMark == 0)
			{
				BSP_Bee();
				WaterPumpControl(0);
				while(mark != KEY_NONE)
				{
					mark = GetKey();
				}
				while(FootMark == 0)
				{
					FootMark = GetFootSwitch();
				}
				goto USERMENU;
			}
			
			GetWeight();
			LcmPutNum(66,7,maindata.sFillActWeight,4);
			
			if(maindata.sFillActWeight > (-maindata.sBottleWeight+5))
			{
				WeightCache = maindata.sFillActWeight;
				StateCache = 1;
			}
			
			if(StateCache == 1)
			{
				StateCache = 0;
				EndUpTime = bspdata.systime + 1000;
				while(EndUpTime > bspdata.systime)
				{
					GetWeight();
					LcmPutNum(66,7,maindata.sFillActWeight,4);
					if((maindata.sFillActWeight - WeightCache>3)||(maindata.sFillActWeight - WeightCache<-3)) goto WAITFORBOTTLE;
				}
				goto FILLUPBEGIN;
			}
		}
	}
	
	//非连续模式
	else
	{
		goto USERMENUSCANLOOP;
	}
	
/**************************灌装程序段结束*************************/
	
/**************************称重校准程序段开始*************************/
WEIGHTCLEBEGIN:
	display(0x00,0x00);
	LcmPutNum(73,3,100,3);
	LcmPutChineseStr(25,3,WEIGHTCLE_DisplayLine_1,0);
	LcmPutChar(100,3,21);
	
	i = 0;
	
	while(i<60)
	{
		ScaleDataTemp = ElectronicScaleGetData();
		SignLongDataTemp = Scale_Data.ScaleDataZero - ScaleDataTemp;
		if(SignLongDataTemp > 2000)
		{
			i++;
		}
		bsp_delayms(10);
	}
	
	LcmPutChar(49,5,17);
	LcmPutChar(57,5,18);
	LcmPutChar(65,5,19);
	LcmPutChar(73,5,20);
	LcmPutChar(81,5,1);
	LcmPutChar(89,5,1);
	LcmPutChar(97,5,1);
	
	ScaleDataTemp = 0;
	
	for(i = 0;i<50;i++)
	{
		ScaleDataTemp += ElectronicScaleGetData();
	}
	
	ScaleData100Temp = ScaleDataTemp / 50;
	
	SystemKTemp = (Scale_Data.ScaleDataZero - ScaleData100Temp)/100;
	
	LcmPutChineseStr(49,5,WEIGHTCLE_DisplayLine_2,0);
	
	while(1)
	{
		mark = GetKey();
		if(mark == KEY1)
		{
			while(mark != KEY_NONE)
			{
				mark = GetKey();
			}
			maindata.SystemK = SystemKTemp;
			
			FLASH_Unlock();
			FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR); 
			if (FLASH_ErasePage(SystemPrivateDataStoreAddrStart)!= FLASH_COMPLETE)
			{
				while (1)
				{
					
				}
			}
			
			SystemDataStoreAddr = SystemPrivateDataStoreAddrStart;
			if (FLASH_ProgramWord(SystemDataStoreAddr, (uint32_t)maindata.SystemK) != FLASH_COMPLETE)
			{
				while(1)
				{
					
				}
			}
			FLASH_Lock(); 
			
			goto USERMENU;
		}
		else if(mark == KEY_NONE)
		{
			
		}
		else
		{
			goto USERMENU;
		}
	}
	
/**************************称重校准程序段结束*************************/

}

void MainDataInit(void)
{
	maindata.sFillActWeight = 0;
	maindata.uFillCounter = 0;
	maindata.sBottleWeight = 0;
	
	SystemDataStoreAddr = SystemDataStoreAddrStart;
	maindata.uFillCountinue	= *(__IO uint32_t *)SystemDataStoreAddr;
	SystemDataStoreAddr += 4;
	maindata.uFillPeelOn = *(__IO uint32_t *)SystemDataStoreAddr;
	SystemDataStoreAddr += 4;
	maindata.uFillSetWeight = *(__IO uint32_t *)SystemDataStoreAddr;
	SystemDataStoreAddr += 4;
	maindata.uFillSpeed = *(__IO uint32_t *)SystemDataStoreAddr;
	SystemDataStoreAddr += 4;
	
	if(!(maindata.uFillCountinue == 1 || maindata.uFillCountinue == 0))
	{
		maindata.uFillCountinue = 1;
	}
	if(!(maindata.uFillPeelOn == 1 || maindata.uFillPeelOn == 0))
	{
		maindata.uFillPeelOn = 1;
	}
	if(maindata.uFillSetWeight > 99999 || maindata.uFillSetWeight == 0)
	{
		maindata.uFillSetWeight = 200;
	}
	if(maindata.uFillSpeed > 99 || maindata.uFillSpeed == 0)
	{
		maindata.uFillSpeed = 80;
	}
	
	SystemDataStoreAddr = SystemPrivateDataStoreAddrStart;
	maindata.SystemK	= *(__IO uint32_t *)SystemDataStoreAddr;
	if(maindata.SystemK == -1)
	{
		maindata.SystemK = 420;
	}
	
}

void WaterPumpControl(uint16_t PumpSpeed)
{
	if(PumpSpeed >= 1000)PumpSpeed = 999;
	if(PumpSpeed < 1)PumpSpeed = 1;
	
	TIM_SetCompare4(TIM3,PumpSpeed);
	TIM_SetCompare3(TIM3,PumpSpeed);
}

void DataMain2Setting(void)
{
	uint32_t DataTemp = 0;
	
	settingdata.uSETPeelOn = maindata.uFillPeelOn;
	
	DataTemp = maindata.uFillSpeed;
	settingdata.sSETSpeed[1] = DataTemp/10;
	settingdata.sSETSpeed[0] = DataTemp%10;
	
	DataTemp = maindata.uFillSetWeight;
	settingdata.sSETSetWeight[4] = DataTemp/10000;
	settingdata.sSETSetWeight[3] = DataTemp%10000/1000;
	settingdata.sSETSetWeight[2] = DataTemp%1000/100;
	settingdata.sSETSetWeight[1] = DataTemp%100/10;
	settingdata.sSETSetWeight[0] = DataTemp%10;
	
	DataTemp = BotleFillUpPID_P;
	settingdata.sSETFillUpPID_P[1] = DataTemp/10;
	settingdata.sSETFillUpPID_P[0] = DataTemp%10;
}

void DataSetting2Main(void)
{
	maindata.uFillPeelOn = settingdata.uSETPeelOn;
	maindata.uFillSpeed = settingdata.sSETSpeed[1] * 10 + settingdata.sSETSpeed[0];
	maindata.uFillSetWeight = settingdata.sSETSetWeight[4] * 10000 + settingdata.sSETSetWeight[3] * 1000 + settingdata.sSETSetWeight[2] * 100 + settingdata.sSETSetWeight[1] * 10 + settingdata.sSETSetWeight[0];
	BotleFillUpPID_P = settingdata.sSETFillUpPID_P[1] * 10 + settingdata.sSETFillUpPID_P[0];
}

void GetWeight(void)
{
	UpdateScaleData();
	if(maindata.uFillPeelOn == 1)
	{
		maindata.sFillActWeight = Scale_Data.ScaleData - maindata.sBottleWeight;
	}
	else
	{
		maindata.sFillActWeight = Scale_Data.ScaleData;
	}
}