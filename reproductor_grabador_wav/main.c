/***************************************************************************//**
 *
 * Reproductor/Grabador de sonidos usando micrófono y altavoz (auriculares). Guarda y reproduce 
 * datos del sonido en una tarjeta SD si hubiese una insertada. También puede eliminar el contenido
 * de la tarjeta SD. 
 *
 */

#include <LPC407x_8x_177x_8x.h>
#include "ff.h"
#include "menu.h"
#include "glcd.h"
#include "timer1.h"
#include <stdio.h>
#include <stdbool.h>
#include "Board_Audio.h"                // ::Board Support:Audio


int reproducir_wav(char *nombre);
void guardar_wav(void);
bool comprobar_cabecera(void);
/* Puntero a la estructura FATFS. Debe estar alineado en una posición
 * de palabra.
 */
__align(4) FATFS fs;
   
/* El buffer de escritura y lectura deben estar en una posición de palabra.
 */
__align(4) unsigned char buffer_w[512];
__align(4) unsigned char buffer_r[512];

static uint8_t nextdata[50000];
static uint32_t SAMP_NUM = 50000;

void AudioInicializar(){
	
  Audio_Initialize   (NULL);
  Audio_SetDataFormat(AUDIO_STREAM_OUT, AUDIO_DATA_8_STEREO);
  Audio_SetFrequency (AUDIO_STREAM_OUT, 8000);
  Audio_SetMute      (AUDIO_STREAM_OUT, 0, 1);
  Audio_SetDataFormat(AUDIO_STREAM_IN, AUDIO_DATA_8_STEREO);
  Audio_SetFrequency (AUDIO_STREAM_IN, 8000);
  Audio_SetVolume    (AUDIO_STREAM_IN, 0, 90 | AUDIO_VOLUME_AUTO_GAIN);
  Audio_SetMute      (AUDIO_STREAM_IN, 0, 1);
	
	Audio_SetVolume(AUDIO_STREAM_OUT, 0, 90 | AUDIO_VOLUME_AUTO_GAIN);
}

void AudioGrabar (void) {
				int i;
				for(i = 0; i<SAMP_NUM; i++){
					nextdata[i] = 0;
				}
				
				Audio_SetMute      (AUDIO_STREAM_OUT, 0, 1);
				Audio_SetMute      (AUDIO_STREAM_IN, 0, 0);
        Audio_ReceiveData(nextdata, SAMP_NUM);
        Audio_Start (AUDIO_STREAM_IN);
}

void Audio_Cbk(uint32_t event){
	
}

void AudioStop(){
	Audio_SetMute      (AUDIO_STREAM_OUT, 0, 1);
	Audio_SetMute      (AUDIO_STREAM_IN, 0, 1);
	 
	Audio_Initialize(&Audio_Cbk);
	Audio_Stop(AUDIO_STREAM_IN);
  Audio_Stop(AUDIO_STREAM_OUT);
	
	guardar_wav();

}

void AudioPlay(){
	Audio_SetMute      (AUDIO_STREAM_OUT, 0, 0);
	Audio_SetMute      (AUDIO_STREAM_IN, 0, 1);
	
	Audio_SendData(nextdata, SAMP_NUM);
  Audio_Start(AUDIO_STREAM_OUT);
}

