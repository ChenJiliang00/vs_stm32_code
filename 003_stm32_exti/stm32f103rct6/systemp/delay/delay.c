#include "./delay.h"

void delay_us(u32 del)
{
	u32 i, j;
	for (i = 0; i < del; i++)
	{
		for (j = 0; j < 8; j++)
		{
			;
		}
	}
}

void delay_ms(u32 del)
{
	u32 i, j;
	for (i = 0; i < del; i++)
	{
		for (j = 0; j < 1000; j++)
			delay_us(1);
	}
}

/*
	函数功能: 延时ms单位
	最大值: 16777215
	最大填入的ms单位: 1864ms
*/
void delay_sys_ms(int ms)
{
	u32 tmp;
	SysTick->VAL = 0;         //CNT计数器值	
	SysTick->LOAD = 9000 * ms;  //9000表示1ms
	SysTick->CTRL |= 1 << 0;    //开启定时器	
	do
	{
		tmp = SysTick->CTRL; //读取状态
	} while ((!(tmp & 1 << 16)) && (tmp & 1 << 0));
}

/*
	函数功能: 延时us单位
*/
void delay_sys_us(int us)
{
	u32 tmp;
	SysTick->VAL = 0;         //CNT计数器值	
	SysTick->LOAD = 9 * us;     //9表示1us
	SysTick->CTRL |= 1 << 0;    //开启定时器	
	do
	{
		tmp = SysTick->CTRL; //读取状态
	} while ((!(tmp & 1 << 16)) && (tmp & 1 << 0));
}


