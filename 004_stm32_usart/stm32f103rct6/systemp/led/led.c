#include "./led.h"

/*
	�������ƣ�void led_init(void)
	�������ܣ�RGB_LED�Ƴ�ʼ������
	�����βΣ���
	�� �� ֵ����
	Ӳ���ӿڣ�
				RGB_R => PB5
				RGB_G => PB0
				RGB_B => PB1
	������ʽ���͵�ƽ����
*/
void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5);
}

/*
	�������ƣ�void beep_init(void)
	�������ܣ���������ʼ������
	�����βΣ���
	�� �� ֵ����
	Ӳ���ӿڣ�
				BEEP => PA8
	������ʽ���ߵ�ƽ����
*/
void beep_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
}


