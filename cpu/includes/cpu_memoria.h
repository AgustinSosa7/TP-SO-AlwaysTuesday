#ifndef CPU_MEMORIA_H_
#define CPU_MEMORIA_H_

#include "cpu_gestor.h"

// Tamaño pagina y tamaño memoria
void pedir_info_inicial_a_memoria();

// Instrucciones
void pedir_instruccion_pseudocodigo(int pid,int pc);
char* recibir_instruccion_pseudocodigo(); 
int pedir_ajustar_tamanio_del_proceso(int pid, int tamanioNuevo);

// Marco
int pedir_numero_de_marco_a_memoria(int numero_de_pagina);

// Lectura y Escritura
u_int8_t leer_1byte_de_memoria(int pid, int direccion_fisica);
u_int32_t leer_4byte_de_memoria(int pid, int direccion_fisica);
void* leer_valor_de_memoria(int pid, int direccion_fisica, int tamanio);

void escribir_1byte_en_memoria(int pid, int direccion_fisica, u_int8_t valor_a_escribir);
void escribir_4byte_en_memoria(int pid, int direccion_fisica, u_int32_t valor_a_escribir);
void escribir_valor_en_memoria(int pid, int direccion_fisica, int tamanio, u_int32_t valor_a_escribir);

#endif 