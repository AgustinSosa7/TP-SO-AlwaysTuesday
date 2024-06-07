
#ifndef CONSOLA_H_
#define CONSOLA_H_

#include "k_gestor.h"

#include "pcb.h"

typedef enum{
	EJECUTAR_SCRIPT,
    INICIAR_PROCESO,
    FINALIZAR_PROCESO,
    DETENER_PLANIFICACION,
    INICIAR_PLANIFICACION,
    MULTIPROGRAMACION,
    PROCESO_ESTADO
}op_code_instruccion;

typedef struct {
    op_code_instruccion op_code_instruccion;
    int cant_parametros;
    char* nombre;
}t_instruccion;

void iniciar_consola();
void agregar_instruccion(t_list* lista_instrucciones, op_code_instruccion op_code, int parametros,char* op_code_string);
void leer_consola();
void leer_comandos();
//bool validar_instruccion(char* leido);
void atender_instruccion_validada(char* leido);
//op_code_instruccion encontrar_op_code(char* leido);

#endif
