#ifndef EXECUTE_H_
#define EXECUTE_H_

#include "cpu_gestor.h"

//Estructuras

typedef enum 
{
    SET,
    MOV_IN,
    MOV_OUT,
    SUM,
    SUB,
    JNZ,
    RESIZE,
    COPY_STRING,
    WAIT,
    SIGNAL,
    IO_GEN_SLEEP,
    IO_STDIN_READ,
    IO_STDOUT_WRITE,
    IO_FS_CREATE,
    IO_FS_DELETE,
    IO_FS_TRUNCATE,
    IO_FS_WRITE,
    IO_FS_READ,
    EXIT_CPU
}cod_instruccion;

//Funciones
void ejecucion_proceso(char* codigo);
cod_instruccion identificador_instruccion(char* codigo);

#endif 