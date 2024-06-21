#ifndef MEMORIA_CPU_H
#define MEMORIA_CPU_H

#include "m_gestor.h"
#include "memoria_procesos.h"
#include "gestion_paginas.h"

void enviar_info_inicial();
void recibir_pedido_instruccion_y_enviar();
void recibir_pedido_marco_y_enviar();
void atender_cpu();


#endif /*MEMORIA_CPU_H*/