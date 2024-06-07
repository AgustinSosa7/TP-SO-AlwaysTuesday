#ifndef CREAR_CONEXIONES_H_
#define CREAR_CONEXIONES_H_

#include "k_gestor.h"
#include "kernel_entradaSalida.h"

// t_list* INSTRUCCIONES_GEN;
// t_list* INSTRUCCIONES_STDIN;
// t_list* INSTRUCCIONES_STDOUT;
// t_list* INSTRUCCIONES_FS;
// t_list* IOS_CONECTADOS;


// extern t_list* INSTRUCCIONES_GEN;
// extern t_list* INSTRUCCIONES_STDIN;
// extern t_list* INSTRUCCIONES_STDOUT;
// extern t_list* INSTRUCCIONES_FS;
// extern t_list* IOS_CONECTADOS;

 void conexion_kernel_memoria();
 void conexion_kernel_cpu_dispatch();
 void conexion_kernel_cpu_interrupt();
 void conexion_kernel_entradaSalida();

void gestionar_entrada_salida();
t_interfaz* identificar_io(int socket);
t_list* asignar_instrucciones_posibles(char* tipo);
void agregar_io(t_interfaz* interfaz);
void gestionar_procesos_io(t_interfaz* interfaz);
void recibir_fin_peticion();
void desbloquear_proceso(t_interfaz* interfaz);


#endif 