void guardar_wav(void){
	FIL file;
	FRESULT res;
	DIR dir;
	bool existe;
	int num;
	unsigned int dummy;
	char nombre[15];
	int i;
	int tam_final;

	res = f_opendir(&dir, "");  //Se guardara en la raiz
	if(res != FR_OK){ //Tarjeta SD no insertada
		mostrar_pantalla("Inserte una tarjeta SD");
		return;
	}
	
	num = 1;
	existe = true;
	while(existe == true){
		snprintf(nombre, 15, "grab%d.wav", num);
		res = f_open(&file, nombre, FA_CREATE_NEW);
		if(res != FR_OK){
			num++;
		}else{
			existe=false;
		}			
	}
	res = f_open(&file, nombre, FA_READ | FA_WRITE | FA_CREATE_ALWAYS);
	/*Agregar cabecera WAV*/
	//Letras "RIFF" 4 Bytes
	buffer_w[0] = (uint8_t)0x52;
	buffer_w[1] = (uint8_t)0x49;
	buffer_w[2] = (uint8_t)0x46;
	buffer_w[3] = (uint8_t)0x46;
	//ChunkSize 4 B
	buffer_w[4] = (uint8_t)0xc4;
	buffer_w[5] = (uint8_t)0x68;
	buffer_w[6] = (uint8_t)0x01;
	buffer_w[7] = (uint8_t)0x00;
	//Letras "WAVE" 4 B
	buffer_w[8] = (uint8_t)0x57;
	buffer_w[9] = (uint8_t)0x41;
	buffer_w[10] = (uint8_t)0x56;
	buffer_w[11] = (uint8_t)0x45;
	//Letras "fmt " 4 B
	buffer_w[12] = (uint8_t)0x66;
	buffer_w[13] = (uint8_t)0x6d;
	buffer_w[14] = (uint8_t)0x74;
	buffer_w[15] = (uint8_t)0x20;
	//Numero 16 para indicar que es PCM (sin comprimir) 4 B
	buffer_w[16] = (uint8_t)0x10;
	buffer_w[17] = (uint8_t)0x00;
	buffer_w[18] = (uint8_t)0x00;
	buffer_w[19] = (uint8_t)0x00;
	//Compresion utilizada (1 si es PCM) 2 B
	buffer_w[20] = (uint8_t)0x01;
	buffer_w[21] = (uint8_t)0x00;
	//Numero de canales (2 para Estereo) 2 B
	buffer_w[22] = (uint8_t)0x02;
	buffer_w[23] = (uint8_t)0x00;
	//Frecuencia en Hz 4 B
	buffer_w[24] = (uint8_t)0x40;
	buffer_w[25] = (uint8_t)0x1f;
	buffer_w[26] = (uint8_t)0x00;
	buffer_w[27] = (uint8_t)0x00;
	//ByteRate 4 B
	buffer_w[28] = (uint8_t)0x80;
	buffer_w[29] = (uint8_t)0x3e;
	buffer_w[30] = (uint8_t)0x00;
	buffer_w[31] = (uint8_t)0x00;
	//BlockAlign 2 B
	buffer_w[32] = (uint8_t)0x02;
	buffer_w[33] = (uint8_t)0x00;
	//Bits 2 B
	buffer_w[34] = (uint8_t)0x08;
	buffer_w[35] = (uint8_t)0x00;
	//Letras "data" 4 B
	buffer_w[36] = (uint8_t)0x64;
	buffer_w[37] = (uint8_t)0x61;
	buffer_w[38] = (uint8_t)0x74;
	buffer_w[39] = (uint8_t)0x61;
	//Numero de datos*Canales*Bits/8  4 B
	buffer_w[40] = (uint8_t)0xa0;
	buffer_w[41] = (uint8_t)0x86;
	buffer_w[42] = (uint8_t)0x01;
	buffer_w[43] = (uint8_t)0x00;
	res = f_write(&file, buffer_w, 44, &dummy);
	
	/*Datos del sonido*/
	tam_final=0;
	while(tam_final<50000){
		for(i=0; i<512 && tam_final<50000; i++){
			buffer_w[i] = '0' + nextdata[tam_final];
			tam_final++;
		}
		res = f_write(&file, buffer_w, 512, &dummy);
		if (res != FR_OK){
			mostrar_pantalla("Error de escritura");
		}
		if(dummy != 512){
			mostrar_pantalla("No se ha escrito completamente");
		}
		
	}
	
	res = f_close(&file);
}

