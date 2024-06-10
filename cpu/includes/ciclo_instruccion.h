#ifndef CICLO_INSTRUCCION_H
#define CICLO_INSTRUCCION_H

#include "cpu_gestor.h"
#include "cpu_memoria.h"
#include "stdbool.h"
#include <commons/log.h>
#include <commons/config.h>
#include "../includes/cpu_kernel_interrupt.h"
void ciclo_instruccion();

//FETCH

char** ciclo_instruccion_fetch();

//DECODE

void decodificacion_instruccion(char *instruccion);
bool codigo_inexistente(char* instruccion);
//bool requiere_traduccion(char * instruccion);


//EXECUTE

void ejecucion_proceso(char** codigo);
cod_instruccion identificador_instruccion(char* codigo);

int obtener_tamanio_registro(char* registroTamanio);
bool es_de_proposito_general(char* registroTamanio);
bool es_extendido(char* registroTamanio);


#endif

