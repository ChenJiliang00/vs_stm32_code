#include "./led.h"

/*
	函数名称：void led_init(void)
	函数功能：RGB_LED灯初始化函数
	函数形参：无
	返 回 值：无
	硬件接口：
				RGB_R => PB5
				RGB_G => PB0
				RGB_B => PB1
	驱动方式：低电平驱动
*/
void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 || GPIO_Pin_1 || GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_0 || GPIO_Pin_1 || GPIO_Pin_5);
}

