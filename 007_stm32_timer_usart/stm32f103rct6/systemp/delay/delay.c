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
	��������: ��ʱms��λ
	���ֵ: 16777215
	��������ms��λ: 1864ms
*/
void delay_sys_ms(int ms)
{
	u32 tmp;
	SysTick->VAL = 0;         //CNT������ֵ	
	SysTick->LOAD = 9000 * ms;  //9000��ʾ1ms
	SysTick->CTRL |= 1 << 0;    //������ʱ��	
	do
	{
		tmp = SysTick->CTRL; //��ȡ״̬
	} while ((!(tmp & 1 << 16)) && (tmp & 1 << 0));
}

/*
	��������: ��ʱus��λ
*/
void delay_sys_us(int us)
{
	u32 tmp;
	SysTick->VAL = 0;         //CNT������ֵ	
	SysTick->LOAD = 9 * us;     //9��ʾ1us
	SysTick->CTRL |= 1 << 0;    //������ʱ��	
	do
	{
		tmp = SysTick->CTRL; //��ȡ״̬
	} while ((!(tmp & 1 << 16)) && (tmp & 1 << 0));
}


