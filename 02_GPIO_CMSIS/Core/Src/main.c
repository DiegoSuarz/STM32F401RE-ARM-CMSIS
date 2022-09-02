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
/*
 * PA5->LED
 * PC13 -> user BUTTON
 */

void GPIO_Config(void);

int main(void)
{
	uint16_t bit_state;
    /* Loop forever */
	GPIO_Config();
	GPIOA->ODR |= 1U << 5; //GPIOA -> ODR |= (1<<5);
	GPIOA->ODR &= ~1U << 5;

	GPIOA->BSRR |= GPIO_BSRR_BS5; //pone el alto
	GPIOA->BSRR |= GPIO_BSRR_BR5; //pone el bajo

	for(;;){
		bit_state = (GPIOC -> IDR>>13) & 0x1U; //extraer el valor de pin PC13
		GPIOA->ODR |= bit_state<<5; //si el pulsador esta presionado cambia de estado



	}
}

void GPIO_Config(void){

	//habilitar el reloj
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOAEN; //habilitar el reloj del PUERTO A y el puerto B

	//PA5 ->Salida 01: General purpose output mode MODERy[1:0]
	GPIOA->MODER &= ~(GPIO_MODER_MODE5_1);
	GPIOA->MODER |= GPIO_MODER_MODE5_0; //PA5 modo salida digital

	GPIOA->OTYPER &=~ GPIO_OTYPER_OT5; //modo de salida push pull

	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5; //velocidad very high speed

	GPIOA->PUPDR &=~GPIO_PUPDR_PUPD5; //desactivado pull up/ pull down

/*******************************************************************************/

	//PC13 ->Entrada 01: General purpose output mode MODERy[1:0]
	GPIOC -> MODER &=~ GPIO_MODER_MODE13; //PC13 modo entrada digital

	GPIOC -> PUPDR &= ~GPIO_PUPDR_PUPD13_1;
	GPIOC -> PUPDR |= GPIO_PUPDR_PUPD13_0; // pull up activado
	return;

}
