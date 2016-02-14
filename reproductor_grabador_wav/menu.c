/***************************************************************************//**
 */

#include "menu.h"
#include "bloque.h"
#include "glcd.h"
#include "timer1.h"

static uint16_t rotulo[5][22] =
{
	
  {RED, BLACK, BLACK,BLACK, RED, BLACK,  BLACK,  BLACK, RED, RED, BLACK, BLACK, BLACK, RED,  BLACK,  BLACK, BLACK, RED},
  {RED, BLACK, BLACK, BLACK, RED, BLACK, BLACK, RED,BLACK, BLACK, RED, BLACK, BLACK, RED,  BLACK, BLACK, BLACK, RED, BLACK},
  {RED, BLACK, RED, BLACK, RED, BLACK,  RED,  BLACK, BLACK, BLACK, BLACK, RED, BLACK, RED,  BLACK,  BLACK, BLACK, RED},
  {RED, BLACK, RED, BLACK, RED, BLACK, RED, RED, RED, RED, RED, RED, BLACK, BLACK,  RED, BLACK, RED, BLACK, BLACK,BLACK},
  {BLACK, RED, BLACK, RED, BLACK, BLACK,  RED,  BLACK, BLACK, BLACK, BLACK, RED, BLACK, BLACK, BLACK,  RED, BLACK, BLACK}
};






/***************************************************************************//**
 */
/* Muestra un menu y devuelve la opcion elegida 
	 Devuelve 0 para ver contenido de la SD, 1 para grabar un nuevo sonido o 2 para Salir*/
int menu_inicial(void)
{
	
    int i, j;
		int opc = 0; //Valor a devolver
		int t; //Pulsacion
		glcd_clear(BLACK);
		do{
			t = leer_pulsacion();
				for (i = 0; i < 5; i++)
				{
						for (j = 0; j < 21; j++)
						{
								bloque_pintar(BLOQUE_TAMANO*(5 + j), BLOQUE_TAMANO*(5 + i), rotulo[i][j]);
						}
				}
				if(opc==0){
					glcd_text(240 - 14*8, 200, "Reproducir", WHITE, BLACK);
				}else if(opc==1){
					glcd_text(240 - 14*8, 200, "Grabar", WHITE, BLACK);
				}else if(opc==2){
					glcd_text(240 - 14*8, 200, "Salir", WHITE, BLACK);
				}
				if(t==1){ //Si pulsas arriba
					if(opc==0){
						opc=2;
						glcd_clear(BLACK);
					}else if(opc==1){
						opc=0;
						glcd_clear(BLACK);
					}else if(opc==2){
						opc=1;
						glcd_clear(BLACK);
					}
				}else if(t==2){ //Si pulsas abajo
					if(opc==0){
						opc=1;
						glcd_clear(BLACK);
					}else if(opc==1){
						opc=2;
						glcd_clear(BLACK);
					}else if(opc==2){
						opc=0;
						glcd_clear(BLACK);
					}
				}
		}while(t!=5);
	
    return opc;
}

/*Menu que muestra un fichero o archivo recibido desde la SD*/
int mostrar_archivo(char *nombre){
		int i, j;
		int t;
		while(leer_pulsacion()==0){}
		glcd_clear(BLACK);
		do{
				for (i = 0; i < 5; i++)
				{
						for (j = 0; j < 21; j++)
						{
								bloque_pintar(BLOQUE_TAMANO*(5 + j), BLOQUE_TAMANO*(5 + i), rotulo[i][j]);
						}
				}
				glcd_text(240 - 14*8, 200, nombre, WHITE, BLACK);
				t = leer_pulsacion();
		}while(t==0);
		return t;
}

