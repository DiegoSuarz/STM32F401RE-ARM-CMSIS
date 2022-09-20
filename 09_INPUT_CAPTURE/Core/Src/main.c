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
#include "delay.h"

#define TMR2_CH1	A,0

float duty=0.0;
float frecuencia=0.0;
uint32_t capture[2];

void TMR2_InputCapture_PWMmode(void);

void getDutyFreq(void);

void GPIO_Init(void);


int main(void)
{
	GPIO_Init();
	TMR2_InputCapture_PWMmode();
    /* Loop forever */
	for(;;){
		getDutyFreq();
		delay_ms(50);
	}
}

void TMR2_InputCapture_PWMmode(void){

	//configurar el pin asignado al canal 1
	RCC->AHB1ENR |= GPIOX_CLOCK(TMR2_CH1); //habilitando reloj para PA5
	GPIOX_MODER(MODE_ALTER,TMR2_CH1);  //configurando PA5 modo funcion alternativa
	GPIOX_AFR(1,TMR2_CH1);	//configurando PA5 con funcion alternativa AF01, trabaja con tmr2
	GPIOX_OSPEEDR(MODE_SPD_VHIGH,TMR2_CH1); //configurando la velocidad del pin PA5 alta velocidad

	//configurar el timer
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //habilitar el reloj del timer2
	TIM2->CR1 = 0; //configuracion del tmr2 ver datasheet
	TIM2->PSC = 4 - 1; //prescaler: 4
	TIM2->ARR = 0xFFFFFFFF; //valor maximo de autorecarga de 32bits

	//Configurar el modo de funcionamiento del canal
	TIM2->CCMR1 &= ~TIM_CCMR1_CC1S;
	TIM2->CCMR1 |= TIM_CCMR1_CC1S_1; //input capture configurado como entrada y mapeado a tmr2

	TIM2->CCER &=~ (TIM_CCER_CC1P | TIM_CCER_CC1NP); //flanco de deteccion ascendente

	TIM2->CCMR1 &=~ TIM_CCMR1_CC2S;
	TIM2->CCMR1 |= TIM_CCMR1_CC2S_1; //input capture configurado como entrada y mapeado a tmr2

	TIM2->CCER |= TIM_CCER_CC1P;
	TIM2->CCER &=~ TIM_CCER_CC1P; //flanco de deteccion descendente

	TIM2->SMCR = 0x5<<4; //seleccionamos el trigger correcto

	TIM2->SMCR |= 0x4U; //reset mode

	//habilitar los canales:
	TIM2->CCER |= TIM_CCER_CC2E | TIM_CCER_CC1E; //habilitar input compare cc1s(rising), cc2s(falling)

	/*******************************************************************************/
	duty = ((float)capture[0]/capture[1])*100; //dutycycle
	frecuencia = ((float)SystemCoreClock/(4))* capture[1];

	//desactivar el periferico;
	TIM2->CR1 &=~ TIM_CR1_CEN;

}

void GPIO_Init(void){
	RCC->AHB1ENR |= GPIOX_CLOCK(LED); //habilitando reloj para pin PA5 (led)
	GPIOX_MODER(MODE_OUT,LED); //PA5 salida
}

void getDutyFreq(void){


	//habilitar el  conteo
	TIM2->SR &=~ (TIM_SR_CC1IF | TIM_SR_CC2IF); //borrar la bandera de interrupcion
	TIM2->CR1 |=  TIM_CR1_CEN;

	//esperar el primer flanco ascendente:
	while(!(TIM2-> SR & TIM_SR_CC1IF)); //esperar a que se realice una captura
	TIM2-> SR &=~( TIM_SR_CC1IF); //borrar la bandera de interrupcion

	//esperar al flanco descendente:
	while(!(TIM2-> SR & TIM_SR_CC2IF)); //esperar a que se realice una captura
	TIM2-> SR &=~( TIM_SR_CC2IF); //borrar la bandera de interrupcion

	capture[0]=TIM2->CCR2;   //guardamos el valor del registro CCR2

	//esperar el primer flanco ascendente:
	while(!(TIM2-> SR & TIM_SR_CC1IF)); //esperar a que se realice una captura
	TIM2-> SR &=~( TIM_SR_CC1IF); //borrar la bandera de interrupcion
	capture[1]=TIM2->CCR1;

}