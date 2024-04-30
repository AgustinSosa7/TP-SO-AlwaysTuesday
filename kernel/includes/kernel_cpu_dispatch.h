#ifndef KERNEL_CPU_DISPATCH_H_
#define KERNEL_CPU_DISPATCH_H_

#include <../src/utils/utils_connections.h>
#include "k_gestor.h"


//typedef struct{
//    int AX;
//    int BX;
//    int CX;
//    int DX;
//}t_registros;
//typedef struct{
//    int ticket;
//    int program_counter;
//    t_registros registros_CPU;
//    int pid;
//}t_pcb;

typedef struct{
    uint32_t tiempo_espera;
    char* archivo;
    char* registro1;
    char* registro2;
    char* registro3; 
} t_peticion_param;
typedef struct{
    char* instruccion;
    char* interfaz;
    t_peticion_param* parametros;
} t_peticion;

void atender_kernel_cpu_dispatch();
t_peticion* recibir_peticion(t_paquete* paquete);
t_peticion_param* leer_parametros(t_paquete* paquete, char* instruccion);
void enviar_peticion_a_interfaz(t_peticion* peticion);
void eliminar_peticion(t_peticion* peticion);
//void _enviar_pcb_a_CPU_por_dispatch(t_pcb* una_pcb);
#endif 