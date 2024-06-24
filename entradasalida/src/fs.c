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
	bool pudo_ser_truncado=false;

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
			if (nuevo_bloque_inicial!=-1)
			{	char *bloque_text=0; 
				sprintf(bloque_text,"%d",nuevo_bloque_inicial);
				config_set_value(config_archivo, "BLOQUE_INICIAL", bloque_text);
				char *tamanio_nuevo_text=0;
				sprintf(tamanio_nuevo_text,"%d",tamanio_nuevo);
				config_set_value(config_archivo, "TAMANIO_ARCHIVO", tamanio_nuevo_text);
				
				for (int i = nuevo_bloque_inicial ; i <= cant_bloques; i++)
				{
					bitarray_set_bit(bitmap,i);
				}
				pudo_ser_truncado = true; //Puede ser que tenga q cambiar esto por un return y fue :D
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
			else  /// Aca falta lo de compactación y fijarme que tengo bloque libres necesarios
			{
				return false;
			}
			
			pudo_ser_truncado = true; 
		}
		
	}else
	{	char* tamanio_text=0; //Se guarda bien?
		sprintf(tamanio_text,"%d",tamanio_nuevo);
		config_set_value(config_archivo, "TAMANIO_ARCHIVO", tamanio_text);
		int cantidad_bloques_viejo = tamanio_viejo/BLOCK_COUNT/8;
			for (int i = cant_bloques; i <= cantidad_bloques_viejo; i++) //libero
			{
				bitarray_clean_bit(bitmap,i);
			}
	}
	return pudo_ser_truncado;
}

void escribir_archivo(char* nombre_archivo,int registro_archivo,char* escrito){
	//Camino feliz entra todo :D
	char* path_archivo = malloc(strlen(PATH_BASE_DIALFS) + strlen(nombre_archivo) + 1);  
	crear_path(path_archivo,strcat("/",nombre_archivo));
	t_config* config_archivo = config_create(path_archivo);
	int bloque_inicial = config_get_int_value(config_archivo,"BLOQUE_INICIAL");
	int donde_escribir = (bloque_inicial * BLOCK_SIZE + registro_archivo); 
	strcat(bloquesEnMemoria+donde_escribir,escrito);
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
                              setear_bitmap(i,j-1); // Hacer Setear bitmap de tal a tal y sete en 1 // el -1 es poque suma siempre y acá justo no hace falta
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

void setear_bitmap(int comienzo, int final){
	for (int i = comienzo; i <= final; i++)
	{
		bitarray_set_bit(bitmap,i);
	}
	
}