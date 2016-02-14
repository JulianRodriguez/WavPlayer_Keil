/***************************************************************************//**
 *
 * \file    joystick.c
 *
 * \brief   Funci�n de lectura del joystick.
 *
 * Las conexiones entre el joystick el microcontrolador son las siguientes:
 *
 *     	ARRIBA  	--> P2[25]
 *      ABAJO   	--> P2[27]
 *      IZQUIERDA	--> P2[23]
 *      DERECHA		--> P2[26]
 *      CENTRAL		--> P2[22]
 *
 *
 * Los pulsadores del joystick son activos a nivel bajo: cuando se pulsan ponen
 * el correspondiente pin del microcontrolador a 0 y cuando est�n sin pulsar
 * ponen el correspondiente pin a 1.
 *
 */

#include <LPC407x_8x_177x_8x.h>
#include "joystick.h"

/***************************************************************************//**
 *
 * \brief   Leer el joystick y retornar un n�mero diferente seg�n la direcci�n
 *          que se est� pulsando.
 *
 * \return  0 si no se est� pulsando ning�na direcci�n.
 *          1 si el joystick est� pulsado hacia ARRIBA.
 *          2 si el joystick est� pulsado hacia ABAJO.
 *          3 si el joystick est� pulsado hacia la IZQUIERDA.
 *          4 si el joystick est� pulsado hacia la DERECHA.
 *          5 si el joystick est� pulsado hacia el CENTRO.
 */
int leer_joystick(void)
{
    if((LPC_GPIO2->PIN & (1<<25)) == 0) return 1;
		if((LPC_GPIO2->PIN & (1<<27)) == 0) return 2;
		if((LPC_GPIO2->PIN & (1<<23)) == 0) return 3;
		if((LPC_GPIO2->PIN & (1<<26)) == 0) return 4;
		if((LPC_GPIO2->PIN & (1<<22)) == 0){

			return 5;
		}
		return 0;
}