/*
Devuelve 0 para reproducir el archivo o entrar en el directorio, 1 para eliminarlo 
o 2 para volver al menu Inicial */
int menu_archivo(void){
	  int i, j;
		int opc = 0;
		int t;
		glcd_clear(BLACK);
		do{
				t = leer_pulsacion();
				for (i = 0; i < 5; i++)
				{
						for (j = 0; j < 21; j++)
						{
								bloque_pintar(BLOQUE_TAMANO*(5 + j), BLOQUE_TAMANO*(5 + i), rotulo[i][j]);
						}
				}
				if(opc==0){
					glcd_text(240 - 14*8, 200, "Seleccionar", WHITE, BLACK);
				}else if(opc==1){
					glcd_text(240 - 14*8, 200, "Eliminar", WHITE, BLACK);
				}else if(opc==2){
					glcd_text(240 - 14*8, 200, "Volver al Menu de Inicio", WHITE, BLACK);
				}
				
				if(t==1){ //Si pulsas arriba
					if(opc==0){
						opc=2;
						glcd_clear(BLACK);
					}else if(opc==1){
						opc=0;
						glcd_clear(BLACK);
					}else if(opc==2){
						opc=1;
						glcd_clear(BLACK);
					}
				}else if(t==2){ //Si pulsas abajo
					if(opc==0){
						opc=1;
						glcd_clear(BLACK);
					}else if(opc==1){
						opc=2;
						glcd_clear(BLACK);
					}else if(opc==2){
						opc=0;
						glcd_clear(BLACK);
					}
				}
		
				
		}while(t!=5);

    return opc;
}

/*Menu de las opciones de grabacion. Devuelve 0 para grabar audio, 
1 para detener la grabacion, 2 para reproducir y 3 para salir. */
int menuGrabacion(void)
{
	
    int i, j;
		int opc = 0; //Valor a devolver
		int t; //Pulsacion
		glcd_clear(BLACK);
		while(leer_pulsacion()==0){}
		do{
			t = leer_pulsacion();
				for (i = 0; i < 5; i++)
				{
						for (j = 0; j < 21; j++)
						{
								bloque_pintar(BLOQUE_TAMANO*(5 + j), BLOQUE_TAMANO*(5 + i), rotulo[i][j]);
						}
				}
				if(opc==0){
					glcd_text(240 - 14*8, 200, "Grabar audio", WHITE, BLACK);
				}else if(opc==1){
					glcd_text(240 - 14*8, 200, "Detener", WHITE, BLACK);
				}else if(opc==2){
					glcd_text(240 - 14*8, 200, "Reproducir", WHITE, BLACK);
				}else if(opc==3){
					glcd_text(240 - 14*8, 200, "Salir", WHITE, BLACK);
				}
				if(t==1){ //Si pulsas arriba
					if(opc==0){
						opc=3;
						glcd_clear(BLACK);
					}else if(opc==1){
						opc=0;
						glcd_clear(BLACK);
					}else if(opc==2){
						opc=1;
						glcd_clear(BLACK);
					}else if(opc==3){
						opc=2;
						glcd_clear(BLACK);
					}
				}else if(t==2){ //Si pulsas abajo
					if(opc==0){
						opc=1;
						glcd_clear(BLACK);
					}else if(opc==1){
						opc=2;
						glcd_clear(BLACK);
					}else if(opc==2){
						opc=3;
						glcd_clear(BLACK);
					}else if(opc==3){
						opc=0;
						glcd_clear(BLACK);
					}
				}
		}while(t!=5);

    return opc;
}

/*Muestra por pantalla el texto recibido*/
void mostrar_pantalla(char *muestra){
		int i, j;
		int t;
		glcd_clear(BLACK);
		while(leer_pulsacion()==0){}
		do{
				t = leer_pulsacion();
				for (i = 0; i < 5; i++)
				{
						for (j = 0; j < 21; j++)
						{
								bloque_pintar(BLOQUE_TAMANO*(5 + j), BLOQUE_TAMANO*(5 + i), rotulo[i][j]);
						}
				}
					glcd_text(240 - 14*8, 200, muestra, WHITE, BLACK);
		}while(t==0);
}

