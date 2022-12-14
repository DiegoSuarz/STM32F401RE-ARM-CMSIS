/*
 * delay.c
 *
 *  Created on: 3 set. 2022
 *      Author: dg_su
 */

#include "delay.h"


volatile uint32_t uwTick = 0; //esta variable puede cambiar tanto por software y por hardware

void delay_init_interrupt(void){
	uint32_t temp;

	SysTick->CTRL = 0; //deshabilita la systick
	SystemCoreClockUpdate(); //actualizo el valor del FOSC
	temp=SystemCoreClock/1000UL;
	SysTick ->LOAD = temp - 1; //Cargar el valor de RELOAD
	SysTick ->CTRL |= SysTick_CTRL_CLKSOURCE_Msk; //toma la fuente de relog SYSCLK


	//habilitar la interrupcion de systick
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; //habilitar interrupcion systick

	SysTick ->CTRL |= SysTick_CTRL_ENABLE_Msk; //habilitar el conteo
	return;

}

static uint32_t GetTick(void){
	return uwTick;
}

/*
 * @brief funcion permite generar retardos en ms
 * @param [delay]: tiemrpo de retardo deseado
 */
void delay_ms(uint32_t delay){
	uint32_t tickstart = GetTick();
	uint32_t wait = delay;

	//generarndo retardo:
	while((GetTick() - tickstart)<wait);
	return;
}


/*brief funcion similar al milis en arduino*/
uint32_t millis(void){
	return GetTick();
}

