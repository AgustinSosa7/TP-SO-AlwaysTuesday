#ifndef MEMORIA_CPU_ENTRADA_SALIDA_H
#define MEMORIA_CPU_ENTRADA_SALIDA_H

#include "m_gestor.h"
#include "memoria_procesos.h"
#include "gestion_paginas.h"

void enviar_info_inicial();
void recibir_pedido_instruccion_y_enviar();
void recibir_pedido_marco_y_enviar();
void recibir_solicitud_de_lectura(int); //necesita el socket
void atender_cpu();
void atender_entradasalida();




#endif /*MEMORIA_CPU_H*/