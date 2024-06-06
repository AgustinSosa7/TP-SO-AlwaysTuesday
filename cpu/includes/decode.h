#ifndef DECODE_H_
#define DECODE_H_

#include "cpu_gestor.h"

cod_instruccion decodificacion_instruccion(char *instruccion);
bool codigo_inexistente(char* instruccion);
bool requiere_traduccion(char * instruccion);
cod_instruccion identificador_instruccion(char* codigo);

#endif 