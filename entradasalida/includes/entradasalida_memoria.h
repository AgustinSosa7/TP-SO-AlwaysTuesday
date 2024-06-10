#ifndef ENTRADASALIDA_MEMORIA_H_
#define ENTRADASALIDA_MEMORIA_H

#include "es_gestor.h"

bool guardar_en_memoria(char* leido, char* registroDireccion);
char* pedir_a_memoria(char* registroDireccion, int registroTamanio);

#endif 