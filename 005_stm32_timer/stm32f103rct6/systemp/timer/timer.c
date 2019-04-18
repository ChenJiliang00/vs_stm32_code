#include "./timer.h"

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
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	}
	else
	{
		return 1;
	}
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;
	TIM_TimeBaseInit(tim, &TIM_TimeBaseStructure);
	TIM_Cmd(tim, ENABLE);
	return 0;
}










