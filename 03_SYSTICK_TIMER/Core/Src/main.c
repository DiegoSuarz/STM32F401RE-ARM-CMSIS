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
#include "delay.h"

#define BITBAND_PERI(a,b)	((PERIPH_BB_BASE + (a - PERIPH_BASE)*32 + 4*b))

#define PIN_A5				*((volatile unsigned char*)(BITBAND_PERI(GPIOA_BASE + 0x14,5)))

void config_port(void);

int main(void)
{
    delay_init();
    config_port();

	for(;;){
		PIN_A5 = 1;
		delay_us(100);
		PIN_A5 = 0;
		delay_us(100);
	}
}

void config_port(void){

	//configurando PINA5 como salida
	RCC->AHB1ENR |= 1U;
	GPIOA->MODER &=~(GPIO_MODER_MODE5);
	GPIOA->MODER |= GPIO_MODER_MODE5_0;

}