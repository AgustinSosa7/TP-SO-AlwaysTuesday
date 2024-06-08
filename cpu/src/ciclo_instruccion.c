#include "../includes/ciclo_instruccion.h"


void ciclo_instruccion(){                         
    //Wait ciclo de instruccion
    while(1){
    
    //FETCH 

    char ** instruccion = ciclo_instruccion_fetch();  

    //DECODE
    
    decodificacion_instruccion(instruccion[0]); //Va una lista de Strings

    //EXECUTE

    ejecucion_proceso(instruccion); // Podría hacerse dentro el decode

    //CHECK INTERRUPT
    //verif del flag que modifico el kernel luego de mandar la interrupcion
    }  // El Check I. debe estar en el while????
    //atender interrupciones
}


    /////////////////////////////////////////////     FETCH    ////////////////////////////////////////


char ** ciclo_instruccion_fetch(){ //
    
	
	//sacar
	printf("Cree fetch\n");
	char* pseudo;
	char** instruccion_spliteada;
	//fin sacar

		log_info(cpu_log_debug, "PID: <%d> - FETCH - PC: <%d>", pcb_global->pid, pcb_global->registros_cpu->PC);
		pedir_instruccion_pseudocodigo(pcb_global->pid,pcb_global->registros_cpu->PC);
		
		//sacar
		pseudo = recibir_instruccion_pseudocodigo(); 
		printf("Recibi el pseudo: %s\n",pseudo);

		instruccion_spliteada = string_split(pseudo, " ");
		printf("Spliteado param0fetch: %s\n",instruccion_spliteada[0]);
		printf("Spliteado param1fetch: %s\n",instruccion_spliteada[1]);
		printf("Spliteado param2fetch: %s\n",instruccion_spliteada[2]);
		
		pcb_global->registros_cpu->PC++;

		return instruccion_spliteada;
		//fin sacar
	}
    /////////////////////////////////////////////     DECODE    ////////////////////////////////////////


  void decodificacion_instruccion(char *instruccion){
	

	if(codigo_inexistente(instruccion)){ 
	
		log_error(cpu_logger, "Hubo un error leyendo %s",instruccion);
		exit(EXIT_FAILURE); 
	}
	//El tp dice que acá debería fijarse si necesita que entra la MMU o no
	//requiere de una traducción de dirección lógica a dirección física. ¿QUÉ SERÍA ESTO?
}


// Esta funcion devuelve True si la cabecera de la lista está mal
bool codigo_inexistente(char* instruccion){

	bool respuesta = true;
	int i = 0; 
	
	while(opcode_cpu[i] != NULL){
		if((opcode_cpu[i], instruccion) == 0) {
			respuesta = false;
			printf("La instruccion %s existe ",instruccion);
			break; // para que no siga buscando 
		}
	i++;
	}
	
	return respuesta;
}


/*
	//Ver linea 27 de memoria_procesos
bool requiere_traduccion(char * instruccion){
	int tamaño_pagina = 16; // seguro esto tendría que ir en el utils para que sea variable global
	int longitud = tamaño_pagina;
		if(instruccion[longitud - 1] == '\n'){
			return true;
	}	
	return false;
}
*/


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

    /////////////////////////////////////////////     EXECUTE    ////////////////////////////////////////


    void ejecucion_proceso(char** instruccion){
switch (identificador_instruccion(instruccion[0]))
{
    case SET: //Solo para probar
        
    break;

    case SUM:

    break;

    case SUB:

    break;

    case JNZ:

    break;

    case IO_GEN_SLEEP:
    
    break;

default:
    break;
}
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