/*Devuelve -1 si error */
int reproducir_wav(char *nombre){
	FIL file;
	FRESULT res;
	unsigned int dummy;
	uint8_t buffer[50000];
	int son_error;
	int tam_final;
	int i;
	
	res = f_open(&file, nombre, FA_READ | FA_WRITE | FA_OPEN_EXISTING);
	if(res!= FR_OK){
		mostrar_pantalla("Error al abrir el archivo");
		return -1;
	}
	tam_final=0;
	res = f_read(&file, buffer_r, 44, &dummy); //Cabecera
	
	//Comprobamos que el archivo sea el correcto
	if(dummy != 44) return -1;
	if(comprobar_cabecera()==false) return -1;
	
	while(tam_final<50000){
		res = f_read(&file,buffer_r,512,&dummy);
		if (res != FR_OK){
			mostrar_pantalla("Error al leer el archivo");
		}
		if(dummy != 512){
			mostrar_pantalla("Error dummy");
		}
		for(i=0;i<512 && tam_final<50000; i++){
			buffer[tam_final] = buffer_r[i];
			tam_final++;
		}
	}

	/*Reproduccion*/
	
	//Inicializar
	Audio_Initialize   (NULL);
  Audio_SetDataFormat(AUDIO_STREAM_OUT, AUDIO_DATA_8_STEREO);
  Audio_SetFrequency (AUDIO_STREAM_OUT, 8000);
  Audio_SetMute      (AUDIO_STREAM_OUT, 0, 0);
	Audio_SetVolume(AUDIO_STREAM_OUT, 0, 90 | AUDIO_VOLUME_AUTO_GAIN);
	
	
	//Reproducir
	son_error = Audio_SendData(buffer, SAMP_NUM);
	if(son_error == -1) mostrar_pantalla("Error al enviar datos");
  son_error = Audio_Start(AUDIO_STREAM_OUT);
	if(son_error == -1) mostrar_pantalla("Error al reproducir audio");
	
	mostrar_pantalla("Pulsar para acabar");
	
	Audio_Uninitialize();
	res = f_close(&file);
	
	return 0;
	
}

/*Devuelve true si es un archivo apto para leer o false si no lo es*/
bool comprobar_cabecera(void){
	
	//Letras "RIFF" 4 Bytes
	if (buffer_r[0] !=(uint8_t)0x52) return false;
	if (buffer_r[1] !=(uint8_t)0x49) return false;
	if (buffer_r[2] !=(uint8_t)0x46) return false;
	if (buffer_r[3] !=(uint8_t)0x46) return false;

	//Letras "WAVE" 4 B
	if (buffer_r[8] !=(uint8_t)0x57) return false;
	if (buffer_r[9] !=(uint8_t)0x41) return false;
	if (buffer_r[10] != (uint8_t)0x56) return false;
	if (buffer_r[11] != (uint8_t)0x45) return false;
	//Letras "fmt " 4 B
	if (buffer_r[12] != (uint8_t)0x66) return false;
	if (buffer_r[13] != (uint8_t)0x6d) return false;
	if (buffer_r[14] != (uint8_t)0x74) return false;
	if (buffer_r[15] != (uint8_t)0x20) return false;
	
	//Numero 16 para indicar que es PCM (sin comprimir) 4 B
	if (buffer_r[16] != (uint8_t)0x10) return false;
	if (buffer_r[17] != (uint8_t)0x00) return false;
	if (buffer_r[18] != (uint8_t)0x00) return false;
	if (buffer_r[19] != (uint8_t)0x00) return false;


	//Numero de canales (2 para Estereo) 2 B
	if (buffer_r[22] != (uint8_t)0x02) return false;
	if (buffer_r[23] != (uint8_t)0x00) return false;
	//Frecuencia en Hz 4 B
	if (buffer_r[24] != (uint8_t)0x40) return false;
	if (buffer_r[25] != (uint8_t)0x1f) return false;
	if (buffer_r[26] != (uint8_t)0x00) return false;
	if (buffer_r[27] != (uint8_t)0x00) return false;

	//Letras "data" 4 B
	if (buffer_r[36] != (uint8_t)0x64) return false;
	if (buffer_r[37] != (uint8_t)0x61) return false;
	if (buffer_r[38] != (uint8_t)0x74) return false;
	if (buffer_r[39] != (uint8_t)0x61) return false;

	
	return true;
}


