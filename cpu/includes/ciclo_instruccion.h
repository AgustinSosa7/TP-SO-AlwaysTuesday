#ifndef CICLO_INSTRUCCION_H
#define CICLO_INSTRUCCION_H

#include "cpu_gestor.h"
#include "cpu_memoria.h"
#include "cpu_kernel_interrupt.h"
#include "stdbool.h"
#include <commons/log.h>
#include <commons/config.h>

void ciclo_instruccion();

void devolver_contexto_por_ser_interrumpido();
void devolver_contexto_por_sleep(char* nombre_instruccion, char* nombre_interfaz, int segundos_sleep);

void escribir_valor_a_registro(char *nombre_registro, u_int32_t valor);
uint32_t leer_valor_de_registro(char *nombre_registro);

/*
//FETCH

char** ciclo_instruccion_fetch();

//DECODE

void decodificacion_instruccion(char *instruccion);
bool codigo_inexistente(char* instruccion);
//bool requiere_traduccion(char * instruccion);


//EXECUTE

void ejecucion_proceso(char** codigo);
cod_instruccion identificador_instruccion(char* codigo);
*/

#endif