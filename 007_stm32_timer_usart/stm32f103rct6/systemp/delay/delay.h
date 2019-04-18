#ifndef _DELAY_H__
#define _DELAY_H__

#include "stm32f10x.h"

void delay_us(u32 del);
void delay_ms(u32 del);
void delay_sys_ms(int ms);
void delay_sys_us(int us);

#endif
