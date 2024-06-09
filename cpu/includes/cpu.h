#ifndef CPU_H_
#define CPU_H_

#include <../src/utils/utils_connections.h>
#include "cpu_gestor.h"
#include "inicializar_cpu.h"
#include "cpu_kernel_dispatch.h"
#include "cpu_kernel_interrupt.h"
#include "cpu_memoria.h"
#include "ciclo_instruccion.h"


void ciclo_instruccion();
char ** ciclo_instruccion_fetch();
void decodificacion_instruccion(char *instruccion);
bool codigo_inexistente(char *instruccion);
//bool requiere_traduccion(char *instruccion);
void ejecucion_proceso(char** codigo);
cod_instruccion identificador_instruccion(char* codigo);


#endif
