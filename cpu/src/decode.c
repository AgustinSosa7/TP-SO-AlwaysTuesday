#include "stdbool.h"
#include "../includes/decode.h"


char* decodificacion_instruccion(t_list *lista_de_instrucciones){
	char * instruccion;
	
	if(codigo_inexistente(lista_de_instrucciones->head->data)){ // Trato a la lista como un vector, está bien?
		//log_error(cpu_logger, "Instruccion no encontrada: [PC: %d][Instruc_Header: %s]", contexto->proceso_ip, instruccion_a_ejecutar[0]);
		instruccion = lista_de_instrucciones->head->data;
		log_error(cpu_logger, "Hubo un error leyendo %s",instruccion);
		exit(EXIT_FAILURE); 
	}
	//Semaforo que active a Execute
	instruccion = lista_de_instrucciones->head->data;

	if(requiere_traduccion(lista_de_instrucciones->head->data)){ // Lo debería hacer la MMU | Concatena esto al string y vuelve a fijarse si necesita denuevo eso
		lista_de_instrucciones->head = lista_de_instrucciones->head->next; 
		string_append(&instruccion,decodificacion_instruccion(lista_de_instrucciones));  
	}

	lista_de_instrucciones->head = lista_de_instrucciones->head->next; 
	lista_de_instrucciones->elements_count = lista_de_instrucciones->elements_count -1;
	return instruccion;
	//requiere de una traducción de dirección lógica a dirección física. ¿QUÉ SERÍA ESTO?
}


// Esta funcion devuelve True si la cabecera de la lista está mal
bool codigo_inexistente(char* instruccion){
	// strign debería tener todas las opciones de codigo y verificar si la cabecera de listas de instrucciones aparece.
	bool respuesta = true;
	//int i = 0;
	char*  string = "codigo a comparar";
	while(strcmp(string,"")==0){
		if((strcmp(string, instruccion)) == 0) {
			respuesta = false;
			break; // para que no siga buscando 
		}
		//i++;
	}
	printf("La instruccion es = %s",instruccion);
	return respuesta;
}


	//Ver linea 27 de memoria_procesos
bool requiere_traduccion(char * instruccion){
	int tamaño_pagina = 16; // seguro esto tendría que ir en el utils para que sea variable global
	int longitud = tamaño_pagina;
		if(instruccion[longitud - 1] == '\n'){
			return true;
	}	
	return false;
}


//char * traduccion_logica_fisicac(char* instruccion , t_list lista_de_instrucciones){
//	string_append(instruccion,decodificacion_instruccion(lista_de_instrucciones));
//}


