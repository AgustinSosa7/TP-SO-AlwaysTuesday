#include "../includes/entradasalida.h"


void crear_fcb(char * nombre_archivo){
    // VER COMO GUAROD EL PATH
	char* path_archivo = malloc(strlen(PATH_METADATA) + strlen(nombre_archivo) + 1);
	strcpy(path_archivo, PATH_METADATA);
	strcat(path_archivo, nombre_archivo);

    // Se crea el FCB y se agrega a la lista 
    t_fcb * new_fcb = malloc(sizeof(t_fcb )); // dps en el destroy FCB tengo que liberar el espacio solicitado :D
    new_fcb->nombre = malloc(strlen(nombre_archivo)+1); 
    new_fcb->tamanio = 1;
    strcpy(new_fcb->nombre,nombre_archivo);
    list_add(lista_struct_fcbs,new_fcb);
    char* text_tamanio_archivo = malloc(10); //el valor de 10 será el valor de cuantos bits como maximo puede ocupar. Ver si va ese valor
	//sprintf(text_tamanio_archivo, "%d", new_fcb->tamanio);

    //Se hace el config en el metadata :D
    //nuevo_fcb->archivo_fcb = config_create(path_archivo);
    new_fcb->archivo_fcb = config_create(path_archivo);
	if(new_fcb->archivo_fcb == NULL){     //Verifica si existe un archivo con ese path y si no lo crea :D
		FILE* file_fcb = fopen(path_archivo, "a+"); 
		fclose(file_fcb);
		new_fcb->archivo_fcb = config_create(path_archivo);
	}
	config_set_value(new_fcb->archivo_fcb, "NOMBRE_ARCHIVO", new_fcb->nombre);
	config_set_value(new_fcb->archivo_fcb, "TAMANIO_ARCHIVO", text_tamanio_archivo);
	config_set_value(new_fcb->archivo_fcb, "BLOQUE_INICIAL", "NULL");

	config_save(new_fcb->archivo_fcb);

}
