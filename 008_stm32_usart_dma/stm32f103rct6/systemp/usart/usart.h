#ifndef _USART_H__
#define _USART_H__

#include "stm32f10x.h"
#include <stm32f10x_usart.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <misc.h>
#include <stm32f10x_dma.h>

#define RXMAX 512

extern u8 usart1_rxBuff[RXMAX];
extern u32 usart1_rxCnt;
extern u8 usart1_receiveOver;

void usart_init(USART_TypeDef *usart, u32 bound);
void usart_sendByte(USART_TypeDef *pUSARTx, uint8_t ch);
void usart_sendArray(USART_TypeDef* pUSARTx, uint8_t *array, uint16_t num);
void usart_sendStr(USART_TypeDef *pUSARTx, char *str);
void usart_sendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch);
void set_usart1_nvic(void);
void set_usart2_nvic(void);
void set_usart3_nvic(void);
void usart_dma_rx_init(void);
void MYDMA_Enable(DMA_Channel_TypeDef *DMA_CHx);

#endif // !_USART_H__

