#ifndef __USART_H
#define __USART_H

#include <stdio.h>
#include "stm32f10x.h"
#include "define.h"

void USART_Config  (void);
void Usart_SendByte(USART_TypeDef* pUSARTx, uint8_t data);
void Usart_SendHalfWord(USART_TypeDef* pUSARTx, uint16_t data);
void Usart_SendArray   (USART_TypeDef* pUSARTx, uint8_t *array,uint8_t num);
void Usart_SendStr (USART_TypeDef* pUSARTx, uint8_t *str);

#endif
