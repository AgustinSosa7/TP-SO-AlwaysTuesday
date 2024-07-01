#include "../includes/entradasalida.h"


//Fijarse los SPRITNF y que hago si le poongo 0 o malloc


void crear_config(char * nombre_archivo){
	//Todo es un config y se guarda y crea con el nombre del archivo

	char* path_archivo = malloc(strlen(PATH_BASE_DIALFS) + strlen(nombre_archivo) + 1);
	 crear_path(path_archivo,strcat("/",nombre_archivo));
	t_config* config_archivo = config_create(path_archivo);

	if(config_archivo == NULL){     //Verifica si existe un archivo con ese path y si no lo crea :D //Hace falta, no lo debería solo crear osea antes no puede existir?? jaja
		FILE* file_fd = fopen(path_archivo, "a+"); 
		fclose(file_fd);
		config_archivo = config_create(path_archivo);
	}
	
	config_set_value(config_archivo, "BLOQUE_INICIAL", "NULL");
	config_set_value(config_archivo, "TAMANIO_ARCHIVO", "0");
	
	config_save(config_archivo);

}


void delete_archivo(char* nombre_archivo){
	char* path_archivo = malloc(strlen(PATH_BASE_DIALFS) + strlen(nombre_archivo) + 1);  
	crear_path(path_archivo ,strcat("/",nombre_archivo));
	t_config* config_archivo = config_create(path_archivo);
	int bloque_inicial = config_get_int_value(config_archivo,"BLOQUE_INICIAL");
	int tamanio = config_get_int_value(config_archivo,"TAMANIO_ARCHIVO");
	int cant_bloques = tamanio / BLOCK_SIZE/8;
	for (int i = bloque_inicial ; i <= cant_bloques; i++)
	{
		bitarray_clean_bit(bitmap,i);
	}
	
	config_destroy(config_archivo);
	

}

bool truncar_archivo(char* nombre_archivo,int tamanio_nuevo){

	char* path_archivo = malloc(strlen(PATH_BASE_DIALFS) + strlen(nombre_archivo) + 1); 
	crear_path(path_archivo,strcat("/",nombre_archivo));
	t_config* config_archivo = config_create(path_archivo);
	int bloque_inicial = config_get_int_value(config_archivo,"BLOQUE_INICIAL");
	int tamanio_viejo= config_get_int_value(config_archivo,"TAMANIO_ARCHIVO");
	int nuevo_bloque_inicial;
	int cant_bloques = tamanio_nuevo/BLOCK_SIZE/8;

	if (tamanio_nuevo>tamanio_viejo)
	{
		if (tamanio_viejo ==0)
		{
			nuevo_bloque_inicial= obtener_bloques_libres(tamanio_nuevo);
			
			if (nuevo_bloque_inicial==-1){
				return false;
			}
				char *bloque_text=0; 
				sprintf(bloque_text,"%d",nuevo_bloque_inicial);
				config_set_value(config_archivo, "BLOQUE_INICIAL", bloque_text);
				char *tamanio_nuevo_text=0;
				sprintf(tamanio_nuevo_text,"%d",tamanio_nuevo);
				config_set_value(config_archivo, "TAMANIO_ARCHIVO", tamanio_nuevo_text);
				
				for (int i = nuevo_bloque_inicial ; i <= cant_bloques; i++)
				{
					bitarray_set_bit(bitmap,i);
				}

		} else if(tamanio_nuevo!=tamanio_viejo){
			if(puede_crecer(bloque_inicial,tamanio_nuevo,tamanio_viejo)){
				char *tamanio_nuevo_text=0;
				sprintf(tamanio_nuevo_text,"%d",tamanio_nuevo);
				config_set_value(config_archivo, "TAMANIO_ARCHIVO", tamanio_nuevo_text);
				for (int i = bloque_inicial ; i <= cant_bloques; i++)
				{
					bitarray_set_bit(bitmap,i);
				}
			
			}
			else  // Aca falta lo de compactación y fijarme que tengo bloque libres necesarios
			{
				if (!hay_espacios(bloque_inicial,tamanio_nuevo-tamanio_viejo)) //si no hay entra D:
				{
					return false;
				}
				nuevo_bloque_inicial = compactacion(bloque_inicial,tamanio_viejo,tamanio_nuevo);
				char *bloque_text=0; 
				sprintf(bloque_text,"%d",nuevo_bloque_inicial);
				config_set_value(config_archivo, "BLOQUE_INICIAL", bloque_text);
				char *tamanio_nuevo_text=0;
				sprintf(tamanio_nuevo_text,"%d",tamanio_nuevo);
				config_set_value(config_archivo, "TAMANIO_ARCHIVO", tamanio_nuevo_text);
			}
			
		}
		
	}else // o el tam nuevo es igual o menor así que sirve si o si :D
	{	char* tamanio_text=0; //Se guarda bien?
		sprintf(tamanio_text,"%d",tamanio_nuevo);
		config_set_value(config_archivo, "TAMANIO_ARCHIVO", tamanio_text);
		int cantidad_bloques_viejo = tamanio_viejo/BLOCK_COUNT/8;
			for (int i = cant_bloques; i <= cantidad_bloques_viejo; i++) //libero
			{
				bitarray_clean_bit(bitmap,i);
			}
			
	}
	return true;
}

