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
void devolver_contexto_por_stdin_read(char* nombre_instruccion, char* nombre_interfaz, t_list* lista_de_accesos);
void devolver_contexto_por_stdout_write(char* nombre_instruccion, char* nombre_interfaz, t_list* lista_de_accesos);
void devolver_contexto_por_fs_create(char* nombre_instruccion, char* nombre_interfaz, char* nombre_archivo);
void devolver_contexto_por_fs_delete(char* nombre_instruccion, char* nombre_interfaz, char* nombre_archivo);
void devolver_contexto_por_fs_truncate(char* nombre_instruccion, char* nombre_interfaz, char* nombre_archivo, int tamanio);
void devolver_contexto_por_fs_write(char* nombre_instruccion, char* nombre_interfaz, char* nombre_archivo, int direccion_fisica, int tamanio, int puntero);
void devolver_contexto_por_fs_read(char* nombre_instruccion, char* nombre_interfaz, char* nombre_archivo, int direccion_fisica, int tamanio, int puntero);

void devolver_contexto_por_correcta_finalizacion();

// Operaciones con Registros:
void escribir_valor_a_registro(char* nombre_registro, u_int32_t valor);
uint32_t leer_valor_de_registro(char* nombre_registro);
int tamanio_del_registro(char* nombre_registro);

// MMU:
t_direccion_a_operar* mmu(int direccion_logica);
int calcular_cantidad_de_accesos(int direccion_logica_inicial,int bytes_a_operar);
void* gestionar_lectura_memoria(int direccion_logica,int cant_bytes_a_operar);
void gestionar_escritura_memoria(int direccion_logica,int cant_bytes_a_operar,void* a_escribir);

t_list* gestionar_accesos_para_io(int direccion_logica, int bytes_a_operar);
void agregar_acceso_a_lista(t_list* listado_accesos, int direccion, int bytes);

void agregar_acceso_a_paquete(t_paquete* paquete, t_direccion_a_operar* acceso);
void agregar_lista_de_accesos_a_paquete(t_paquete* paquete, t_list* lista_de_accesos);

#endif