#include "./exti.h"
#include "./systemp/key/key.h"
#include "./systemp/led/led.h"

/*
	�������ƣ�void exti_init(void)
	�������ܣ������ⲿ�жϳ�ʼ������
	�����βΣ���
	�� �� ֵ����
*/
void exti_init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//ʹ�ܸ��ù���ʱ��
	
	///�����ⲿ�ж���
	///PA0
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//EXTI_Trigger_Falling EXTI_Trigger_Rising_Falling
	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	///PC13
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//EXTI_Trigger_Falling EXTI_Trigger_Rising_Falling
	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	///�����ⲿ�ж����ȼ�
	///�ⲿ�ж���0
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;				//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;			//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);
	///�ⲿ�ж���13
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//��ռ���ȼ�1�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;			//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);
}

//�ⲿ�ж�0������� 
void EXTI0_IRQHandler(void)
{
	static i = 0;
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		if (KEY1 == 1)	 	 //KEY1����
		{
			i = !i;
			BEEP(i);
		}
		EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ  
	}
}

void EXTI15_10_IRQHandler(void)
{
	static j = 1;
	if (EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
		if (KEY2 == 1)	 	 //KEY2����
		{
			j = !j;
			RGB_G(j);
		}
		EXTI_ClearITPendingBit(EXTI_Line13); //���LINE13�ϵ��жϱ�־λ 
	}	
}

