#ifndef _TIMER_H__
#define _TIMER_H__

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

u8 base_timer_init(TIM_TypeDef *tim, u32 arr, u32 psc);

#endif // !_TIMER_H__



