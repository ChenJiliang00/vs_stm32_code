#include "./key.h"
#include "./systemp/delay/delay.h"

/*
	�������ƣ�void key_init(void)
	�������ܣ�������ʼ������
	�����βΣ���
	�� �� ֵ����
	Ӳ���ӿڣ�
				KEY1 => PA0
				KEY2 => PC13
	������ʽ���ߵ�ƽ����
*/
void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������s
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������s
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*
	�������ƣ�u8 get_key_val(u8 mode)
	�������ܣ���ȡ������ֵ
	�����βΣ�
				u8 mode:0 -> ֧������������Ч  1 -> ֧�ֵ��ΰ�����Ч
	�� �� ֵ��
				0��û�а�������
				1������1����
				2������2����
*/
u8 get_key_val(u8 mode)
{
	static u8 keyFlag = 0;//�������±�־λ
	if (! mode) 
		keyFlag = 0;

	if (((KEY1 == 1) || (KEY2 == 1)) && (keyFlag == 0))
	{
		delay_sys_ms(10);//��ʱ����
		keyFlag = 1;//��ʾ�����Ѿ�����
		if (KEY1 == 1)	return 1;
		else if (KEY2 == 1) return 2;
	}
	else if ((KEY1 == 0) && (KEY2 == 0))
		keyFlag = 0;//�����ɿ�

	return 0;
}
