#ifndef CICLO_INSTRUCCION_H
#define CICLO_INSTRUCCION_H

#include "cpu_gestor.h"
#include "cpu_memoria.h"
#include "cpu_kernel_dispatch.h"
#include "cpu_kernel_interrupt.h"
#include "stdbool.h"
#include <commons/log.h>
#include <commons/config.h>
#include <math.h>

void ciclo_instruccion();

void devolver_contexto_por_ser_interrumpido();
void devolver_contexto_por_page_fault(int numero_de_pagina);
void devolver_contexto_por_out_of_memory();

void devolver_contexto_por_wait(char* nombre_recurso);
void devolver_contexto_por_signal(char* nombre_recurso);
void devolver_contexto_por_sleep(char* nombre_instruccion, char* nombre_interfaz, int segundos_sleep);
void devolver_contexto_por_stdin_read(char* nombre_instruccion, char* nombre_interfaz, int direccion_fisica, int tamanio);
void devolver_contexto_por_stdout_write(char* nombre_instruccion, char* nombre_interfaz, int direccion_fisica, int tamanio);
void devolver_contexto_por_fs_create(char* nombre_instruccion, char* nombre_interfaz, char* nombre_archivo);
void devolver_contexto_por_fs_delete(char* nombre_instruccion, char* nombre_interfaz, char* nombre_archivo);
void devolver_contexto_por_fs_truncate(char* nombre_instruccion, char* nombre_interfaz, char* nombre_archivo, int tamanio);
void devolver_contexto_por_fs_write(char* nombre_instruccion, char* nombre_interfaz, char* nombre_archivo, int direccion_fisica, int tamanio, int puntero);
void devolver_contexto_por_fs_read(char* nombre_instruccion, char* nombre_interfaz, char* nombre_archivo, int direccion_fisica, int tamanio, int puntero);

void devolver_contexto_por_correcta_finalizacion();

void escribir_valor_a_registro(char* nombre_registro, u_int32_t valor);
uint32_t leer_valor_de_registro(char* nombre_registro);
int mmu(int direccion_logica);

#endif