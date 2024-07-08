#include "../includes/entradasalida.h"


//Fijarse los SPRITNF y que hago si le poongo 0 o malloc


bool crear_config(char * nombre_archivo){
	//Todo es un config y se guarda y crea con el nombre del archivo
	char* path_archivo = string_new();
	path_archivo = generar_path_config(nombre_archivo);
	t_config* config_archivo = config_create(path_archivo);

	if(config_archivo == NULL){     //Verifica si existe un archivo con ese path y si no lo crea :D //Hace falta, no lo debería solo crear osea antes no puede existir?? jaja
		FILE* file_fd = fopen(path_archivo, "a+"); 
		fclose(file_fd);
		config_archivo = config_create(path_archivo);
	}
	
	 
	config_set_value(config_archivo, "TAMANIO_ARCHIVO", "0"); // SOLO ES PRUEBA

	int nuevo_bloque_inicial = obtener_bloques_libres(1); // el 1 es hardcodeo para que ocupe un bloque aunque tenga tamaño 0
			
	if (nuevo_bloque_inicial==-1){ // Entra si no hay bloques libres
				return false;
	}
	char *bloque_text=malloc(10); 
	sprintf(bloque_text,"%d",nuevo_bloque_inicial);
	config_set_value(config_archivo, "BLOQUE_INICIAL", bloque_text);
	setear_bitmap(nuevo_bloque_inicial,nuevo_bloque_inicial,true);
	///////// PRUEBA ////////////
	mostrar_estado_archivo(config_archivo);
	config_save(config_archivo);
	return true;
}


bool delete_archivo(char* nombre_archivo){
	
	char* path_archivo = string_new();
	path_archivo = generar_path_config(nombre_archivo);
	t_config* config_archivo = config_create(path_archivo);
	if( config_archivo == NULL){
		return false;
	}
	
	///////// PRUEBA ////////////
	log_info(entradasalida_logger,"ESTADO INNICIAL ARCHIVO:");
	mostrar_estado_archivo(config_archivo);

	int bloque_inicial = config_get_int_value(config_archivo,"BLOQUE_INICIAL");
	int tamanio = config_get_int_value(config_archivo,"TAMANIO_ARCHIVO");
	int final_bloques_nuevo = tamanio <= 1024 ? bloque_inicial : bloque_inicial + ceil(tamanio/BLOCK_SIZE)-1; // Le resto 1 para que cuente el bloque inicial.
	
	setear_bitmap(bloque_inicial, final_bloques_nuevo, false);
	
	///////// PRUEBA ////////////
	mostrar_estado_archivo(config_archivo);
	
	remove(path_archivo);
	config_destroy(config_archivo); //Elimina solo el config que usaba
	return true;

}

bool truncar_archivo(char* nombre_archivo,int tamanio_nuevo){

	t_config* config_archivo = config_create(generar_path_config(nombre_archivo));
	int bloque_inicial = config_get_int_value(config_archivo,"BLOQUE_INICIAL");
	int tamanio_viejo= config_get_int_value(config_archivo,"TAMANIO_ARCHIVO");
	int nuevo_bloque_inicial;
	int final_bloques_viejo = tamanio_viejo <= 1024 ? bloque_inicial : bloque_inicial + ceil(tamanio_viejo/BLOCK_SIZE)-1; // Le resto 1 para que cuente el bloque inicial.
	int final_bloques_nuevo = tamanio_nuevo <= 1024 ? bloque_inicial : bloque_inicial + ceil(tamanio_nuevo/BLOCK_SIZE)-1; // Le resto 1 para que cuente el bloque inicial.
	//int final_bloques_nuevo = bloque_inicial + ceil((tamanio_nuevo)/BLOCK_SIZE) -1;

	////////////      PRUEBA        /////////
	log_info(entradasalida_logger,"ESTADO INICIAL ARCHIVO:");
	mostrar_estado_archivo(config_archivo);

	if (final_bloques_nuevo>final_bloques_viejo)
	{
		if(puede_crecer(final_bloques_nuevo,final_bloques_viejo)){
				char *tamanio_nuevo_text=malloc(10);
				sprintf(tamanio_nuevo_text,"%d",tamanio_nuevo);
				config_set_value(config_archivo, "TAMANIO_ARCHIVO", tamanio_nuevo_text);
				
				setear_bitmap(final_bloques_viejo,final_bloques_nuevo,true);
				config_save(config_archivo);

				///////////// PRUEBA ////////////////
				mostrar_estado_archivo(config_archivo);

		}
		else  // Aca falta lo de compactación y fijarme que tengo bloque libres necesarios
		{
				if (!hay_espacios(final_bloques_nuevo-final_bloques_viejo)) //si no hay entra D:
				{
					return false;
				}
				nuevo_bloque_inicial = compactacion(bloque_inicial,final_bloques_viejo,final_bloques_nuevo);
				char *bloque_text= malloc(10); 
				sprintf(bloque_text,"%d",nuevo_bloque_inicial);
				config_set_value(config_archivo, "BLOQUE_INICIAL", bloque_text);
				char *tamanio_nuevo_text= malloc(10); 
				sprintf(tamanio_nuevo_text,"%d",tamanio_nuevo);
				config_set_value(config_archivo, "TAMANIO_ARCHIVO", tamanio_nuevo_text);
				config_save(config_archivo);

				///////////// PRUEBA ////////////////
				mostrar_estado_archivo(config_archivo);
		}
			
	}
	else // o el tam nuevo es igual o menor así que sirve si o si :D
	{	char* tamanio_text=malloc(10); //Se guarda bien?
		sprintf(tamanio_text,"%d",tamanio_nuevo);
		config_set_value(config_archivo, "TAMANIO_ARCHIVO", tamanio_text);
		if(final_bloques_nuevo!=final_bloques_viejo){
			setear_bitmap(final_bloques_viejo,final_bloques_nuevo,false);
		}
		free(tamanio_text);
		config_save(config_archivo);
	}
	return true;
}

