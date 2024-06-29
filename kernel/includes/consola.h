
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
bool validar_instruccion(char* leido);
void atender_instruccion_validada(char* leido);
bool encontrar_instruccion(void* instruccion);
bool estaa_o_no(t_instruccion* instruccion, char* nombre_instruccion);
bool validar_nombre_y_parametros(char* nombre_instruccion,int cant_parametros);
bool esta_la_instruccion(void* instruccion) ;
bool esta_o_noo(char* nombre_instruccion, int cant_parametros, t_instruccion* instruccion);
t_list* leer_archivo(char* path);
void enviar_path_a_memoria(char* path,int pid,int socket);
void imprimir_lista(t_listas_estados* lista_a_mostrar);
void imprimir_lista_blocked_recursos();
#endif
