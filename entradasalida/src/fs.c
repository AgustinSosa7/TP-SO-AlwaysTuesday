#include "../includes/entradasalida.h"


bool crear_config(char * nombre_archivo){
	//Todo es un config y se guarda y crea con el nombre del archivo
	
	char* path_archivo = generar_path_config(nombre_archivo);
	t_config* config_archivo = config_create(path_archivo);

	if(config_archivo == NULL){     //Verifica si existe un archivo con ese path y si no lo crea 
		FILE* file_fd = fopen(path_archivo, "a+"); 
		fclose(file_fd);
		config_archivo = config_create(path_archivo);
	}

	config_set_value(config_archivo, "TAMANIO_ARCHIVO", "0"); 

	pthread_mutex_lock(&mutex_bitmap);
	int nuevo_bloque_inicial = obtener_bloques_libres(1); // el 1 es hardcodeo para que ocupe un bloque aunque tenga tamaño 0
			
	if (nuevo_bloque_inicial==-1){ // Entra si no hay bloques libres
				free(path_archivo);
				return false;
	}
	char *bloque_text=malloc(10); 
	sprintf(bloque_text,"%d",nuevo_bloque_inicial);
	config_set_value(config_archivo, "BLOQUE_INICIAL", bloque_text);
	setear_bitmap(nuevo_bloque_inicial,nuevo_bloque_inicial,true);
	pthread_mutex_unlock(&mutex_bitmap);

	///////// PRUEBA ////////////
	mostrar_estado_archivo(config_archivo);

	
	config_save(config_archivo);
	config_destroy(config_archivo);
	free(bloque_text);
	free(path_archivo);
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
	log_info(entradasalida_log_debug,"ESTADO INNICIAL ARCHIVO:");
	mostrar_estado_archivo(config_archivo);

	int bloque_inicial = config_get_int_value(config_archivo,"BLOQUE_INICIAL");
	int tamanio = config_get_int_value(config_archivo,"TAMANIO_ARCHIVO");
	int final_bloques_nuevo = tamanio <= BLOCK_SIZE ? bloque_inicial : bloque_inicial + tamanio/BLOCK_SIZE; // Le resto 1 para que cuente el bloque inicial.
		
	pthread_mutex_lock(&mutex_bitmap);
	setear_bitmap(bloque_inicial, final_bloques_nuevo, false);
	pthread_mutex_unlock(&mutex_bitmap);


	///////// PRUEBA ////////////
	mostrar_estado_archivo(config_archivo);

	remove(path_archivo);
	free(path_archivo);
	config_destroy(config_archivo); //Elimina solo el config que usaba
	return true;
}

