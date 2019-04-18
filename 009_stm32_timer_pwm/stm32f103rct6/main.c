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
	u16 led0pwmval = 0;
	u8 dir = 1;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	led_init();
	key_init();
	beep_init();
	exti_init();
	usart_init(USART1, 115200);
	usart_dma_rx_init();
	set_usart1_nvic();
	base_timer_init(TIM6, 100, 7200);//10ms
	general_timer3_pwm_init(900, 1);
//	general_timer5_capture_init(0xFFFF, 72 - 1);

	while (1)
	{
		delay_sys_ms(10);
		if (dir)led0pwmval++;
		else led0pwmval--;

		if (led0pwmval > 300)dir = 0;
		if (led0pwmval == 0)dir = 1;
		TIM_SetCompare2(TIM3, led0pwmval);
		TIM_SetCompare3(TIM3, led0pwmval);
		TIM_SetCompare4(TIM3, led0pwmval);
	}
}

