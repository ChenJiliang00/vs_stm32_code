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
	u8 i = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	led_init();
	key_init();
	beep_init();
	exti_init();
	usart_init(USART1, 115200);
	set_usart1_nvic();
	base_timer_init(TIM6, 5000, 7200);//500ms

	while (1)
	{
		if (TIM_GetFlagStatus(TIM6, TIM_IT_Update) != RESET)
		{
			TIM_ClearFlag(TIM6, TIM_IT_Update);
//			TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
			RGB_R(i); RGB_G(i); RGB_B(i); BEEP(i);
			i = !i;
		}
	}
}

