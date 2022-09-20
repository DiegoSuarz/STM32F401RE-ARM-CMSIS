/*
 * delay.c
 *
 *  Created on: 3 set. 2022
 *      Author: dg_su
 */

#include "delay.h"

/* Configuración del SYSTICK TIMER
 * 1.- Desactivar el contador. ENABLE = 0
 * 2- Cargar el valor de RELOAD
 * 3- Escribir cualquier valor en la cuenta para que se ponga a 0.
 * 4- Configurar los registros de control y estado, incluyendo la activación.
 *
 * Ejm: para 1ms
 * 		ValorDeRecarga = SysTickCounterClock x TemporizaciónDeseada(s)
 * 		ValorDeRecarga = 16000000 x 0.001
 * 		ValorDeRecarga = 16000
 */

void delay_init(void){
	uint32_t temp;

	SysTick->CTRL = 0; //deshabilita la systick
	SystemCoreClockUpdate(); //actualizo el valor del FOSC

#if(DELAY_MODE == 0)  //trabaja en us y ms
	temp = SystemCoreClock / 1000000UL;

#elif(DELAY_MODE == 1)// solo ms
	temp = SystemCoreClock / 1000UL;

#else
#error "Eleccion incorrecta"
#endif

	SysTick ->LOAD = temp - 1; //Cargar el valor de RELOAD
	SysTick ->CTRL |= SysTick_CTRL_CLKSOURCE_Msk; //toma la fuente de relog SYSCLK
	SysTick ->CTRL |= SysTick_CTRL_ENABLE_Msk; //habilitar el conteo
	return;

}

#if (DELAY_MODE == 0)

void delay_us(uint32_t delay){
	uint32_t cont;
	SysTick->VAL = 0; //inicializamos a 0 el contador
	for(cont=0; cont<delay; cont++){
		while(!(SysTick->CTRL & 1U << 16));
	}
	return;
}
#endif

void delay_ms(uint32_t delay){
	uint32_t cont;
#if(DELAY_MODE == 0)
	for(cont=0;cont<delay;cont++){
		delay_us(1000);
	}
#elif(DELAY_MODE == 1)
	SysTick->VAL =0;
	for(cont=0;cont<delay;cont++){
		while(!(SysTick->CTRL & 1U<<16)); //genera un retardo de 1us
	}
#else
#error "ERROR DE ELECCION"
#endif
	return;
}
