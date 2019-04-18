#include "stm32f10x.h"
#include "./systemp/led/led.h"
#include "./systemp/delay/delay.h"
#include "./systemp/key/key.h"
#include "misc.h"
#include "./systemp/usart/usart.h"
#include <stdio.h>
#include "./systemp/timer/timer.h"

int main(void)
{
	u32 temp = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	led_init();
	key_init();
	beep_init();
//	exti_init();
	usart_init(USART1, 115200);
	usart_dma_rx_init();
	set_usart1_nvic();
	base_timer_init(TIM6, 100, 7200);//10ms
	general_timer3_pwm_init(900, 1);
	general_timer5_capture_init(0xFFFF, 72 - 1);

	while (1)
	{
		delay_sys_ms(10);
//		TIM_SetCompare2(TIM3, TIM_GetCapture2(TIM3) + 1);
//		if (TIM_GetCapture2(TIM3) == 300)TIM_SetCompare2(TIM3, 0);

		if (TIM5CH1_CAPTURE_STA & 0X80)//成功捕获到了一次上升沿
		{
			temp = TIM5CH1_CAPTURE_STA & 0X3F;
			temp *= 65536;//溢出时间总和
			temp += TIM5CH1_CAPTURE_VAL;//得到总的高电平时间
//			printf("HIGH:%d us\r\n", temp);//打印总的高点平时间
			TIM5CH1_CAPTURE_STA = 0;//开启下一次捕获
		}
	}
}

