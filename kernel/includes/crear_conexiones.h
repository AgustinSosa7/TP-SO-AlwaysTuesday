#ifndef CREAR_CONEXIONES_H
#define CREAR_CONEXIONES_H

#include <../src/utils/utils_connections.h>
#include "k_gestor.h"



 void conexion_kernel_memoria();
 void conexion_kernel_cpu_dispatch();
 void conexion_kernel_cpu_interrupt();
 void conexion_kernel_entradaSalida();

void gestionar_entrada_salida();
t_interfaz* identificar_io(int socket);
void agregar_io(t_interfaz* interfaz);




#endif 