bool truncar_archivo(char* nombre_archivo,int tamanio_nuevo, int pid){
	
	char* path_archivo = generar_path_config(nombre_archivo);
	t_config* config_archivo = config_create(path_archivo);
	free(path_archivo);

	int bloque_inicial = config_get_int_value(config_archivo,"BLOQUE_INICIAL");
	int tamanio_viejo= config_get_int_value(config_archivo,"TAMANIO_ARCHIVO");
	int nuevo_bloque_inicial;
	int final_bloques_viejo = tamanio_viejo <= BLOCK_SIZE ? bloque_inicial : bloque_inicial + tamanio_viejo/BLOCK_SIZE; // Le resto 1 para que cuente el bloque inicial.
	int final_bloques_nuevo = tamanio_nuevo <= BLOCK_SIZE ? bloque_inicial : bloque_inicial + tamanio_nuevo/BLOCK_SIZE; 
	final_bloques_nuevo = (tamanio_nuevo%BLOCK_SIZE) == 0 ? final_bloques_nuevo-1 : final_bloques_nuevo;
	final_bloques_viejo = (tamanio_viejo%BLOCK_SIZE== 0 &&tamanio_viejo!=0)  ? final_bloques_viejo-1 : final_bloques_viejo;
	

	////////////      PRUEBA        /////////
	log_info(entradasalida_log_debug,"ESTADO INICIAL ARCHIVO:");
	mostrar_estado_archivo(config_archivo);
	
	if (final_bloques_nuevo>final_bloques_viejo)
	{	pthread_mutex_lock(&mutex_bitmap);
		if(puede_crecer(final_bloques_nuevo,final_bloques_viejo)){
				char *tamanio_nuevo_text=malloc(10);
				sprintf(tamanio_nuevo_text,"%d",tamanio_nuevo);
				config_set_value(config_archivo, "TAMANIO_ARCHIVO", tamanio_nuevo_text);
				free(tamanio_nuevo_text);
				
				setear_bitmap(final_bloques_viejo,final_bloques_nuevo,true);
			pthread_mutex_unlock(&mutex_bitmap);
				config_save(config_archivo);

				///////////// PRUEBA ////////////////
				mostrar_estado_archivo(config_archivo);
				config_destroy(config_archivo);

		}
		else  // Aca falta lo de compactación y fijarme que tengo bloque libres necesarios
		{		int cant_bloques = tamanio_nuevo%BLOCK_SIZE == 0 ?  tamanio_nuevo/BLOCK_SIZE : tamanio_nuevo/BLOCK_SIZE+1;
				nuevo_bloque_inicial = obtener_bloques_libres(cant_bloques);
				if(nuevo_bloque_inicial!=-1){
				char* archivo_aux= malloc(tamanio_viejo);
				setear_bitmap(bloque_inicial,final_bloques_viejo,false);
				for (int i = 0; i < tamanio_viejo; i++)
					{
					strcat(archivo_aux+i,bloquesEnMemoria+(bloque_inicial*BLOCK_SIZE+i));
					}
				strcat(bloquesEnMemoria+(nuevo_bloque_inicial*BLOCK_SIZE),archivo_aux);  //copio lo que esté escrito
				setear_bitmap(nuevo_bloque_inicial,nuevo_bloque_inicial+cant_bloques-1,true);
				pthread_mutex_unlock(&mutex_bitmap);
				char *bloque_text= malloc(10); 
				sprintf(bloque_text,"%d",nuevo_bloque_inicial);
				config_set_value(config_archivo, "BLOQUE_INICIAL", bloque_text);
				char *tamanio_nuevo_text= malloc(10); 
				sprintf(tamanio_nuevo_text,"%d",tamanio_nuevo);
				config_set_value(config_archivo, "TAMANIO_ARCHIVO", tamanio_nuevo_text);
				config_save(config_archivo);

				///////////// PRUEBA ////////////////
				mostrar_estado_archivo(config_archivo);
				config_destroy(config_archivo);


				free(archivo_aux);
				free(bloque_text);
				free(tamanio_nuevo_text);
				return true;
		}	
				if (!hay_espacios(final_bloques_nuevo-final_bloques_viejo-1)) //si no hay entra D:
				{
					pthread_mutex_unlock(&mutex_bitmap);
					return false;
				}
				inicializar_lista_archivos();
				for (int i = 0; i < list_size(lista_archivos_existentes); i++)
				{
					if (list_get(lista_archivos_existentes,i)==nombre_archivo)
					{
						list_remove(lista_archivos_existentes,i);
						break;
					}
					
				}
				nuevo_bloque_inicial = compactacion(bloque_inicial,final_bloques_viejo,final_bloques_nuevo,pid);
				pthread_mutex_unlock(&mutex_bitmap);
				char *bloque_text= malloc(10); 
				sprintf(bloque_text,"%d",nuevo_bloque_inicial);
				config_set_value(config_archivo, "BLOQUE_INICIAL", bloque_text);
				free(bloque_text);
				char *tamanio_nuevo_text= malloc(10); 
				sprintf(tamanio_nuevo_text,"%d",tamanio_nuevo);
				config_set_value(config_archivo, "TAMANIO_ARCHIVO", tamanio_nuevo_text);
				free(tamanio_nuevo_text);
				config_save(config_archivo);

				list_add(lista_archivos_existentes,nombre_archivo);

				///////////// PRUEBA ////////////////
				mostrar_estado_archivo(config_archivo);
				config_destroy(config_archivo);
		}
			
	}
	else // o el tam nuevo es igual o menor así que sirve si o si :D
	{	char* tamanio_text=malloc(10); 
		sprintf(tamanio_text,"%d",tamanio_nuevo);
		config_set_value(config_archivo, "TAMANIO_ARCHIVO", tamanio_text);
		if(final_bloques_nuevo!=final_bloques_viejo){
			pthread_mutex_lock(&mutex_bitmap);
			setear_bitmap(final_bloques_viejo,final_bloques_nuevo,false);
			pthread_mutex_unlock(&mutex_bitmap);
		}
		free(tamanio_text);
		config_save(config_archivo);
		config_destroy(config_archivo);
	}
	return true;
}

