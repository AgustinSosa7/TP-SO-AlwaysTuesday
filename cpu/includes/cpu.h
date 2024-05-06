#ifndef CPU_H_
#define CPU_H_

#include "cpu_gestor.h"
#include "inicializar_cpu.h"
#include "cpu_kernel_dispatch.h"
#include "cpu_kernel_interrupt.h"
#include "cpu_memoria.h"

typedef struct
{
    u_int32_t PC;
    u_int8_t AX;
    u_int8_t BX;
    u_int8_t CX;
    u_int8_t DX;
    u_int32_t EAX;
    u_int32_t EBX;
    u_int32_t ECX;
    u_int32_t EDX;
    u_int32_t SI;
    u_int32_t DI;
}t_cpu;

t_cpu* cpu;

t_cpu* cpu = crear_paquete(op_code)
//Variables GLOBALES

t_log* cpu_logger;
t_log* cpu_log_debug;
t_config* cpu_config;

int fd_cpu_dispatch;
int fd_cpu_interrupt;
int fd_kernel_dispatch;  
int fd_kernel_interrupt;
int fd_memoria;

char* IP_MEMORIA;
char* IP_CPU;
char* PUERTO_MEMORIA; 
char* PUERTO_ESCUCHA_DISPATCH;
char* PUERTO_ESCUCHA_INTERRUPT;
int CANTIDAD_ENTRADAS_TLB;
char* ALGORITMO_TLB;



#endif
