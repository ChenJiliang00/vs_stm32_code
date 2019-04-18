#include "./timer.h"

/*
	�������ƣ�u8 base_timer_init(TIM_TypeDef *tim, u32 arr, u32 psc)
	�������ܣ�������ʱ����ʼ������
	�����βΣ�
				TIM_TypeDef *tim����ʱ������ ==> TIM6/TIM7
				u32 arr���Զ���װ��ֵ
				u32 psc��Ԥ��������
	�� �� ֵ��
				0����ʼ���ɹ�
				1����ʼ��ʧ��
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










