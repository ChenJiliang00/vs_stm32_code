#include "stm32f10x.h"
#include "./systemp/led/led.h"
#include "./systemp/delay/delay.h"
#include "./systemp/key/key.h"
#include "misc.h"

int main(void)
{
//	u8 key = 0, i = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	led_init();
	key_init();
	beep_init();
	exti_init();

	while (1)
	{
/*		key = get_key_val(1);
		if (key)
		{
			i = !i;
			BEEP(i);
		}
*/	}
}