bool escribir_archivo(char* nombre_archivo,int registro_archivo,char* escrito){
	//Camino feliz entra todo :D
	t_config* config_archivo = config_create(generar_path_config(nombre_archivo));
	int bloque_inicial = config_get_int_value(config_archivo,"BLOQUE_INICIAL");
	int tamanio = config_get_int_value(config_archivo,"TAMANIO_ARCHIVO");
	int donde_escribir = (bloque_inicial * BLOCK_SIZE + registro_archivo); 
	if(donde_escribir+strlen(escrito)<bloque_inicial+tamanio){
		strcat(bloquesEnMemoria+donde_escribir,escrito);
		return true;
	}
	return false;
 }

 char* leer_archivo(char* nombre_archivo,int registro_archivo,int tamanio){
	char* leido = malloc (tamanio); //llega en bytes?
	t_config* config_archivo = config_create(generar_path_config(nombre_archivo));
	int bloque_inicial = config_get_int_value(config_archivo,"BLOQUE_INICIAL");
	int tamanio_archivo = config_get_int_value(config_archivo,"TAMANIO_ARCHIVO");
	if (registro_archivo+tamanio-1>tamanio_archivo) //Se fija que este dentro del archivo menos uno xq escribe uno en el puntero
	{
		return "";
	}

	for (int i = 0; i < tamanio; i++)
	{
		strcat(leido+i,bloquesEnMemoria+bloque_inicial*BLOCK_SIZE+registro_archivo+i);	
	}
	return leido;
 }


int obtener_bloques_libres(int tamanio){
      int i = 0;
	  int j= 0 ;
	  int bloques_libres= 0;
      int tamanio_bits = ceil(tamanio/BLOCK_SIZE/8);
      bool suficiente = false;
      int rta=-1;
	  int tamanio_bitmap = bitarray_get_max_bit(bitmap);
	  if(tamanio_bits==0){ tamanio_bits=1;} //preguntarse en el create //UN SEGURO ver si saco
      while (tamanio_bitmap>i)
      {
            if (!bitarray_test_bit(bitmap,i)) //Ta vacio el int i??
            {     bloques_libres=1;
                  j=i+1;
                while(bloques_libres<tamanio_bits) // tamanio en bytes/cantidad de bloques que ocupa/ cuanos bits son :D
                  {
                        if(bitarray_test_bit(bitmap,j)){ // Se fija si el bit siguiente esta libre si es 0 no entra
                              i=j;
							  bloques_libres=0;
							  //i++;
                              break;
                        }

						bloques_libres++;
						j++;
                        if (bloques_libres==tamanio_bits)
                        {     
                              setear_bitmap(i,j-1,true); // Hacer Setear bitmap de tal a tal y sete en 1 // el -1 es poque suma siempre y acá justo no hace falta
                              suficiente = true;
                        }
                  }
                  if(suficiente || bloques_libres == tamanio_bits){
                        rta= i;
                        break;
                  }
            }
            i++;
      }
      return rta;
      
}

bool puede_crecer(int final_bloques_nuevo,int final_bloques_viejo){

	for (int i = final_bloques_viejo+1 ; i <= final_bloques_viejo ; i++)
	{
		if (bitarray_test_bit(bitmap,i))
		{
			return false;
			break;
		}
		
	}
	
	return true;
}

//Comienzo y final en bloques
void setear_bitmap(int comienzo, int final,bool asigna){ 
	for (int i = comienzo; i <=  final; i++)
	{	
		asigna ? bitarray_set_bit(bitmap,i) : bitarray_clean_bit(bitmap,i);
	}
	
}

bool hay_espacios(int bloques_faltantes){
	int tamanio_bitmap = bitarray_get_max_bit(bitmap);
	int bloques_libres = 0;
	for (int i = 0; i < tamanio_bitmap; i++)
	{
		if (!bitarray_test_bit(bitmap,i)) 
		{
			bloques_libres ++;
		}
		if( bloques_libres == bloques_faltantes) return true;

	}

	return false;
}

