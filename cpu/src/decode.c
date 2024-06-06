#include "stdbool.h"
#include "../includes/decode.h"


cod_instruccion decodificacion_instruccion(char *instruccion){
	

	if(codigo_inexistente(instruccion)){ 
		//log_error(cpu_logger, "Instruccion no encontrada: [PC: %d][Instruc_Header: %s]", contexto->proceso_ip, instruccion_a_ejecutar[0]);
		log_error(cpu_logger, "Hubo un error leyendo %s",instruccion);
		exit(EXIT_FAILURE); 
	}
	//Semaforo que active a Execute
    //El tp dice que acá debería fijarse si necesita que entra la MMU o no.
	
	return identificador_instruccion(instruccion);
	//requiere de una traducción de dirección lógica a dirección física. ¿QUÉ SERÍA ESTO?
}


// Esta funcion devuelve True si la cabecera de la lista está mal
bool codigo_inexistente(char* instruccion){

	bool respuesta = true;
	int i = 0; 
	char**  instrucciones = "codigo a comparazr" ;// Todos los codigos metidos ahí para que sea un codigo valido :D inicializa instrucciones :D
	while(instrucciones[i] != NULL){
		if((strcmp(instrucciones, instruccion)) == 0) {
			respuesta = false;
			printf("La instruccion %s existe ",instruccion);
			break; // para que no siga buscando 
		}
	i++;
	}
	
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


cod_instruccion identificador_instruccion(char* codigo){
   	cod_instruccion instruccion_a_ejecutar = -1;
    if(strcmp(codigo, "SET") == 0){//[SET][AX][22]
    instruccion_a_ejecutar = SET;
    
    }else if(strcmp(codigo, "SUM") == 0){//[SUM][destino:AX][origen:BX]
    instruccion_a_ejecutar = SUM;

	}else if(strcmp(codigo, "SUB") == 0){//[SUB][destino:AX][origen:BX]
    instruccion_a_ejecutar = SUB;

	}else if(strcmp(codigo, "JNZ") == 0){// [JNZ][Registro][Instruccion]
    instruccion_a_ejecutar = JNZ;

    }else if(strcmp(codigo, "MOV_IN") == 0){
    instruccion_a_ejecutar = MOV_IN;

    }else if(strcmp(codigo, "MOV_OUT") == 0){
    instruccion_a_ejecutar = MOV_OUT;

    }else if(strcmp(codigo, "RESIZE") == 0){    
    instruccion_a_ejecutar = RESIZE;

    }else if(strcmp(codigo, "COPY_STRING") == 0){
    instruccion_a_ejecutar = COPY_STRING;

    }else if(strcmp(codigo, "WAIT") == 0){
    instruccion_a_ejecutar = WAIT;

    }else if(strcmp(codigo, "SIGNAL") == 0){
    instruccion_a_ejecutar = SIGNAL;

    }else if(strcmp(codigo, "IO_GEN_SLEEP") == 0){ // [SLEEP][tiempo]
    instruccion_a_ejecutar = IO_GEN_SLEEP;

    }else if(strcmp(codigo, "IO_STDIN_READ") == 0){
    instruccion_a_ejecutar = IO_STDIN_READ;

    }else if(strcmp(codigo, "IO_STDOUT_WRITE") == 0){
    instruccion_a_ejecutar = IO_STDOUT_WRITE;

    }else if(strcmp(codigo, "IO_FS_CREATE") == 0){
    instruccion_a_ejecutar = IO_FS_CREATE;

    }else if(strcmp(codigo, "IO_FS_DELETE") == 0){
    instruccion_a_ejecutar = IO_FS_DELETE;

    }else if(strcmp(codigo, "IO_FS_TRUNCATE") == 0){
    instruccion_a_ejecutar = IO_FS_TRUNCATE;

    }else if(strcmp(codigo, "IO_FS_WRITE") == 0){
    instruccion_a_ejecutar = IO_FS_WRITE;

    }else if(strcmp(codigo, "IO_FS_READ") == 0){
    instruccion_a_ejecutar = IO_FS_READ;

    }else if(strcmp(codigo, "EXIT") == 0){         
    instruccion_a_ejecutar = EXIT;

}  
    return instruccion_a_ejecutar;
} 


//char * traduccion_logica_fisicac(char* instruccion , t_list lista_de_instrucciones){
//	string_append(instruccion,decodificacion_instruccion(lista_de_instrucciones));
//}


////////////////////////MMU///////////////////////
/*

if(requiere_traduccion(instruccion)){ // Lo debería hacer la MMU | Concatena esto al string y vuelve a fijarse si necesita denuevo eso
		instruccion->head = instruccion;
		string_append(&instruccion,decodificacion_instruccion(instruccion));  
	}

	instruccion->head = instruccion;
	instruccion->elements_count = lista_de_instrucciones->elements_count -1;

*/