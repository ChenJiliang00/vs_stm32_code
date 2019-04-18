#ifndef _KEY_H__
#define _KEY_H__

#include "stm32f10x.h"
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

#define KEY1 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)
#define KEY2 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)

void key_init(void);
u8 get_key_val(u8 mode);

#endif