int compactacion(int bloque_inicial,int final_bloques_viejo, int final_bloques_nuevo){
	int tamanio_archivo = (final_bloques_viejo-bloque_inicial+1)*BLOCK_SIZE;  //Puedo poner el tamanio viejo D1?
	char* archivo_aux= malloc(tamanio_archivo);

	setear_bitmap(bloque_inicial,final_bloques_viejo,false);

	for (int i = 0; i < tamanio_archivo; i++)
	{
		strcat(archivo_aux+i,bloquesEnMemoria+(bloque_inicial*BLOCK_SIZE+i));
	}

	int nuevo_bloque_inicial = mover_archivos();
	
	strcat(bloquesEnMemoria+(nuevo_bloque_inicial*BLOCK_SIZE),archivo_aux);  //copio lo que esté escrito
	
	setear_bitmap(nuevo_bloque_inicial,final_bloques_nuevo-bloque_inicial+1,true); //2do parametro cant bloques + 1 por el inicial

	free(archivo_aux);
	usleep(1000*RETRASO_COMPACTACION);
	return nuevo_bloque_inicial;
}

int mover_archivos(){

	int primer_bloque_libre = 0, ultimo_bloque_ocupado = 0, primer_bloque_ocupado = 0;
	int nuevo_bloque_inicial;
	for (int i = 0; i < BLOCK_COUNT; i++)
	{	if (!bitarray_test_bit(bitmap,i)) //Esta libre ? 
		{
		primer_bloque_libre = i;

		for (int j = i+1; j < BLOCK_COUNT; j++)
		{	
		
		if (bitarray_test_bit(bitmap,j)) // entra si está ocupado
		{	
			primer_bloque_ocupado=j;
			ultimo_bloque_ocupado = tamanio_bloque_escrito(primer_bloque_ocupado);
			i = copiar_archivo(primer_bloque_libre,primer_bloque_ocupado,ultimo_bloque_ocupado); // devuelve el ult bloque ocupado
			nuevo_bloque_inicial = i+1;
			break;
		}
			
			}
		}
	}
	return nuevo_bloque_inicial;
}

int tamanio_bloque_escrito(int primer_bloque_ocupado){

	for (int i = primer_bloque_ocupado+1; i < BLOCK_COUNT; i++)
	{
		if (!bitarray_test_bit(bitmap,i))
		{
			return i-1;
		}
	}
	return BLOCK_COUNT-1;
}

int copiar_archivo(int primer_bloque_libre, int primer_bloque_ocupado, int ultimo_bloque_ocupado){
	int tamanio = (ultimo_bloque_ocupado - primer_bloque_ocupado+1)*BLOCK_SIZE;
	int cant_bloques =ceil (ultimo_bloque_ocupado - primer_bloque_ocupado+1);
	char* archivo_aux = malloc(tamanio);
	for (int i = 0; i < tamanio; i++)
	{
		strcat(archivo_aux+i,bloquesEnMemoria+primer_bloque_ocupado*BLOCK_SIZE+i);  //copio lo que esté escrito
	}
	setear_bitmap(primer_bloque_ocupado,ultimo_bloque_ocupado,false); //Libero el bitmap
	setear_bitmap(primer_bloque_libre,primer_bloque_libre + cant_bloques -1 , true); //  -1 por el inicio
	strcat(bloquesEnMemoria+primer_bloque_libre*BLOCK_SIZE,archivo_aux); //copio todo
	free(archivo_aux);
	return primer_bloque_libre + cant_bloques -1 ;
}

char* generar_path_config(char* nombre_archivo){
	char* path_archivo = string_new();
	strcat(path_archivo,"/");
	strcat(path_archivo,nombre_archivo);
	path_archivo = crear_path(path_archivo);
	return path_archivo;

}

void mostrar_estado_archivo(t_config* config_archivo){
	log_info(entradasalida_logger,config_get_string_value(config_archivo,"BLOQUE_INICIAL"));
	log_info(entradasalida_logger, config_get_string_value(config_archivo,"TAMANIO_ARCHIVO"));
	
	log_info(entradasalida_logger,"BITMAP");
	int bloque_inicial = config_get_int_value(config_archivo,"BLOQUE_INICIAL");
	int cant_bloques = ceil((config_get_int_value(config_archivo,"TAMANIO_ARCHIVO"))/BLOCK_SIZE/8);
	if(cant_bloques==0) {cant_bloques++;}
	char* rta;
	int bitmap_index = bloque_inicial;
	for (int i = 0 ; i < cant_bloques; i++)
	{	
		
		rta = bitarray_test_bit(bitmap,bitmap_index) ? "1" : "0";
		log_info(entradasalida_logger,rta);
		bitmap_index++;	
	}
}