bool escribir_archivo(char* nombre_archivo,int registro_archivo,char* escrito, int tamanio_text){

	char* path_archivo = generar_path_config(nombre_archivo);
	t_config* config_archivo = config_create(path_archivo);
	int bloque_inicial = config_get_int_value(config_archivo,"BLOQUE_INICIAL");
	int tamanio = config_get_int_value(config_archivo,"TAMANIO_ARCHIVO");
	int donde_escribir = (bloque_inicial * BLOCK_SIZE + registro_archivo); 

	if(donde_escribir+tamanio_text<bloque_inicial*BLOCK_SIZE+tamanio){
		pthread_mutex_lock(&mutex_bloques);
		memcpy(bloquesEnMemoria+donde_escribir,escrito,tamanio_text);
		pthread_mutex_unlock(&mutex_bloques);
		config_destroy(config_archivo);
		free(path_archivo);
		return true;
	}
	
	config_destroy(config_archivo);
	free(path_archivo);
	return false;
 }

 char* leer_archivo(char* nombre_archivo,int registro_archivo,int tamanio){
	char* leido = malloc(tamanio); 
	char* path_archivo = generar_path_config(nombre_archivo);
	t_config* config_archivo = config_create(path_archivo);

	free(path_archivo);

	int bloque_inicial = config_get_int_value(config_archivo,"BLOQUE_INICIAL");
	int tamanio_archivo = config_get_int_value(config_archivo,"TAMANIO_ARCHIVO");
	if (registro_archivo+tamanio-1>tamanio_archivo) //Se fija que este dentro del archivo menos uno xq escribe uno en el puntero
	{
		config_destroy(config_archivo);
		return "";
	}
	pthread_mutex_lock(&mutex_bloques);
	
	memcpy(leido,bloquesEnMemoria+bloque_inicial*BLOCK_SIZE+registro_archivo,tamanio);	
	log_info(entradasalida_log_debug,"MENSAJE.%s",leido);
	pthread_mutex_unlock(&mutex_bloques);
	config_destroy(config_archivo);
	return leido;
 }

//DEVUELVE LA POSICION A SETEAR
int obtener_bloques_libres(int cant_bloques){
      int i = 0;
	  int j= 0 ;
	  int bloques_libres= 0;
      bool suficiente = false;
      int rta=-1;
	  int bitmap_max_index = BLOCK_COUNT;
	  
      while (bitmap_max_index>i)
      {		log_info(entradasalida_log_debug,"EL bit en la posicion %d , es %d" , i , bitarray_test_bit(bitmap,i));
            if (!bitarray_test_bit(bitmap,i)) //Ta vacio el int i??
            {     bloques_libres=1;
                  j=i+1;
                while(bloques_libres<cant_bloques && bitmap_max_index>j)   // tamanio en bytes/cantidad de bloques que ocupa/ cuanos bits son :D
                  {
                        if(bitarray_test_bit(bitmap,j)){ // Se fija si el bit siguiente esta libre si es 0 no entra
                              i=j;
							  bloques_libres=0;
							  //i++;
                              break;
                        }

						bloques_libres++;
						j++;
                    
                  }
                  if(suficiente || bloques_libres == cant_bloques){
                        rta= i;
                        break;
                  }
            }
            i++;
      }
	  
      return rta;
      
}

