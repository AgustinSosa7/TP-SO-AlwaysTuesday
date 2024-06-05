#include "../includes/execute.h"


void ejecucion_proceso(char* codigo){
cod_instruccion instruccion = identificador_instruccion(codigo);
switch (instruccion)
{
    case SET:

    break;

    case SUM:

    break;

    case SUB:

    break;

    case JNZ:

    break;

    case IO_GEN_SLEEP:
    
    break;

    case -1: //Puede llegar una instruccion rota?

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

