#ifndef _TIMER_H__
#define _TIMER_H__

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include <misc.h>

u8 base_timer_init(TIM_TypeDef *tim, u32 arr, u32 psc);
void set_basetimer6_nvic(void);
void set_basetimer7_nvic(void);
void general_timer3_pwm_init(u32 arr, u32 psc);
void general_timer5_capture_init(u32 arr, u32 psc);

#endif // !_TIMER_H__



