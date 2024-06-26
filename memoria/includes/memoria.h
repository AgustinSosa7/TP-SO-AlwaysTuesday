#ifndef MEMORIA_H_
#define MEMORIA_H_

#include "m_gestor.h"
#include "inicializar_memoria.h"
#include "memoria_procesos.h"
#include "gestion_paginas.h"
#include "memoria_cpu_entrada_salida.h"
#include "memoria_kernel.h"

pthread_mutex_t mutex_tabla_marcos;
pthread_mutex_t mutex_tabla_paginas;
pthread_mutex_t mutex_espacio_memoria;

sem_t retardo;
sem_t ejecucion;

void* bitmap_memoria_usuario;
t_bitarray* tabla_de_marcos;
size_t size_de_tabla_marcos;
void* espacio_memoria;

void gestionar_entrada_salida();

#endif /* MEMORIA_H_*/
