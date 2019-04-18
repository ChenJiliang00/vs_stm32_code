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
	TIM_Cmd(tim, ENABLE);
//	TIM_Cmd(tim, DISABLE);
	return 0;
}

/*
	函数名称：void general_timer3_pwm_init(u32 arr, u32 psc)
	函数功能：通用定时器3PWM波形输出初始化函数
	函数形参：
				u32 arr：自动重装载值
				u32 psc：预分配数据
	返 回 值：无
*/
void general_timer3_pwm_init(u32 arr, u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB2Periph_AFIO, ENABLE);

	///定时器3基本配置
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	///定时器3PWM波形配置
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);			//Timer3部分重映射
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;				//选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//输出极性:TIM输出比较极性高
//	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);						//根据T指定的参数初始化外设TIM3 OC2

	///复用IO口初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_1 | GPIO_Pin_0; //TIM_CH2 3 4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO

	///使能RBG灯IO口的使能预装载
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	///使能定时器
	TIM_Cmd(TIM3, ENABLE);                   						//使能定时器	
}

/*
	函数名称：void general_timer3_pwm_init(u32 arr, u32 psc)
	函数功能：通用定时器3PWM波形输出初始化函数
	函数形参：
				u32 arr：自动重装载值
				u32 psc：预分配数据
	返 回 值：无
*/
void general_timer5_capture_init(u32 arr, u32 psc)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//使能TIM5时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能GPIOA时钟

	///定义结构体变量
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	///定时器基本配置
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;// 时钟分频因子 ，配置死区时间时需要用到	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;// 计数器计数模式，设置为向上计数	
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;// 重复计数器的值，没用到不用管	
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);// 初始化定时器

	///输入捕获结构体初始化	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;					// 配置输入捕获的通道，需要根据具体的GPIO来配置	
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;			// 输入捕获信号的极性配置	
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;		// 输入通道和捕获通道的映射关系，有直连和非直连两种	
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;				// 输入的需要被捕获的信号的分频系数	
	TIM_ICInitStructure.TIM_ICFilter = 0;								// 输入的需要被捕获的信号的滤波系数	
	TIM_ICInit(TIM5, &TIM_ICInitStructure);								// 定时器输入捕获初始化

	///初始化TIM5_CH1 => PA0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;							//PA0 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;						//PA0 输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);									//PA0 下拉

	///配置定时器5中断优先级
	TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1, ENABLE);				//允许更新中断 ,允许CC1IE捕获中断	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;						//TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;			//先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);										//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 

	TIM_Cmd(TIM5, ENABLE);
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

///定时器5中断服务函数
u8  TIM5CH1_CAPTURE_STA = 0;	//输入捕获状态		    				
u16	TIM5CH1_CAPTURE_VAL;		//输入捕获值
void TIM5_IRQHandler(void)
{
	if ((TIM5CH1_CAPTURE_STA & 0X80) == 0)//还未成功捕获	
	{
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
		{
			if (TIM5CH1_CAPTURE_STA & 0X40)//已经捕获到高电平了
			{
				if ((TIM5CH1_CAPTURE_STA & 0X3F) == 0X3F)//高电平太长了
				{
					TIM5CH1_CAPTURE_STA |= 0X80;//标记成功捕获了一次
					TIM5CH1_CAPTURE_VAL = 0XFFFF;
				}
				else TIM5CH1_CAPTURE_STA++;
			}
		}
		if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
		{
			if (TIM5CH1_CAPTURE_STA & 0X40)		//捕获到一个下降沿 		
			{
				TIM5CH1_CAPTURE_STA |= 0X80;		//标记成功捕获到一次高电平脉宽
				TIM5CH1_CAPTURE_VAL = TIM_GetCapture1(TIM5);
				TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}
			else  								//还未开始,第一次捕获上升沿
			{
				TIM5CH1_CAPTURE_STA = 0;			//清空
				TIM5CH1_CAPTURE_VAL = 0;
				TIM_SetCounter(TIM5, 0);
				TIM5CH1_CAPTURE_STA |= 0X40;		//标记捕获到了上升沿
				TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
			}
		}
	}
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1 | TIM_IT_Update); //清除中断标志位
}

///定时器6中断服务函数
void TIM6_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		
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