bool puede_crecer(int final_bloques_nuevo,int final_bloques_viejo){

	for (int i = final_bloques_viejo+1 ; i <= final_bloques_nuevo ; i++)
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
	msync(bitmap->bitarray,ceil(BLOCK_COUNT/8),MS_SYNC);
	
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

int compactacion(int bloque_inicial,int final_bloques_viejo, int final_bloques_nuevo,int pid){
	int tamanio_archivo = (final_bloques_viejo-bloque_inicial+1)*BLOCK_SIZE;
	int cant_bloques = (final_bloques_nuevo - bloque_inicial+1); 
	char* archivo_aux= malloc(tamanio_archivo);

	log_info(entradasalida_logger,"PID: <%d> - Inicio Compactación.",pid);

	setear_bitmap(bloque_inicial,final_bloques_viejo,false);
	memcpy(archivo_aux,bloquesEnMemoria+(bloque_inicial*BLOCK_SIZE),tamanio_archivo);

	int nuevo_bloque_inicial = mover_archivos();
	
	memcpy(bloquesEnMemoria+(nuevo_bloque_inicial*BLOCK_SIZE),archivo_aux,tamanio_archivo);  //copio lo que esté escrito
	
	setear_bitmap(nuevo_bloque_inicial,nuevo_bloque_inicial+cant_bloques-1 ,true); //2do parametro cant bloques + 1 por el inicial

	free(archivo_aux);
	usleep(1000*RETRASO_COMPACTACION);
	log_info(entradasalida_logger,"PID: <%d> - Fin Compactación.",pid);
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

void modificar_config (int primer_bloque_ocupado, int ultimo_bloque_ocupado,int primer_bloque_libre){
	char* nombre_archivo = malloc(30);
	char *bloque_text=malloc(10); 
	int ultimo_bloque_copiado=primer_bloque_ocupado-1;
	int bloque_inicial, tamanio_viejo;
	char* path = string_new();
	
	int cant_bloques;
	t_config* config_archivo;

	
	while (ultimo_bloque_copiado < ultimo_bloque_ocupado)
	{
		for (int i = 0; i < list_size(lista_archivos_existentes); i++)
	{
		nombre_archivo = list_get(lista_archivos_existentes,i);
		path = generar_path_config(nombre_archivo);
		//free(nombre_archivo);
		config_archivo = config_create(path);
		free(path);
		bloque_inicial = config_get_int_value(config_archivo,"BLOQUE_INICIAL");
		if(bloque_inicial==primer_bloque_ocupado){
			tamanio_viejo= config_get_int_value(config_archivo,"TAMANIO_ARCHIVO");
			cant_bloques = tamanio_viejo%BLOCK_SIZE == 0 ?  tamanio_viejo/BLOCK_SIZE : tamanio_viejo/BLOCK_SIZE+1;
			
			sprintf(bloque_text,"%d",primer_bloque_libre);
			config_set_value(config_archivo,"BLOQUE_INICIAL",bloque_text);
			config_save(config_archivo);
			config_destroy(config_archivo);
			list_remove(lista_archivos_existentes,i);
			primer_bloque_libre = primer_bloque_libre + cant_bloques;
			ultimo_bloque_copiado = ultimo_bloque_copiado + cant_bloques; 
			primer_bloque_ocupado = primer_bloque_ocupado + cant_bloques ; //No le resto uno porque es el proximo a copiar
			
			break;
		}
		config_destroy(config_archivo);
		}
	}

	//free(nombre_archivo);	
	free(bloque_text);
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
	char* archivo_aux_copiar = malloc(tamanio);

	modificar_config(primer_bloque_ocupado,ultimo_bloque_ocupado,primer_bloque_libre);

	memcpy(archivo_aux_copiar,bloquesEnMemoria+primer_bloque_ocupado*BLOCK_SIZE,tamanio);

	setear_bitmap(primer_bloque_ocupado,ultimo_bloque_ocupado,false); //Libero el bitmap

	setear_bitmap(primer_bloque_libre,primer_bloque_libre + cant_bloques -1 , true); //  -1 por el inicio

	memcpy(bloquesEnMemoria+primer_bloque_libre*BLOCK_SIZE,archivo_aux_copiar,tamanio); //copio todo

	free(archivo_aux_copiar);
	return primer_bloque_libre + cant_bloques -1 ;
}

char* generar_path_config(char* nombre_archivo){
	char* path_archivo = string_new();
	strcat(path_archivo,"/");
	strcat(path_archivo,nombre_archivo);
	path_archivo = crear_path(path_archivo);
	return path_archivo;

}


void  mostrar_estado_archivo(t_config* config_archivo){
	
	
	log_info(entradasalida_log_debug,"BITMAP");
	int bloque_inicial = config_get_int_value(config_archivo,"BLOQUE_INICIAL");
	int cant_bloques = ((config_get_int_value(config_archivo,"TAMANIO_ARCHIVO")/BLOCK_SIZE)+1);
	int rta;
	int bitmap_index = bloque_inicial;

	log_info(entradasalida_log_debug,"Bloque Inicial : %d ",bloque_inicial);
	log_info(entradasalida_log_debug,"Cantidad de bloques : %d ", cant_bloques);

	for (int i = 0 ; i < cant_bloques; i++)
	{	
		
		rta = bitarray_test_bit(bitmap,bitmap_index) ? 1 : 0;
		log_info(entradasalida_log_debug,"bitmap posicion %d : %d",bitmap_index,rta);
		bitmap_index++;	
	}
}

int inicializar_lista_archivos(){
	lista_archivos_existentes = list_create();
	DIR* fd_directorio = opendir(PATH_BASE_DIALFS);
    struct dirent *ent;
	char* nombre_archivo = malloc(30);
	if (fd_directorio) {
        // Lee todos los archivos y directorios dentro del directorio
        while ((ent = readdir(fd_directorio)) != NULL) {
        	if (ent->d_type == DT_REG && !ignorar(ent->d_name)) {  // Si es un archivo regular
                list_add(lista_archivos_existentes, ent->d_name);  // Agrega el nombre del archivo a la lista
            }
        }
        closedir(fd_directorio);
    } else {
        // Si no se puede abrir el directorio
        perror("Error al abrir el directorio");
        return EXIT_FAILURE;
    }

	free(nombre_archivo);
	return 1;
}

int ignorar(const char *nombre_archivo) {
    if (strcmp(nombre_archivo, "Bloques.dat") == 0 || strcmp(nombre_archivo, "Bitmap.dat") == 0 ||
        strcmp(nombre_archivo, ".") == 0 || strcmp(nombre_archivo, "..") == 0) {
        return 1;  // Ignorar este archivo/directorio
    }
    return 0;  // No ignorar este archivo/directorio
}