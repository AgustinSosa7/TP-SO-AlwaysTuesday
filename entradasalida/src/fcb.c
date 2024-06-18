#include "../includes/entradasalida.h"


void crear_fcb(char * nombre_archivo){
    // VER COMO GUAROD EL PATH
	char* path_archivo = malloc(strlen(PATH_METADATA) + strlen(nombre_archivo) + 1);
	strcpy(path_archivo, PATH_METADATA);
	strcat(path_archivo, nombre_archivo);

    // Se crea el FCB y se agrega a la lista 
    t_fcb * new_fcb = malloc(sizeof(t_fcb )); // dps en el destroy FCB tengo que liberar el espacio solicitado :D
    new_fcb->nombre = malloc(strlen(nombre_archivo)+1); 
    new_fcb->tamanio = 0; //Aunque realmente ocupa 1
    strcpy(new_fcb->nombre,nombre_archivo);
    list_add(lista_struct_fcbs,new_fcb);
    char* tamanio_text = malloc(10); // Ver si va ese valor
	sprintf(tamanio_text, "%d", new_fcb->tamanio); //Pasa el tamanio expresado en int a char*

    //Se hace el config en el metadata :D
    //nuevo_fcb->archivo_fcb = config_create(path_archivo);
    new_fcb->archivo_fcb = config_create(path_archivo);
	if(new_fcb->archivo_fcb == NULL){     //Verifica si existe un archivo con ese path y si no lo crea :D
		FILE* file_fcb = fopen(path_archivo, "a+"); 
		fclose(file_fcb);
		new_fcb->archivo_fcb = config_create(path_archivo);
	}
	config_set_value(new_fcb->archivo_fcb, "NOMBRE_ARCHIVO", new_fcb->nombre);
	config_set_value(new_fcb->archivo_fcb, "BLOQUE_INICIAL", "NULL");
	config_set_value(new_fcb->archivo_fcb, "TAMANIO_ARCHIVO", tamanio_text);
	

	config_save(new_fcb->archivo_fcb);

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