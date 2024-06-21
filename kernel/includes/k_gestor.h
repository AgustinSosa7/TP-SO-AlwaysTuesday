#ifndef K_GESTOR_H_
#define K_GESTOR_H_



#include <../src/utils/utils_connections.h>


///////////////////////ESTRUCTURAS PCB////////////////////////////

typedef struct 
{
    char* nombre;
    char* tipo;
    t_list* instrucciones_posibles;
    int fd_interfaz;
    bool esta_conectada;
    t_queue* cola_procesos_blocked;
    sem_t semaforo_cola_procesos_blocked;
    pthread_mutex_t mutex_cola_blocked;
} t_interfaz;

typedef struct{
    int tiempo_espera;
    char* archivo;
    char* registroDireccion;
    int registroTamanio;
    char* registroPunteroArchivo;
} t_peticion_param;

typedef struct{
    char* instruccion;
    char* interfaz;
    t_peticion_param* parametros;
} t_peticion;

typedef struct{
    t_peticion* peticion;
    t_pcb* un_pcb;
    t_interfaz* interfaz;
}t_peticion_pcb_interfaz;


///////////////////////ESTRUCTURAS PCB////////////////////////////

typedef struct{
        t_pcb* un_pcb;
        t_peticion* peticion;
}t_proceso_blocked;

extern int pid_global;
extern int tiempo_transcurrido;

extern t_list* lista_instrucciones;


extern t_list* INSTRUCCIONES_GEN;
extern t_list* INSTRUCCIONES_STDIN;
extern t_list* INSTRUCCIONES_STDOUT;
extern t_list* INSTRUCCIONES_FS;
extern t_list* IOS_CONECTADOS;


extern t_list* lista_new;
extern t_list* lista_ready;
extern t_list* lista_ready_plus;
extern t_list* lista_exec;
extern t_list* lista_exit;

extern t_list* lista_recursos;
//////////////////////SEMAFOROS/////////////////////////////////////////

extern pthread_mutex_t mutex_pid;
extern pthread_mutex_t mutex_new;
extern pthread_mutex_t mutex_ready;
extern pthread_mutex_t mutex_exec;
extern pthread_mutex_t mutex_ready_plus;
extern pthread_mutex_t mutex_exit;
extern pthread_mutex_t mutex_io;
extern pthread_mutex_t mutex_detener_planificacion;

extern sem_t sem_grado_multiprogram;
extern sem_t sem_new_a_ready;
extern sem_t sem_planificador_corto_plazo;
extern sem_t sem_detener_planificacion;
////////////////////RECURSOS////////////////////////////////
typedef struct{
    char* nombre_recurso;
    int instancias;
    t_list* lista_procesos_bloqueados;
    t_list* lista_procesos_asignados;
    //sem_t* sem_tipo_recurso; ver después
} t_recursos;
///////////////////////////////////////////////////////////////
extern t_log* kernel_logger;
extern t_log* kernel_log_debug;
extern t_config* kernel_config;

extern int fd_kernel;
extern int fd_memoria;
extern int fd_entradasalida;
extern int fd_cpu_dispatch;
extern int fd_cpu_interrupt;

extern int pid_global;
extern bool flag_detener_planificacion;

extern char* PUERTO_ESCUCHA;
extern char* IP_MEMORIA;
extern char* PUERTO_MEMORIA; //Quizas en crear_concxion() los toma como char* a los int
extern char* IP_CPU;
extern char* PUERTO_CPU_DISPATCH;
extern char* PUERTO_CPU_INTERRUPT;
extern char* ALGORITMO_PLANIFICACION;
extern int QUANTUM;
extern char** RECURSOS;    // REVISAR EL TIPO EN EL ENUNCIADO 
extern char ** INSTANCIAS_RECURSOS; // POR FAVOR REVISAR EL TIPO EN EL ENUNCIADO
extern int GRADO_MULTIPROGRAMACION;
extern char * IP_KERNEL;

void enviar_interrupción_a_cpu(op_code interrupción,int motivo);
void eliminar_proceso(t_pcb* un_pcb, motivo_fin_de_proceso motivo);
void detener_planificacion();
t_pcb* buscar_pcb(int pid);
bool encontre_el_pcb(t_pcb* pcb, int pid);
t_pcb* buscar_pcb_en_bloqueados(int pid);
t_list* buscar_lista_de_recursos_pcb(t_pcb* pcb);
void liberar_recursos(t_pcb* pcb);
void liberar_estructuras_en_memoria(op_code code_op ,int pid);
#endif