#include "./exti.h"
#include "./systemp/key/key.h"
#include "./systemp/led/led.h"

/*
	函数名称：void exti_init(void)
	函数功能：按键外部中断初始化函数
	函数形参：无
	返 回 值：无
*/
void exti_init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//使能复用功能时钟
	
	///配置外部中断线
	///PA0
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//EXTI_Trigger_Falling EXTI_Trigger_Rising_Falling
	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	///PC13
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//EXTI_Trigger_Falling EXTI_Trigger_Rising_Falling
	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	///配置外部中断优先级
	///外部中断线0
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;				//使能按键WK_UP所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);
	///外部中断线13
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能按键WK_UP所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//抢占优先级1， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);
}

//外部中断0服务程序 
void EXTI0_IRQHandler(void)
{
	static i = 0;
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		if (KEY1 == 1)	 	 //KEY1按键
		{
			i = !i;
			BEEP(i);
		}
		EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位  
	}
}

void EXTI15_10_IRQHandler(void)
{
	static j = 1;
	if (EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
		if (KEY2 == 1)	 	 //KEY2按键
		{
			j = !j;
			RGB_G(j);
		}
		EXTI_ClearITPendingBit(EXTI_Line13); //清除LINE13上的中断标志位 
	}	
}

