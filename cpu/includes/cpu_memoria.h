#ifndef CPU_MEMORIA_H_
#define CPU_MEMORIA_H_

#include "cpu_gestor.h"

// Tamaño pagina y tamaño memoria
void pedir_info_inicial_a_memoria();

// Instrucciones
void pedir_instruccion_pseudocodigo(int pid,int pc);
char* recibir_instruccion_pseudocodigo(); 

// Marco
int pedir_numero_de_marco_a_memoria(int numero_de_pagina);

// Lectura y Escritura
u_int32_t leer_valor_de_memoria(int pid, int direccion_fisica);
void escribir_valor_en_memoria(int pid, int direccion_fisica, u_int32_t valor_a_escribir);

#endif 