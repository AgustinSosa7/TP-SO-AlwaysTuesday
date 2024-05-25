#ifndef CPU_H_
#define CPU_H_

#include <../src/utils/utils_connections.h>
#include "cpu_gestor.h"
#include "inicializar_cpu.h"
#include "cpu_kernel_dispatch.h"
#include "cpu_kernel_interrupt.h"
#include "cpu_memoria.h"


//moví la declaración del struc t_pcb a utils connections :D
t_cpu* cpu;

//t_cpu* cpu = crear_paquete(op_code);   Fijarse como sería :D 
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

semaforo_t semaforo_control_fetch_decode;
semaforo_t semaforo_control_decode_execute;


void iniciar_semaforo();
void comenzar_ciclo_instruccion();
void ciclo_instruccion_fetch();
void ciclo_instruccion_decode();
bool preguntando_si_hay_interrupciones_vigentes();

void ciclo_instruccion_execute();
bool validador_de_header(char* header_string);
uint32_t* detectar_registro(char* RX);
t_paquete* alistar_paquete_de_desalojo(op_code code_op);
void enviarPaqueteKernel(char* motivo);
void enviarPaqueteKernelConInfoExtra(t_paquete* infoExtra);


#endif