bool escribir_archivo(char* nombre_archivo,int registro_archivo,char* escrito){
	//Camino feliz entra todo :D
	char* path_archivo = malloc(strlen(PATH_BASE_DIALFS) + strlen(nombre_archivo) + 1);  
	crear_path(path_archivo,strcat("/",nombre_archivo));
	t_config* config_archivo = config_create(path_archivo);
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
	char* path_archivo = malloc(strlen(PATH_BASE_DIALFS) + strlen(nombre_archivo) + 1);  
	crear_path(path_archivo,strcat("/",nombre_archivo));
	t_config* config_archivo = config_create(path_archivo);
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
      int i,j,bloques_libres=0;
      int tamanio_bits = tamanio/BLOCK_SIZE/8;
      bool suficiente = false;
      int rta=-1;
      while (bitarray_get_max_bit(bitmap)>i)
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
                  if(suficiente){
                        rta= i;
                        break;
                  }
            }
            i++;
      }
      return rta;
      
}

bool puede_crecer(int bloque_inicial, int tamanio_viejo,int tamanio_nuevo){
	for (int i = bloque_inicial+tamanio_viejo; i <= bloque_inicial+tamanio_viejo; i++)
	{
		if (bitarray_test_bit(bitmap,i))
		{
			return false;
			break;
		}
		
	}
	
	return true;
}

void setear_bitmap(int comienzo, int final,bool asigna){
	for (int i = comienzo; i <= final; i++)
	{	
		asigna ? bitarray_set_bit(bitmap,i) : bitarray_clean_bit(bitmap,i);
	}
	
}

bool hay_espacios(int bloque_inicial, int bloques_faltantes){
	int bloques_libres = 0;
	for (int i = 0; i < bitarray_get_max_bit(bitmap); i++)
	{
		if (!bitarray_test_bit(bitmap,i)) 
		{
			bloques_libres ++;
		}
	}

	return bloques_libres >= bloques_faltantes;
}

int compactacion(int bloque_inicial,int tamanio_viejo, int tamanio_nuevo){
	char* archivo_aux= malloc(tamanio_viejo);
	int cant_bloques =tamanio_viejo/BLOCK_SIZE/8;
	setear_bitmap(bloque_inicial,cant_bloques,false);
	for (int i = 0; i < tamanio_viejo; i++)
	{
		strcat(archivo_aux+i,bloquesEnMemoria+bloque_inicial*BLOCK_SIZE+i);
	}
	int nuevo_bloque_inicial = mover_archivos();
	
	strcat(bloquesEnMemoria+nuevo_bloque_inicial*BLOCK_SIZE,archivo_aux);  //copio lo que esté escrito
	
	setear_bitmap(nuevo_bloque_inicial,tamanio_nuevo/8,true);

	free(archivo_aux);
	sleep(1000*RETRASO_COMPACTACION);
	return nuevo_bloque_inicial;
}

int mover_archivos(){
	int tamanio_archivo=BLOCK_COUNT*BLOCK_SIZE;
	int primer_bloque_libre, ultimo_bloque_ocupado,primer_bloque_ocupado = 0;
	int nuevo_bloque_inicial;
	for (int i = 0; i < tamanio_archivo; i++)
	{	if (bitarray_test_bit(bitmap,i)) //Esta libre ? es el primero, cuantos mas hay?
	{
		primer_bloque_libre = i;

		for (int j = i; j < tamanio_archivo; j++)
		{
		
		if (bitarray_test_bit(bitmap,j)) // entra si está ocupado
		{	
			primer_bloque_ocupado=j;
			ultimo_bloque_ocupado = tamanio_bloque_escrito(primer_bloque_libre,tamanio_archivo);
			i = copiar_archivo(primer_bloque_libre,primer_bloque_ocupado,ultimo_bloque_ocupado); // devuelve el ult bloque ocupado
			nuevo_bloque_inicial = i;
			break;
		}
			
		}
	}
	}
	return nuevo_bloque_inicial;
}

int tamanio_bloque_escrito(int primer_bloque_libre, int tamanio_archivo){

	for (int i = 0; i < BLOCK_COUNT/8; i++)
	{
		if (!bitarray_test_bit(bitmap,i))
		{
			return i-1;
		}
	}
	return BLOCK_COUNT/8;
}

int copiar_archivo(int primer_bloque_libre, int primer_bloque_ocupado, int ultimo_bloque_ocupado){
	int tamanio = ultimo_bloque_ocupado - primer_bloque_libre;
	char* archivo_aux = malloc(tamanio*8*BLOCK_SIZE);
	for (int i = 0; i < tamanio*BLOCK_SIZE*8; i++)
	{
		strcat(archivo_aux+i,bloquesEnMemoria+primer_bloque_ocupado*BLOCK_SIZE+i);  //copio lo que esté escrito
	}
	setear_bitmap(primer_bloque_ocupado,ultimo_bloque_ocupado,false); //Libero el bitmap
	setear_bitmap(primer_bloque_libre,tamanio,true); //asigno segun el tamanio
	strcat(bloquesEnMemoria+primer_bloque_libre*BLOCK_SIZE,archivo_aux); //copio todo
	free(archivo_aux);
	return primer_bloque_libre + tamanio;
}