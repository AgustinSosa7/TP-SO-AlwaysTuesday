#include "../includes/cpu_gestor.h"

t_log* cpu_logger;
t_log* cpu_log_debug;
t_config* cpu_config;

t_pcb* pcb_global;

t_list* lista_registros_prop_gen;
t_list* lista_registros_extendidos;

////////////////////////////////////SEMAFOROS /////////////////////////////////
sem_t sem_ciclo_de_instruccion;

char ** opcode_cpu;

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
