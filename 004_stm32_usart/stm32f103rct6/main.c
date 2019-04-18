#include "stm32f10x.h"
#include "./systemp/led/led.h"
#include "./systemp/delay/delay.h"
#include "./systemp/key/key.h"
#include "misc.h"
#include "./systemp/usart/usart.h"
#include <stdio.h>

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

	while (1)
	{
//		usart_sendStr(USART1, "1234567890\r\n");
		RGB_R(i);
		delay_sys_ms(500);
		i = !i;
	}
}

