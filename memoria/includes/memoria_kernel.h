#ifndef MEMORIA_KERNEL_H
#define MEMORIA_KERNEL_H

#include "m_gestor.h"
#include "memoria_procesos.h"
#include "gestion_paginas.h"
#include "inicializar_memoria.h"

t_proceso* crear_proceso_nuevo();
void eliminar_instruccion(char* instruccion);
void finalizar_proceso();
void atender_kernel();

#endif /*MEMORIA_KERNEL_H*/