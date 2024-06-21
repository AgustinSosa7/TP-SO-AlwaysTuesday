#include "../includes/entradasalida.h"


void crear_config(char * nombre_archivo){
//Todo es un config y se guarda y crea con el nombre del archivo

    // VER COMO GUAROD EL PATH
	
	char* path_archivo = malloc(strlen(PATH_BASE_DIALFS) + strlen(nombre_archivo) + 1);
	path_archivo =	crear_path(PATH_BASE_DIALFS,strcat("/",nombre_archivo));
	int config_archivo = config_create(path_archivo);


    // Se crea el FCB y se agrega a la lista 
    //t_fcb * new_fcb = malloc(sizeof(t_fcb )); // dps en el destroy FCB tengo que liberar el espacio solicitado :D
    //new_fcb->nombre = malloc(strlen(nombre_archivo)+1); 
    //new_fcb->tamanio = 0; //Aunque realmente ocupa 1
    //strcpy(new_fcb->nombre,nombre_archivo);
    //list_add(lista_struct_fcbs,new_fcb);
    

    //Se hace el config en el metadata :D
    //nuevo_fcb->archivo_fcb = config_create(path_archivo);
    //new_fcb->archivo_fcb = config_create(path_archivo);
	if(config_archivo == NULL){     //Verifica si existe un archivo con ese path y si no lo crea :D //Hace falta, no lo debería solo crear osea antes no puede existir
		FILE* file_fd = fopen(path_archivo, "a+"); 
		fclose(file_fd);
		config_archivo = config_create(path_archivo);
	}
	//config_set_value(config_archivo, "NOMBRE_ARCHIVO", nombre_archivo);
	config_set_value(config_archivo, "BLOQUE_INICIAL", "NULL");
	//char* tamanio_text = malloc(10); // Ver si va ese valor
	//sprintf(tamanio_text, "%d", config_get_string_value(config_archivo,"TAMANIO_ARCHIVO")); //Pasa el tamanio expresado en int a char*
	config_set_value(config_archivo, "TAMANIO_ARCHIVO", "0");
	
	config_save(config_archivo);

}


delete_archivo(char* nombre_archivo){
	char* path_archivo = malloc(strlen(PATH_BASE_DIALFS) + strlen(nombre_archivo) + 1);  //Se podría modularizar
	path_archivo =	crear_path(PATH_BASE_DIALFS,strcat("/",nombre_archivo));
	int config_archivo = config_create(path_archivo);
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

	char* path_archivo = malloc(strlen(PATH_BASE_DIALFS) + strlen(nombre_archivo) + 1);  //Se podría modularizar
	path_archivo =	crear_path(PATH_BASE_DIALFS,strcat("/",nombre_archivo));
	int config_archivo = config_create(path_archivo);
	int bloque_inicial = config_get_int_value(config_archivo,"BLOQUE_INICIAL");
	int tamanio_viejo= config_get_int_value(config_archivo,"TAMANIO_ARCHIVO");
	int nuevo_bloque_inicial;
	int cant_bloques = tamanio_nuevo/BLOCK_SIZE/8;

	if (tamanio_nuevo>tamanio_viejo)
	{
		if (tamanio_viejo ==0)
		{
			nuevo_bloque_inicial= obtener_bloques_libres(tamanio_nuevo);
			if (nuevo_bloque_inicial!=NULL)
			{
				config_set_value(config_archivo, "BLOQUE_INICIAL", nuevo_bloque_inicial);
				config_set_value(config_archivo, "TAMANIO_ARCHIVO", tamanio_nuevo);
				
				for (int i = nuevo_bloque_inicial ; i <= cant_bloques; i++)
				{
					bitarray_set_bit(bitmap,i);
				}
				pudo_ser_truncado = true; //Puede ser que tenga q cambiar esto por un return y fue :D
			}
		} else if(tamanio_nuevo!=tamanio_viejo){
			if(puede_crecer(bloque_inicial,tamanio_nuevo,tamanio_viejo)){
				config_set_value(config_archivo, "TAMANIO_ARCHIVO", tamanio_nuevo);
				for (int i = bloque_inicial ; i <= cant_bloques; i++)
				{
					bitarray_set_bit(bitmap,i);
				}
				
			}
			else  /// Aca falta lo de compactación
			{
				return false;
			}
			
			pudo_ser_truncado = true; //Esto va acá porque si bien puede ser que tamaño nuevo sea del mismo tamaño que el viejo como es el mismo entonces e puede y no modifica nada
		}
		
	}else
	{
		config_set_value(config_archivo, "TAMANIO_ARCHIVO", tamanio_nuevo);
		int cantidad_bloques_viejo = tamanio_viejo/BLOCK_COUNT/8;
			for (int i = cant_bloques; i <= cantidad_bloques_viejo; i++) //libero
			{
				bitarray_clear_bit(bitmap,i);
			}
	}
	return pudo_ser_truncado;
}

void escribir_archivo(char* nombre_archivo,int registro_archivo,char* escrito){
	//Camino feliz entra todo :D
	char* path_archivo = malloc(strlen(PATH_BASE_DIALFS) + strlen(nombre_archivo) + 1);  //Se podría modularizar
	path_archivo =	crear_path(PATH_BASE_DIALFS,strcat("/",nombre_archivo));
	int config_archivo = config_create(path_archivo);
	int bloque_inicial = config_get_int_value(config_archivo,"BLOQUE_INICIAL");
	int donde_escribir = (bloque_inicial + registro_archivo); // hace falta *sizeof(char)?
	strcat(bloquesEnMemoria[donde_escribir],escrito);
 }


int obtener_bloques_libres(char* tamanio){
      int i,j=0;
      int tamanio_bits = tamanio/BLOCK_SIZE/8;
      bool suficiente = false;
      int rta=NULL;
      while (bitarray_get_max_bit(bitmap)>=i)
      {
            if (bitarray_test_bit(bitmap,i)) //Ta bien el int i??
            {     
                  j=i+1;
                  while(j<=tamanio_bits) // tamanio en bytes/cantidad de bloques que ocupa/ cuanos bits son :D
                  {
                        if(bitarray_test_bit(bitmap,j)){ // Se fija si el bit siguiente esta libre si es 0 no entra
                              i=j;
                              break;
                        }
                        if (j==tamanio_bits)
                        {     
                              setear_bitmap(i,j);
                              i=j;
                              suficiente = true;
                        }
                  }
                  if(!suficiente){
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

void modificar_tamanio_fcb(t_fcb* fcb,int tamanio_nuevo){
	char* tamanio_text=malloc(10);
	sprintf(tamanio_text, "%d",fcb->tamanio);
	
	for(int i = 0; i < list_size(lista_struct_fcbs); i++){ //Se puede hacer con un Iterator???
		t_fcb* archivo_a_comparar= list_get(lista_struct_fcbs, i);
		if(strcmp(archivo_a_comparar->nombre, fcb.nombre) == 0){
			config_set_value(fcb->archivo_fcb,"TAMANIO_ARCHIVO",tamanio_text);
			config_save(archivo_buscado->archivo_fcb);
		}
}
/*
void eliminar_fcb(t_peticion *peticion, int pid){
	char * nombre_archivo = peticion->parametros->registroDireccion;
	int tamanio_nuevo = peticion->parametros->registroTamanio;

	t_fcb *fcb = obtener_fcb(nombre_archivo);

}

void truncar_fs(t_fcb* fcb){
	
}*/