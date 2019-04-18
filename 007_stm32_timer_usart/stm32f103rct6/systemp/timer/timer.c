#include "./timer.h"
#include "./systemp/led/led.h"
#include "./systemp/usart/usart.h"

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


///��ʱ��6�ж����ȼ�����
void set_basetimer6_nvic(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);					//ʹ�ܶ�ʱ�������ж�
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;				//���ö�ʱ��7Ϊ�ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//�������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);
}

///��ʱ��7�ж����ȼ�����
void set_basetimer7_nvic(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);					//ʹ�ܶ�ʱ�������ж�
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;				//���ö�ʱ��7Ϊ�ж�Դ 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//�������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//�����ȼ� 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ж� 
	NVIC_Init(&NVIC_InitStructure);
}

///��ʱ��6�жϷ�����
void TIM6_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		TIM_Cmd(TIM6, DISABLE);
		usart1_receiveOver = 1;
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
}

///��ʱ��7�жϷ�����
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






