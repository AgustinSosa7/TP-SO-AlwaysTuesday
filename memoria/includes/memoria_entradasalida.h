#ifndef MEMORIA_ENTRADASALIDA_H_
#define MEMORIA_ENTRADASALIDA_H_

#include "m_gestor.h"


void atender_entradasalida();

void guardar_leido_en_registro(char* leido, char* registroDireccion);
char* devolver_registro(char* registro_Direccion, int registroTamanio);

#endif /* MEMORIA_H_*/