/*
 * delay_timer.c
 *
 *  Created on: 15 set. 2022
 *      Author: dg_su
 */

#include "delay_timer.h"

volatile uint32_t uwTick_tim;
/*function definition...................*/
/*
 * @brief: funcion para generar retardos en ms
 * @param [ck_psc]: frecuencia de entrada al timer
 */
void Timer4_Init(uint32_t ck_psc){
	uint32_t temp;

	/*habilitar el reloj del bus que esta asociado el timer, para TIMER4 -> APB1*/
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	TIM4->CR1 &= ~TIM_CR1_CEN; //deshabilitar TIMER4

	/*
	 *
	 *Formula:  Tiempo = (PSC+1)(ARR+1)/Fclock_PSC
	*/

	//calculo del prescaler
	temp = ck_psc / 1000000UL; //temp = 16000000/1000000 = 16
	TIM4->PSC = temp-1; //PCS = 15

	//calculo del valor de autorecarga (ARR)
	TIM4->ARR = 1000 - 1; //ARR = 999

	/*generar retardo para 1ms
	 * 	Tiempo = (PSC+1)(ARR+1)/Fclock_PSC
	 * 	Tiempo = (15+1)(999+1)/16000000
	 * 	Tiempo = 0.001 -> 1ms
	 *
	 * 	ARR = (Tiempo*Fclock_PSC)/(PSC+1)-1
	 * 	ARR -> para 1ms ,Fclock 16mhz, prescaler = 15
	 * 	ARR = ((1x10E-3*16x10E+6)/(15+1))-1
	 * 	ARR = 999
	 */

	/*Configurar interrupciones...........*/
	TIM4->DIER |= TIM_DIER_UIE; //habilitar interrupcion por desbordamiento
	NVIC_EnableIRQ(TIM4_IRQn);	//habilitar interrupcion NVIC del CPU para timer4

	//activar timer
	TIM4->CR1 |= TIM_CR1_CEN;

}


/*
 * @brief: genera retardos en ms
 * @param [ms]: retardo en ms deseado
 */
void Timer4_delayms(uint32_t ms){

	uint32_t tickstart;
	uint32_t wait = ms;

	//generar retardo
	tickstart = uwTick_tim;
	while((uwTick_tim - tickstart) < wait);
}


