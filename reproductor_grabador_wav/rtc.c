#include <LPC407x_8x_177x_8x.h>
#include "rtc.h"

/*==============================================================================
 * Función:
 *------------------------------------------------------------------------------
 * Utilidad: 
 *
 * Entrada: 
 *
 * Salida:  
 *==============================================================================
 */
void rtc_inicializar(void)
{
    LPC_RTC->CCR = (1 << 4) | 1;
}

/*==============================================================================
 * Función:
 *------------------------------------------------------------------------------
 * Utilidad: 
 *
 * Entrada: 
 *
 * Salida:  
 *==============================================================================
 */
void rtc_leer_hora(unsigned int *horas,  unsigned int *minutos,
                   unsigned int *segundos)
{
    int temp;

    temp = LPC_RTC->CTIME0;

    *segundos = temp & 0x3F;
    *minutos = (temp >> 8) & 0x3F;
    *horas = (temp >> 16) & 0x1F;
}

/*==============================================================================
 * Función:
 *------------------------------------------------------------------------------
 * Utilidad: 
 *
 * Entrada: 
 *
 * Salida:  
 *==============================================================================
 */
void rtc_ajustar_hora(unsigned int horas, unsigned int minutos,
                      unsigned int segundos)
{
    LPC_RTC->CCR &= ~1;
    LPC_RTC->SEC = segundos;
    LPC_RTC->MIN = minutos;
    LPC_RTC->HOUR = horas;
    LPC_RTC->CCR |= 1;
}

/*==============================================================================
 * Función:
 *------------------------------------------------------------------------------
 * Utilidad: 
 *
 * Entrada: 
 *
 * Salida:  
 *==============================================================================
 */
void rtc_leer_fecha(unsigned int *dia, unsigned int *mes, unsigned int *ano)
{
    int temp;

    temp = LPC_RTC->CTIME1;

    *dia = temp & 0x1F;
    *mes = (temp >> 8) & 0x0F;
    *ano = (temp >> 16) & 0xFFF;
}

/*==============================================================================
 * Función:
 *------------------------------------------------------------------------------
 * Utilidad: 
 *
 * Entrada: 
 *
 * Salida:  
 *==============================================================================
 */
void rtc_ajustar_fecha(unsigned int dia, unsigned int mes, unsigned int ano)
{
    LPC_RTC->CCR &= ~1;
    LPC_RTC->DOM = dia;
    LPC_RTC->MONTH = mes;
    LPC_RTC->YEAR = ano;
    LPC_RTC->CCR |= 1;
}

