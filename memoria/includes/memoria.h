#ifndef MEMORIA_H_
#define MEMORIA_H_

#include "m_gestor.h"
#include "inicializar_memoria.h"
#include "memoria_procesos.h"
#include "gestion_paginas.h"
#include "memoria_entradasalida.h"



void gestionar_entrada_salida();

void* bitmap_memoria_usuario;
t_bitarray* tabla_de_marcos;
size_t size_de_tabla_marcos;
void* espacio_memoria;

#endif /* MEMORIA_H_*/