int main(void)
{
	
		int opc;
		bool volver;
		bool salir;
	
	/*Inicializamos el FatFS */
	  FRESULT res;
    FILINFO fno;
    DIR dir;
	  char path[75]; //Origen del archivo
		char seleccion[75];
    char *fn;   /* Aqui se guardara los datos del archivo */
		
	//Si esta activado el soporte de nombres largos pues inicializamos las varibles correspondientes
#if _USE_LFN
    static char lfn[_MAX_LFN + 1];   /* Buffer to store the LFN */
    fno.lfname = lfn;
    fno.lfsize = sizeof lfn;
#endif
  
		/*Inicializamos la pantalla */
		glcd_init();

    timer1_inicializar();
		glcd_clear(BLACK);
	

  while(1){
    /* Montar el sistema de archivos.
     */
    f_mount(0, &fs);
		salir = false;
		while(salir == false){
			opc = menu_inicial(); //Menu inicial
			volver = false;
			if(opc == 0){ //Si seleccionamos Ver contenido de la SD
		
			/*Intentamos leer los archivos */
				sprintf(path, ""); //Leemos en la raiz de la tarjeta SD
				res = f_opendir(&dir, path);  //Abrimos el directorio
				if (res == FR_OK) { //Si se ha abierto correctamente
					while(volver == false){
						//Reiniciamos los datos para cuando terminemos de recorrer la lista de archivos
						sprintf(path, "");
						res = f_opendir(&dir, path); 
						for (;;) { //Recorremos todos los archivos de la SD
								res = f_readdir(&dir, &fno);                   /* Intenta leer un archivo */
								if (res != FR_OK || fno.fname[0] == 0) break;  /* Termina si llegamos al final o hay algun error */
								if (fno.fname[0] == '.') continue;            
							
								//Guardamos el nombre dle archivo (la forma depende de si usamos o no nombres largos)
								#if _USE_LFN
									fn = *fno.lfname ? fno.lfname : fno.fname;
								#else
									fn = fno.fname;
								#endif
						
								//Tenemos el nombre en fn
								//Si la carpeta actual es un directorio entrara por aqui
								if (fno.fattrib & AM_DIR){
										if(volver==false){
											opc = mostrar_archivo(fn);
											if(opc==5){ //si selecciona este archivo
													opc = menu_archivo(); //Mostramos si desea reproducir o eliminar
													if(opc == 0){ //Desea reproducirlo
														sprintf(path, "/%s", fn); //Cambiamos el path para entrar en el directorio
														res = f_opendir(&dir, path); 
													}else if(opc == 1){//Desea eliminarlo
														sprintf(seleccion, "%s/%s", path, fn); //Guardamos en selecciona la direccion completa del fichero
														res = f_unlink(seleccion); //Eliminar
														if(res != FR_OK){
															mostrar_pantalla("Error, el directorio debe estar vacio");
														}else{
															mostrar_pantalla("Archivo eliminado correctamente");
														}
													}else if (opc==2){
														volver = true;
													}
												}//fin_if_opc==5
											}//if volver==false
								}else{ //Si en vez de un directorio es un archivo entrara por aqui
										if(volver==false){
											opc = mostrar_archivo(fn); //Mostramos el nombre del archivo en pantalla
											if(opc==5){ //si selecciona este archivo
												opc = menu_archivo(); //Mostramos si desea reproducir o eliminar
												if(opc == 0){ //Desea reproducirlo
													sprintf(seleccion, "%s/%s", path, fn); //Gurdamos la direccion completa del archivo
													if (reproducir_wav(seleccion) == -1) mostrar_pantalla("Archivo incompatible");
												}else if(opc == 1){//Desea eliminarlo
													sprintf(seleccion, "%s/%s", path, fn); //Gurdamos la direccion completa del archivo
													res = f_unlink(seleccion); //Eliminamos
													if(res != FR_OK){
														mostrar_pantalla("Error al borrar");
													}else{
														mostrar_pantalla("Archivo eliminado correctamente");
													}
												}else if (opc ==2){ //Salir al menu inicial
													volver = true;
												}
											}//fin_if_opc==5
										}//fin_volver_false
								}//fin_else
								
						}//fin_for
					}//fin_while_volver
				}else {//fin_if FR_OK
					mostrar_pantalla("Inserte una tarjeta SD");
				}

		}else if (opc==1){ //Selecciona grabar sonido
				AudioInicializar();
				while (volver == false){
					opc = menuGrabacion(); //Menu de grabacion
					switch(opc){
						case 0:{
							AudioGrabar(); //Comienza a grabar
							break;
						}
						case 1:{
							AudioStop(); //Para de grabar y almacena en la tarjeta SD
							break;
						}
						case 2:{
							AudioPlay(); //Reproduce el ultimo audio grabado
							break;
						}
						case 3:{ //Salir
							volver=true;
							Audio_Uninitialize();
							break;
						}
					}
				}
			
		}else if(opc ==2){
			f_mount(0, NULL); //Desmontamos
			salir=true;
		}
	}//fin_while(1)
		mostrar_pantalla("Pulse para volver a comenzar.");
}//fin_while(1)
 
}//fin main






