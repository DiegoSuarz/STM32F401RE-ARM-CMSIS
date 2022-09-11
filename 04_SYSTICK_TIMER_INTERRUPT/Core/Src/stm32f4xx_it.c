/*
 * stm32f4xx_it.c
 *
 *  Created on: 9 set. 2022
 *      Author: dg_su
 */

#include <stm32f4xx.h>

extern volatile uint32_t uwTick;

/*ISR------------------------*/

void SysTick_handler(void){
	if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk){
		uwTick += 1;
	}
}
