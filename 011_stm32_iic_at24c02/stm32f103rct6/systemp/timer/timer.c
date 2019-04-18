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
	TIM_Cmd(tim, ENABLE);
//	TIM_Cmd(tim, DISABLE);
	return 0;
}

/*
	�������ƣ�void general_timer3_pwm_init(u32 arr, u32 psc)
	�������ܣ�ͨ�ö�ʱ��3PWM���������ʼ������
	�����βΣ�
				u32 arr���Զ���װ��ֵ
				u32 psc��Ԥ��������
	�� �� ֵ����
*/
void general_timer3_pwm_init(u32 arr, u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB2Periph_AFIO, ENABLE);

	///��ʱ��3��������
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	///��ʱ��3PWM��������
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);			//Timer3������ӳ��
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;				//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//�������:TIM����Ƚϼ��Ը�
//	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);						//����Tָ���Ĳ�����ʼ������TIM3 OC2

	///����IO�ڳ�ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_1 | GPIO_Pin_0; //TIM_CH2 3 4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO

	///ʹ��RBG��IO�ڵ�ʹ��Ԥװ��
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	///ʹ�ܶ�ʱ��
	TIM_Cmd(TIM3, ENABLE);                   						//ʹ�ܶ�ʱ��	
}

/*
	�������ƣ�void general_timer3_pwm_init(u32 arr, u32 psc)
	�������ܣ�ͨ�ö�ʱ��3PWM���������ʼ������
	�����βΣ�
				u32 arr���Զ���װ��ֵ
				u32 psc��Ԥ��������
	�� �� ֵ����
*/
void general_timer5_capture_init(u32 arr, u32 psc)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//ʹ��TIM5ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��GPIOAʱ��

	///����ṹ�����
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	///��ʱ����������
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;// ����������ģʽ������Ϊ���ϼ���	
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;// �ظ���������ֵ��û�õ����ù�	
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);// ��ʼ����ʱ��

	///���벶��ṹ���ʼ��	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;					// �������벶���ͨ������Ҫ���ݾ����GPIO������	
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;			// ���벶���źŵļ�������	
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;		// ����ͨ���Ͳ���ͨ����ӳ���ϵ����ֱ���ͷ�ֱ������	
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;				// �������Ҫ��������źŵķ�Ƶϵ��	
	TIM_ICInitStructure.TIM_ICFilter = 0;								// �������Ҫ��������źŵ��˲�ϵ��	
	TIM_ICInit(TIM5, &TIM_ICInitStructure);								// ��ʱ�����벶���ʼ��

	///��ʼ��TIM5_CH1 => PA0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;							//PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;						//PA0 ����  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);									//PA0 ����

	///���ö�ʱ��5�ж����ȼ�
	TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1, ENABLE);				//��������ж� ,����CC1IE�����ж�	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;						//TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;			//��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);										//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 

	TIM_Cmd(TIM5, ENABLE);
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

///��ʱ��5�жϷ�����
u8  TIM5CH1_CAPTURE_STA = 0;	//���벶��״̬		    				
u16	TIM5CH1_CAPTURE_VAL;		//���벶��ֵ
void TIM5_IRQHandler(void)
{
	if ((TIM5CH1_CAPTURE_STA & 0X80) == 0)//��δ�ɹ�����	
	{
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
		{
			if (TIM5CH1_CAPTURE_STA & 0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if ((TIM5CH1_CAPTURE_STA & 0X3F) == 0X3F)//�ߵ�ƽ̫����
				{
					TIM5CH1_CAPTURE_STA |= 0X80;//��ǳɹ�������һ��
					TIM5CH1_CAPTURE_VAL = 0XFFFF;
				}
				else TIM5CH1_CAPTURE_STA++;
			}
		}
		if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//����1���������¼�
		{
			if (TIM5CH1_CAPTURE_STA & 0X40)		//����һ���½��� 		
			{
				TIM5CH1_CAPTURE_STA |= 0X80;		//��ǳɹ�����һ�θߵ�ƽ����
				TIM5CH1_CAPTURE_VAL = TIM_GetCapture1(TIM5);
				TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}
			else  								//��δ��ʼ,��һ�β���������
			{
				TIM5CH1_CAPTURE_STA = 0;			//���
				TIM5CH1_CAPTURE_VAL = 0;
				TIM_SetCounter(TIM5, 0);
				TIM5CH1_CAPTURE_STA |= 0X40;		//��ǲ�����������
				TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			}
		}
	}
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1 | TIM_IT_Update); //����жϱ�־λ
}

///��ʱ��6�жϷ�����
void TIM6_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		
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






