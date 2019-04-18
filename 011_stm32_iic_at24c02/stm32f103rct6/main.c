#include "stm32f10x.h"
#include "./systemp/led/led.h"
#include "./systemp/delay/delay.h"
#include "./systemp/key/key.h"
#include "misc.h"
#include "./systemp/usart/usart.h"
#include <stdio.h>
#include "./systemp/timer/timer.h"
#include "./systemp/iic/iic.h"

#define  EEP_Firstpage      0x00
uint8_t I2c_Buf_Write[256];
uint8_t I2c_Buf_Read[256];
uint8_t I2C_Test(void);

int main(void)
{
	u32 temp = 0;
	u8 i = 0;
	u8 write_at24c02[] = { 0x00,0x11,0x22,0x33,0x44,0x55,0x66 };
	u8 read_at24c02[10] = { 0x00 };
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	led_init();
	key_init();
	beep_init();
//	exti_init();
	usart_init(USART1, 115200);
	usart_dma_rx_init();
	set_usart1_nvic();
	base_timer_init(TIM6, 100, 7200);//10ms
	general_timer3_pwm_init(900, 1);
	general_timer5_capture_init(0xFFFF, 72 - 1);
	I2C_EE_Init();

	//EEPROM ��д����
	if (I2C_Test() == 1)
	{
		RGB_G(0);
	}
	else
	{
		RGB_R(0);
	}

	while (1)
	{
		delay_sys_ms(10);
//		TIM_SetCompare2(TIM3, TIM_GetCapture2(TIM3) + 1);
//		if (TIM_GetCapture2(TIM3) == 300)TIM_SetCompare2(TIM3, 0);

		if (TIM5CH1_CAPTURE_STA & 0X80)//�ɹ�������һ��������
		{
			temp = TIM5CH1_CAPTURE_STA & 0X3F;
			temp *= 65536;//���ʱ���ܺ�
			temp += TIM5CH1_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
//			printf("HIGH:%d us\r\n", temp);//��ӡ�ܵĸߵ�ƽʱ��
			TIM5CH1_CAPTURE_STA = 0;//������һ�β���
		}
	}
}

/**
  * @brief  I2C(AT24C02)��д����
  * @param  ��
  * @retval ��������1���쳣����0
  */
uint8_t I2C_Test(void)
{
	uint16_t i;

//	printf("д�������\n\r");
	usart_sendStr(USART1,"д�������\n\r");

	for (i = 0; i <= 255; i++) //��仺��
	{
		I2c_Buf_Write[i] = i;

//		printf("0x%02X ", I2c_Buf_Write[i]);
		usart_sendByte(USART1, I2c_Buf_Write[i]);
		if (i % 16 == 15)
//			printf("\n\r");
			usart_sendStr(USART1, "\n\r");
	}

	//��I2c_Buf_Write��˳�����������д��EERPOM�� 
	I2C_EE_BufferWrite(I2c_Buf_Write, EEP_Firstpage, 256);

//	EEPROM_INFO("\n\rд�ɹ�\n\r");
	usart_sendStr(USART1, "n\rд�ɹ�\n\r");
//	EEPROM_INFO("\n\r����������\n\r");
	usart_sendStr(USART1, "n\rд�ɹ�\n\r");
	//��EEPROM��������˳�򱣳ֵ�I2c_Buf_Read��
	I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage, 256);

	//��I2c_Buf_Read�е�����ͨ�����ڴ�ӡ
	for (i = 0; i < 256; i++)
	{
		if (I2c_Buf_Read[i] != I2c_Buf_Write[i])
		{
//			EEPROM_ERROR("0x%02X ", I2c_Buf_Read[i]);
			usart_sendByte(USART1, I2c_Buf_Read[i]);
//			EEPROM_ERROR("����:I2C EEPROMд������������ݲ�һ��\n\r");
			usart_sendStr(USART1, "����:I2C EEPROMд������������ݲ�һ��\n\r");
			return 0;
		}
//		printf("0x%02X ", I2c_Buf_Read[i]);
		usart_sendByte(USART1, I2c_Buf_Read[i]);
		if (i % 16 == 15)
//			printf("\n\r");
			usart_sendStr(USART1, "\n\r");

	}
//	EEPROM_INFO("I2C(AT24C02)��д���Գɹ�\n\r");
	usart_sendStr(USART1, "I2C(AT24C02)��д���Գɹ�\n\r");
	return 1;
}


