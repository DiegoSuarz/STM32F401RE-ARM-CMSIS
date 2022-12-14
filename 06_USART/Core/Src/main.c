/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stm32f4xx.h>
#include "defines_pines.h"
#include <stdio.h>
#include <string.h>
#include "delay.h"

#define USART2_TX	A,2
#define USART2_RX	A,3

uint8_t txBuffer[50];
uint8_t rxBuffer[50];
uint16_t len;


/*
 * @brief configura el USART2 a 115200
 */
void USART2_Init(void);

void USART2_GPIO_Init(void);

/**
 * @brief enviar N data por el USART
 */
void USART_SendData(USART_TypeDef * USARTx,uint8_t *pTxBuffer, uint32_t Size);

/**
 * @brief enviar N data por el USART
 */
void USART_ReceiveData(USART_TypeDef * USARTx,uint8_t *pRxBuffer, uint32_t Size);

int main(void)
{
    /* Loop forever */
	delay_init();
	USART2_GPIO_Init();
	USART2_Init();
	len = sprintf((char*)txBuffer,"HOLA MUNDO\r\n");
	USART_SendData(USART2,txBuffer,len); //enviar el mensaje HOLA MUNDO

	USART_ReceiveData(USART2, rxBuffer, 9); //
	len = sprintf((char*)txBuffer, "RX->%s\r\n",rxBuffer); //reenviar lo que se recive por el usart2
	USART_SendData(USART2,txBuffer,len); //enviar el mensaje

	printf("usando la funcion printf del usart%d\n\r",2);
	printf("el numero pi vale: %f\n\r",3.1415);  //habilitar desde los ajustes para la impresion de flotantes
	for(;;);
}

void USART2_Init(void){
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //habilitar el reloj del periferico USART2

	/*
	 * Configurar los parametros de la comunicación serial
	*/

	USART2->CR1 = 0; //sobremuestreo 16; Usart disable; 1 bits start, 8 bits data; Idle Line; paridad off; paridad seleccion off; interrupt off; flag tx inhibet;

	/**
	 * FCK = 16Mhz
	 * BAUD = 115200
	 *
	 * USARTDIV = 16Mhz/(16*115200) = 8.681  Fclk/(Oversampling * baudRate)
	 * FRACTION = 4bits
	 * MANTISA = 12bits = 0x8
	 *
	 * fraction = 0.681 * 16(oversampling) = 10.896 -> 11 (redondeando) -> 0xB
	 */
	 USART2->BRR = 0x8U<<4 | 0xBU; //baudRate para 115200

	 USART2->CR1 |= USART_CR1_TE | USART_CR1_RE; //Habilitando transimision y recepción USART

	 USART2->CR1 |= USART_CR1_UE; //habilitar USART2

	 return;
}

void USART2_GPIO_Init(void){
	RCC -> AHB1ENR |= GPIOX_CLOCK(USART2_TX);
	//PA2 -> TX
	GPIOX_MODER(MODE_ALTER, USART2_TX);   	//pin PA2 modo funcion alternativa
	GPIOX_OSPEEDR(MODE_SPD_VHIGH,USART2_TX); //pin alta velocidad
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL2); //Limpiar los bits de funcion alternativa
	GPIOA->AFR[0] |= (0x7U << 8U); //PA2 modo funcion alternativa para usarlo como USART2_TX (ver datasheet)
	//GPIOA->MODER |= 0x7U << 8; //PA2 modo funcion alternativa para usarlo como USART2_TX (ver datasheet)

	//PA3 -> RX
	GPIOX_MODER(MODE_ALTER, USART2_RX);   	//pin PA2 modo funcion alternativa
	GPIOX_OSPEEDR(MODE_SPD_VHIGH,USART2_RX); //pin alta velocidad
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL3); //Limpiar los bits de funcion alternativa
	GPIOA->AFR[0] |= (0x7U << 12U); //PA2 modo funcion alternativa para usarlo como USART2_RX (ver datasheet)
	//GPIOX_AFR(7U,USART2_RX);

	return;
}

void USART_SendData(USART_TypeDef * USARTx,uint8_t *pTxBuffer, uint32_t Size){
	uint16_t *pdata;
	for(uint32_t i= 0; i<Size;i++){
		//verificar el flag TXE
		while(!(USARTx->SR & USART_SR_TXE));

		//verificar si se ve transmitir en modo de 8bit o 9bit
		if(USARTx->CR1 & USART_CR1_M){
			//enviar el dato
			pdata = (uint16_t*) pTxBuffer;
			USARTx->DR = (*pdata & (uint16_t)0x1FF);
			//Verificar la paridad
			if(!(USARTx->CR1 & USART_CR1_PCE )){
				pTxBuffer++;
				pTxBuffer++;
			}
			else{
				pTxBuffer++;
			}
		}
		else{		//modo de 8bit
			USARTx->DR = (*pTxBuffer & (uint8_t)0xFF);
			pTxBuffer++;
		}
	}
	/*esperamos que TC se ponga a 1*/
	while(!(USARTx->SR & USART_SR_TC));

	return;
}

/**
 * @brief enviar N data por el USART
 */
void USART_ReceiveData(USART_TypeDef * USARTx,uint8_t *pRxBuffer, uint32_t Size){
	for(uint32_t i= 0; i<Size;i++){
		//verificar el flag RXNE
		while(!(USARTx->SR & USART_SR_RXNE));
		//verificar si se va recibir en modo de 8bit o 9bit
		if(USARTx->CR1 & USART_CR1_M){

			/*VERIFICAR LA PARIDAD*/
			if(!(USARTx->CR1 & USART_CR1_PCE )){	//sin paridad
				*((uint16_t*) pRxBuffer) = USARTx->DR & (uint16_t) 0x1FF;
				pRxBuffer++;
				pRxBuffer++;
			}
			else{	//paridad
				*pRxBuffer = USARTx->DR & (uint8_t) 0xFF;
				pRxBuffer++;
			}
		}
		else{
			/*VERIFICAR LA PARIDAD*/
			if(!(USARTx->CR1 & USART_CR1_PCE)){
				*pRxBuffer = USARTx->DR & (uint8_t) 0xFF;
			}
			else{
				*pRxBuffer = USARTx->DR & (uint8_t) 0x7F;
			}
			pRxBuffer++;
		}
	}
	return;
}

/*************************************************/
int __io_putchar(int ch){
	uint8_t c = ch & 0xFF;
	USART_SendData(USART2, &c, 1);
	return ch;
}



