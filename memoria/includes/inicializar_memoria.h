#ifndef INICIALIZAR_MEMORIA_H_
#define INICIALIZAR_MEMORIA_H_

#include "m_gestor.h"
#include "gestion_paginas.h"


void inicializar_memoria(char* path);
void _iniciar_log();
void _iniciar_config(char* path);
void _imprimir_config();
void _inicializar_listas();
void _inicializar_estructuras();
void _inicializar_semaforos();
void hilo_retardo();

#endif /*INICIALIZAR_MEMORIA_H_*/