#ifndef RTC_H
#define RTC_H

void rtc_inicializar( void );

void rtc_leer_hora(unsigned int *horas, unsigned int *minutos,
                   unsigned int *segundos);

void rtc_ajustar_hora(unsigned int horas, unsigned int minutos,
                      unsigned int segundos);

void rtc_leer_fecha(unsigned int *dia, unsigned int *mes, unsigned int *ano);

void rtc_ajustar_fecha(unsigned int dia, unsigned int mes, unsigned int ano);

#endif
