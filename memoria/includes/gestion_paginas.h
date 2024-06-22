#ifndef GESTION_PAGINAS_H_
#define GESTION_PAGINAS_H_

#include "m_gestor.h"

void ocupar_marco(int);
void liberar_marco(int);
int traer_numero_marco(t_proceso*,int);
int buscar_marco_libre();
void cambiar_tamanio_proceso(t_proceso*,int);
void cambiar_variable_long_tabla_pags(t_proceso*,int);
void asignar_marcos_memoria(t_proceso*,int);
void liberar_marcos_memoria(t_proceso*,int);
int calcular_paginas_necesarias(int);
int calcular_marcos_libres();
int ajustar_tamanio_proceso(t_proceso*,int);


#endif