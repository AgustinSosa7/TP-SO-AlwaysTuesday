#include "../includes/execute.h"


void ejecucion_proceso(cod_instruccion instruccion, char** codigo){
//cod_instruccion instruccion = identificador_instruccion(codigo);
switch (instruccion)
{
    case SET: //Solo para probar
        printf("Spliteado param0: %s\n",codigo[0]);
		printf("Spliteado param1: %s\n",codigo[1]);
		printf("Spliteado param2: %s\n",codigo[2]);
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



