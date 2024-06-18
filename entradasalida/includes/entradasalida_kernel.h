#ifndef ENTRADASALIDA_KERNEL_H_
#define ENTRADASALIDA_KERNEL_H

#include "es_gestor.h"
#include "entradasalida_memoria.h"



void identificarse_con_kernel(int socket, char* nombre_io);

void atender_entradasalida_kernel();

t_peticion* recibir_peticion(t_paquete* paquete);
void asignar_parametros_segun_tipo(t_peticion* peticion, t_buffer* buffer);
void procesar_peticion(t_peticion* peticion);
char* iniciar_la_consola(int registroTamanio);
bool validar_tamanio_leido(char* leido, int registroTamanio);

void finalizar_peticion(t_peticion* peticion);
void eliminar_peticion(t_peticion* peticion);
void eliminar_parametros_segun_instruccion(char* instruccion, t_peticion_param* parametros);

//FS
void crear_fcb();

#endif 