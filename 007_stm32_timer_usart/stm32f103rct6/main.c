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
	base_timer_init(TIM6, 100, 7200);//10ms
	base_timer_init(TIM7, 5000, 7200);//500ms
	TIM_Cmd(TIM7, ENABLE);
	set_basetimer6_nvic();
	set_basetimer7_nvic();

	while (1)
	{
		if (usart1_receiveOver == 1)
		{
			usart1_rxBuff[usart1_rxCnt++] = '\r';
			usart1_rxBuff[usart1_rxCnt++] = '\n';
			usart1_rxBuff[usart1_rxCnt++] = '\0';
			usart_sendStr(USART1, usart1_rxBuff);
			usart1_receiveOver = 0;
			usart1_rxCnt = 0;
		}
	}
}

