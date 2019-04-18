#include "./usart.h"
#include <stdio.h>
#include "./systemp/timer/timer.h"

/*
	�������ƣ�void usart_init(USART_TypeDef *usart,u32 bound)
	�������ܣ����ڳ�ʼ������
	�����βΣ�
				USART_TypeDef *usart����������
				u32 bound��������
	�� �� ֵ����
	Ӳ���ӿڣ�
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
		///1.ʹ�ܴ���1��GPIOA
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
		///2.��ʼ��IO��
		//USART1_TX   GPIOA.9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
		//USART1_RX	  GPIOA.10��ʼ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10 
	}
	else if (usart == USART2)
	{
		///1.ʹ�ܴ���2��GPIOA
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//ʹ��USART2
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��GPIOAʱ��
		///2.��ʼ��IO��
		//USART2_TX   GPIOA.2
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.2
		//USART2_RX	  GPIOA.3
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.3 
	}
	else if(usart == USART3)
	{
		///1.ʹ�ܴ���3��GPIOB
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART3
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��GPIOBʱ��
		///2.��ʼ��IO��
		//USART3_TX   GPIOB.10
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
		GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB10
		//USART3_RX	  GPIOB.11
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
		GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB11
	}

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(usart, &USART_InitStructure);		//��ʼ������
	USART_Cmd(usart, ENABLE);						//ʹ�ܴ���
}

///================�����ж�����=====================///
///����1�ж�����
void set_usart1_nvic(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);				/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);				/* ʹ�ܴ��ڿ����ж� */
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);				/* ʹ�ܴ���1DMA����*/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			/* ����USARTΪ�ж�Դ */	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	/* �������ȼ�*/	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			/* �����ȼ� */	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				/* ʹ���ж� */	
	NVIC_Init(&NVIC_InitStructure);								/* ��ʼ������NVIC */
}
///����2�ж�����
void set_usart2_nvic(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);				/*ʹ�ܴ��ڽ����ж�*/
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			/* ����USARTΪ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	/* �������ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			/* �����ȼ� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				/* ʹ���ж� */
	NVIC_Init(&NVIC_InitStructure);								/* ��ʼ������NVIC */
}
///����3�ж�����
void set_usart3_nvic(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);				/*ʹ�ܴ��ڽ����ж�*/
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			/* ����USARTΪ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	/* �������ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			/* �����ȼ� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				/* ʹ���ж� */
	NVIC_Init(&NVIC_InitStructure);								/* ��ʼ������NVIC */
}

///���ڹ��ܺ���
/*
	�������ƣ�void usart_sendByte(USART_TypeDef *pUSARTx, uint8_t ch)
	�������ܣ����ڷ���һ���ַ�
	�����βΣ�
				USART_TypeDef *pUSARTx����������
				uint8_t ch��Ҫ���͵��ַ�
	�� �� ֵ����
*/
void usart_sendByte(USART_TypeDef *pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx, ch);
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/*
	�������ƣ�void usart_sendArray(USART_TypeDef* pUSARTx, uint8_t *array, uint16_t num)
	�������ܣ����ڷ���8λ������
	�����βΣ�
				USART_TypeDef *pUSARTx����������
				uint8_t *array��Ҫ���͵�����
				uint16_t num��Ҫ���͵����鳤��
	�� �� ֵ����
*/
void usart_sendArray(USART_TypeDef* pUSARTx, uint8_t *array, uint16_t num)
{
	uint8_t i;

	for (i = 0; i < num; i ++)
	{
		/* ����һ���ֽ����ݵ�USART */
		usart_sendByte(pUSARTx, array[i]);
	}
	/* �ȴ�������� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
}

/*
	�������ƣ�void usart_sendStr(USART_TypeDef *pUSARTx, char *str)
	�������ܣ����ڷ����ַ���
	�����βΣ�
				USART_TypeDef *pUSARTx����������
				char *str��Ҫ���͵��ַ���
	�� �� ֵ����
*/
void usart_sendStr(USART_TypeDef *pUSARTx, char *str)
{
	unsigned int k = 0;
	do
	{
		usart_sendByte(pUSARTx, *(str + k));
		k ++;
	} while (*(str + k) != '\0');

	/* �ȴ�������� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET){}
}

/*
	�������ƣ�void usart_sendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch)
	�������ܣ����ڷ���һ��16λ��
	�����βΣ�
				USART_TypeDef *pUSARTx����������
				uint16_t ch��Ҫ����16λ����
	�� �� ֵ����
*/
void usart_sendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;

	/* ȡ���߰�λ */
	temp_h = (ch & 0XFF00) >> 8;
	/* ȡ���Ͱ�λ */
	temp_l = ch & 0XFF;

	/* ���͸߰�λ */
	USART_SendData(pUSARTx, temp_h);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx, temp_l);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

///�����жϷ�����
u8 usart1_rxBuff[RXMAX] = { 0 };
u32 usart1_rxCnt = 0;
u8 usart1_receiveOver = 0;
void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		USART_ReceiveData(USART1);//��ȡ���� ע�⣺������Ҫ�������ܹ�����жϱ�־λ����Ҳ��֪��Ϊɶ��
		usart1_rxCnt = RXMAX - DMA_GetCurrDataCounter(DMA1_Channel5);	//����ӱ�֡���ݳ���

		//***********֡���ݴ�����************//
		usart_sendStr(USART1,"The data:\r\n");
		usart_sendArray(USART1, usart1_rxBuff, usart1_rxCnt);
		usart_sendStr(USART1,"\r\nOver! \r\n");
		//*************************************//
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);         //����жϱ�־
		MYDMA_Enable(DMA1_Channel5);                   //�ָ�DMAָ�룬�ȴ���һ�εĽ���
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

///ʹ�ܴ���DMA����
void usart_dma_rx_init(void)
{
	DMA_InitTypeDef DMA_InitStructure;	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);// ����DMAʱ��

	DMA_DeInit(DMA1_Channel5);													//��DMA��ͨ��5�Ĵ�������Ϊȱʡֵ  ����1��Ӧ����DMAͨ��5
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;				//DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)usart1_rxBuff;					//DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;							//���ݴ��䷽�򣬴������ȡ���͵��ڴ�
	DMA_InitStructure.DMA_BufferSize = RXMAX;									//DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				//���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;								//��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;						//DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);								//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��
	DMA_ClearFlag(DMA1_FLAG_TC5);												// ���DMA���б�־
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TE, ENABLE);
	DMA_Cmd(DMA1_Channel5, ENABLE);												//��ʽ����DMA����
}

//���»ָ�DMAָ��
void MYDMA_Enable(DMA_Channel_TypeDef *DMA_CHx)
{
	DMA_Cmd(DMA_CHx, DISABLE);  //�ر�USART1 TX DMA1 ��ָʾ��ͨ��      
	DMA_SetCurrDataCounter(DMA_CHx, RXMAX);//DMAͨ����DMA����Ĵ�С
	DMA_Cmd(DMA_CHx, ENABLE);  //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
}











