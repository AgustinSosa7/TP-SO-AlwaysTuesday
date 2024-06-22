#ifndef MEMORIA_ENTRADASALIDA_H_
#define MEMORIA_ENTRADASALIDA_H_

#include "m_gestor.h"


void atender_entradasalida();

bool guardar_leido_en_registro(char* leido, int registroDireccion);
char* devolver_registro(char* registro_Direccion, int registroTamanio);

#endif /* MEMORIA_H_*/