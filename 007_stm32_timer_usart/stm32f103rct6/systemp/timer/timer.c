#include "./timer.h"
#include "./systemp/led/led.h"
#include "./systemp/usart/usart.h"

/*
	函数名称：u8 base_timer_init(TIM_TypeDef *tim, u32 arr, u32 psc)
	函数功能：基本定时器初始化函数
	函数形参：
				TIM_TypeDef *tim：定时器向量 ==> TIM6/TIM7
				u32 arr：自动重装载值
				u32 psc：预分配数据
	返 回 值：
				0：初始化成功
				1：初始化失败
*/
u8 base_timer_init(TIM_TypeDef *tim, u32 arr, u32 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	if (tim == TIM6)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	}
	else if(tim == TIM7)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	}
	else
	{
		return 1;
	}
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;
	TIM_TimeBaseInit(tim, &TIM_TimeBaseStructure);
	TIM_ClearFlag(tim, TIM_FLAG_Update);
//	TIM_Cmd(tim, ENABLE);
	TIM_Cmd(tim, DISABLE);
	return 0;
}


///定时器6中断优先级配置
void set_basetimer6_nvic(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);					//使能定时器更新中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;				//配置定时器7为中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//抢断优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能中断
	NVIC_Init(&NVIC_InitStructure);
}

///定时器7中断优先级配置
void set_basetimer7_nvic(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);					//使能定时器更新中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;				//配置定时器7为中断源 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//抢断优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//子优先级 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能中断 
	NVIC_Init(&NVIC_InitStructure);
}

///定时器6中断服务函数
void TIM6_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		TIM_Cmd(TIM6, DISABLE);
		usart1_receiveOver = 1;
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
}

///定时器7中断服务函数
void TIM7_IRQHandler(void)
{
	static char i = 1;
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
	{
		BEEP(i);
		i = !i;
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	}
}






