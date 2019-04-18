#include "./usart.h"
#include <stdio.h>
#include "./systemp/timer/timer.h"

/*
	函数名称：void usart_init(USART_TypeDef *usart,u32 bound)
	函数功能：串口初始化函数
	函数形参：
				USART_TypeDef *usart：串口向量
				u32 bound：波特率
	返 回 值：无
	硬件接口：
				USRT1_TX => PA9
				USRT1_RX => PA10
				USRT2_TX => PA2
				USRT2_RX => PA3
				USRT3_TX => PB10
				USRT3_RX => PB11
*/
void usart_init(USART_TypeDef *usart,u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	if (usart == USART1)
	{
		///1.使能串口1与GPIOA
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
		///2.初始化IO口
		//USART1_TX   GPIOA.9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
		//USART1_RX	  GPIOA.10初始化
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10 
	}
	else if (usart == USART2)
	{
		///1.使能串口2与GPIOA
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART2
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能GPIOA时钟
		///2.初始化IO口
		//USART2_TX   GPIOA.2
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.2
		//USART2_RX	  GPIOA.3
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.3 
	}
	else if(usart == USART3)
	{
		///1.使能串口3与GPIOB
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能GPIOB时钟
		///2.初始化IO口
		//USART3_TX   GPIOB.10
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
		GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB10
		//USART3_RX	  GPIOB.11
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
		GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB11
	}

	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(usart, &USART_InitStructure);		//初始化串口
	USART_Cmd(usart, ENABLE);						//使能串口
}

///================串口中断配置=====================///
///串口1中断配置
void set_usart1_nvic(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);				/* 使能串口接收中断 */
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);				/* 使能串口空闲中断 */
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);				/* 使能串口1DMA接收*/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			/* 配置USART为中断源 */	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	/* 抢断优先级*/	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			/* 子优先级 */	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				/* 使能中断 */	
	NVIC_Init(&NVIC_InitStructure);								/* 初始化配置NVIC */
}
///串口2中断配置
void set_usart2_nvic(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);				/*使能串口接收中断*/
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			/* 配置USART为中断源 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	/* 抢断优先级*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			/* 子优先级 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				/* 使能中断 */
	NVIC_Init(&NVIC_InitStructure);								/* 初始化配置NVIC */
}
///串口3中断配置
void set_usart3_nvic(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);				/*使能串口接收中断*/
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			/* 配置USART为中断源 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	/* 抢断优先级*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			/* 子优先级 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				/* 使能中断 */
	NVIC_Init(&NVIC_InitStructure);								/* 初始化配置NVIC */
}

///串口功能函数
/*
	函数名称：void usart_sendByte(USART_TypeDef *pUSARTx, uint8_t ch)
	函数功能：串口发送一个字符
	函数形参：
				USART_TypeDef *pUSARTx：串口向量
				uint8_t ch：要发送的字符
	返 回 值：无
*/
void usart_sendByte(USART_TypeDef *pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx, ch);
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/*
	函数名称：void usart_sendArray(USART_TypeDef* pUSARTx, uint8_t *array, uint16_t num)
	函数功能：串口发送8位的数组
	函数形参：
				USART_TypeDef *pUSARTx：串口向量
				uint8_t *array：要发送的数组
				uint16_t num：要发送的数组长度
	返 回 值：无
*/
void usart_sendArray(USART_TypeDef* pUSARTx, uint8_t *array, uint16_t num)
{
	uint8_t i;

	for (i = 0; i < num; i ++)
	{
		/* 发送一个字节数据到USART */
		usart_sendByte(pUSARTx, array[i]);
	}
	/* 等待发送完成 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
}

/*
	函数名称：void usart_sendStr(USART_TypeDef *pUSARTx, char *str)
	函数功能：串口发送字符串
	函数形参：
				USART_TypeDef *pUSARTx：串口向量
				char *str：要发送的字符串
	返 回 值：无
*/
void usart_sendStr(USART_TypeDef *pUSARTx, char *str)
{
	unsigned int k = 0;
	do
	{
		usart_sendByte(pUSARTx, *(str + k));
		k ++;
	} while (*(str + k) != '\0');

	/* 等待发送完成 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET){}
}

/*
	函数名称：void usart_sendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch)
	函数功能：串口发送一个16位数
	函数形参：
				USART_TypeDef *pUSARTx：串口向量
				uint16_t ch：要发送16位数据
	返 回 值：无
*/
void usart_sendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;

	/* 取出高八位 */
	temp_h = (ch & 0XFF00) >> 8;
	/* 取出低八位 */
	temp_l = ch & 0XFF;

	/* 发送高八位 */
	USART_SendData(pUSARTx, temp_h);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

	/* 发送低八位 */
	USART_SendData(pUSARTx, temp_l);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

///串口中断服务函数
u8 usart1_rxBuff[RXMAX] = { 0 };
u32 usart1_rxCnt = 0;
u8 usart1_receiveOver = 0;
void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		USART_ReceiveData(USART1);//读取数据 注意：这句必须要，否则不能够清除中断标志位。我也不知道为啥！
		usart1_rxCnt = RXMAX - DMA_GetCurrDataCounter(DMA1_Channel5);	//算出接本帧数据长度

		//***********帧数据处理函数************//
		usart_sendStr(USART1,"The data:\r\n");
		usart_sendArray(USART1, usart1_rxBuff, usart1_rxCnt);
		usart_sendStr(USART1,"\r\nOver! \r\n");
		//*************************************//
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);         //清除中断标志
		MYDMA_Enable(DMA1_Channel5);                   //恢复DMA指针，等待下一次的接收
	}
}

void USART2_IRQHandler(void)
{
	uint8_t ucTemp;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		ucTemp = USART_ReceiveData(USART2);
		USART_SendData(USART2, ucTemp);
	}
}

void USART3_IRQHandler(void)
{
	uint8_t ucTemp;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		ucTemp = USART_ReceiveData(USART3);
		USART_SendData(USART3, ucTemp);
	}
}

///使能串口DMA接收
void usart_dma_rx_init(void)
{
	DMA_InitTypeDef DMA_InitStructure;	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);// 开启DMA时钟

	DMA_DeInit(DMA1_Channel5);													//将DMA的通道5寄存器重设为缺省值  串口1对应的是DMA通道5
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;				//DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)usart1_rxBuff;					//DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;							//数据传输方向，从外设读取发送到内存
	DMA_InitStructure.DMA_BufferSize = RXMAX;									//DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				//数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;								//工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;						//DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);								//根据DMA_InitStruct中指定的参数初始化DMA的通道
	DMA_ClearFlag(DMA1_FLAG_TC5);												// 清除DMA所有标志
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TE, ENABLE);
	DMA_Cmd(DMA1_Channel5, ENABLE);												//正式驱动DMA传输
}

//重新恢复DMA指针
void MYDMA_Enable(DMA_Channel_TypeDef *DMA_CHx)
{
	DMA_Cmd(DMA_CHx, DISABLE);  //关闭USART1 TX DMA1 所指示的通道      
	DMA_SetCurrDataCounter(DMA_CHx, RXMAX);//DMA通道的DMA缓存的大小
	DMA_Cmd(DMA_CHx, ENABLE);  //使能USART1 TX DMA1 所指示的通道 
}











