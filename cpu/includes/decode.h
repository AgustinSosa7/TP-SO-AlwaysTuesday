#ifndef DECODE_H_
#define DECODE_H_

#include "cpu_gestor.h"

char* decodificacion_instruccion(t_list * lista_de_instrucciones);
bool codigo_inexistente(char* instruccion);
bool requiere_traduccion(char * instruccion);

#endif 