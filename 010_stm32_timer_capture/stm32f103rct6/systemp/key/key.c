#include "./key.h"
#include "./systemp/delay/delay.h"

/*
	函数名称：void key_init(void)
	函数功能：按键初始化函数
	函数形参：无
	返 回 值：无
	硬件接口：
				KEY1 => PA0
				KEY2 => PC13
	驱动方式：高电平驱动
*/
void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入s
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入s
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*
	函数名称：u8 get_key_val(u8 mode)
	函数功能：获取按键的值
	函数形参：
				u8 mode:0 -> 支持连续按下有效  1 -> 支持单次按下有效
	返 回 值：
				0：没有按键按下
				1：按键1按下
				2：按键2按下
*/
u8 get_key_val(u8 mode)
{
	static u8 keyFlag = 0;//按键按下标志位
	if (! mode) 
		keyFlag = 0;

	if (((KEY1 == 1) || (KEY2 == 1)) && (keyFlag == 0))
	{
		delay_sys_ms(10);//延时消抖
		keyFlag = 1;//表示按键已经按下
		if (KEY1 == 1)	return 1;
		else if (KEY2 == 1) return 2;
	}
	else if ((KEY1 == 0) && (KEY2 == 0))
		keyFlag = 0;//按键松开

	return 0;
}
