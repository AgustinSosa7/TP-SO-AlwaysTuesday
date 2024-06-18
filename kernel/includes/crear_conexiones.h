#ifndef CREAR_CONEXIONES_H_
#define CREAR_CONEXIONES_H_

#include "k_gestor.h"
#include "kernel_entradaSalida.h"


 void conexion_kernel_memoria();
 void conexion_kernel_cpu_dispatch();
 void crear_conexion_kernel_cpu_dispatch();
 void conexion_kernel_cpu_interrupt();
 void crear_conexion_kernel_cpu_interrupt();
 void conexion_kernel_entradaSalida();

void gestionar_entrada_salida();
t_interfaz* identificar_io(int socket);
t_list* asignar_instrucciones_posibles(char* tipo);
void agregar_io(t_interfaz* interfaz);
void gestionar_procesos_io(t_interfaz* interfaz);

#endif 
