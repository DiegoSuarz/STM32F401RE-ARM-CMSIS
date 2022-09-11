/*
 * delay.h
 *
 *  Created on: 3 set. 2022
 *      Author: dg_su
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_

#include <stm32f4xx.h>

/*
 *
 * 0: generar retardos en us y ms
 * 1: generar solo retardos en ms
 * */

#define DELAY_MODE 0

void delay_init_interrupt(void);

#if (DELAY_MODE == 0)
/*funcion encargada de generar retardos en us
 * @param [delay]: tiempo de retarde en us deseado
 */
void delay_us(uint32_t delay);
#endif


/*
 * @brief funcion encargada de generar retardos en ms
 * @param [delay]: tiempo de retardo en ms deseado
*/

void delay_ms(uint32_t delay);

uint32_t millis(void); //devuelve un numero de cuentas similar al de arduino

#endif /* INC_DELAY_H_ */
