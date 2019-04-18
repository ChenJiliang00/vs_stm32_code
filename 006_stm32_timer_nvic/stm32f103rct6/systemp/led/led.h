#ifndef _LED_H__
#define _LED_H__

#include "stm32f10x.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

void led_init(void);
void beep_init(void);

#define RGB_R(x) if(x){GPIO_SetBits(GPIOB, GPIO_Pin_5);}else{GPIO_ResetBits(GPIOB, GPIO_Pin_5);}
#define RGB_B(x) if(x){GPIO_SetBits(GPIOB, GPIO_Pin_1);}else{GPIO_ResetBits(GPIOB, GPIO_Pin_1);}
#define RGB_G(x) if(x){GPIO_SetBits(GPIOB, GPIO_Pin_0);}else{GPIO_ResetBits(GPIOB, GPIO_Pin_0);}
#define BEEP(x)  if(x){GPIO_SetBits(GPIOA, GPIO_Pin_8);}else{GPIO_ResetBits(GPIOA, GPIO_Pin_8);}

#endif


