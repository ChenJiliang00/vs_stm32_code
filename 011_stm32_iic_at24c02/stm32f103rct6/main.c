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

	//EEPROM 读写测试
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

		if (TIM5CH1_CAPTURE_STA & 0X80)//成功捕获到了一次上升沿
		{
			temp = TIM5CH1_CAPTURE_STA & 0X3F;
			temp *= 65536;//溢出时间总和
			temp += TIM5CH1_CAPTURE_VAL;//得到总的高电平时间
//			printf("HIGH:%d us\r\n", temp);//打印总的高点平时间
			TIM5CH1_CAPTURE_STA = 0;//开启下一次捕获
		}
	}
}

/**
  * @brief  I2C(AT24C02)读写测试
  * @param  无
  * @retval 正常返回1，异常返回0
  */
uint8_t I2C_Test(void)
{
	uint16_t i;

//	printf("写入的数据\n\r");
	usart_sendStr(USART1,"写入的数据\n\r");

	for (i = 0; i <= 255; i++) //填充缓冲
	{
		I2c_Buf_Write[i] = i;

//		printf("0x%02X ", I2c_Buf_Write[i]);
		usart_sendByte(USART1, I2c_Buf_Write[i]);
		if (i % 16 == 15)
//			printf("\n\r");
			usart_sendStr(USART1, "\n\r");
	}

	//将I2c_Buf_Write中顺序递增的数据写入EERPOM中 
	I2C_EE_BufferWrite(I2c_Buf_Write, EEP_Firstpage, 256);

//	EEPROM_INFO("\n\r写成功\n\r");
	usart_sendStr(USART1, "n\r写成功\n\r");
//	EEPROM_INFO("\n\r读出的数据\n\r");
	usart_sendStr(USART1, "n\r写成功\n\r");
	//将EEPROM读出数据顺序保持到I2c_Buf_Read中
	I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage, 256);

	//将I2c_Buf_Read中的数据通过串口打印
	for (i = 0; i < 256; i++)
	{
		if (I2c_Buf_Read[i] != I2c_Buf_Write[i])
		{
//			EEPROM_ERROR("0x%02X ", I2c_Buf_Read[i]);
			usart_sendByte(USART1, I2c_Buf_Read[i]);
//			EEPROM_ERROR("错误:I2C EEPROM写入与读出的数据不一致\n\r");
			usart_sendStr(USART1, "错误:I2C EEPROM写入与读出的数据不一致\n\r");
			return 0;
		}
//		printf("0x%02X ", I2c_Buf_Read[i]);
		usart_sendByte(USART1, I2c_Buf_Read[i]);
		if (i % 16 == 15)
//			printf("\n\r");
			usart_sendStr(USART1, "\n\r");

	}
//	EEPROM_INFO("I2C(AT24C02)读写测试成功\n\r");
	usart_sendStr(USART1, "I2C(AT24C02)读写测试成功\n\r");
	return 1;
}


