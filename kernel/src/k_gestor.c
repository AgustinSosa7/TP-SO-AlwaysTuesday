#include "../includes/k_gestor.h"

t_list* INSTRUCCIONES_GEN;
t_list* INSTRUCCIONES_STDIN;
t_list* INSTRUCCIONES_STDOUT;
t_list* INSTRUCCIONES_FS;
t_list* IOS_CONECTADOS;

t_list* lista_instrucciones;

t_list* lista_new;
t_list* lista_ready;
t_list* lista_ready_plus;
t_list* lista_exec;
t_list* lista_exit;

t_list* lista_recursos;

////////////////SEMAFOROS/////////////////////////////////////////
pthread_mutex_t mutex_pid;
pthread_mutex_t mutex_new;
pthread_mutex_t mutex_ready;
pthread_mutex_t mutex_exec;
pthread_mutex_t mutex_ready_plus;
pthread_mutex_t mutex_exit;
pthread_mutex_t mutex_io;
pthread_mutex_t mutex_detener_planificacion;

sem_t sem_grado_multiprogram;
sem_t sem_new_a_ready;
sem_t sem_planificador_corto_plazo;
sem_t sem_detener_planificacion;
bool flag_detener_planificacion = false;

void enviar_interrupción_a_cpu(op_code tipo_interrupción){
    t_paquete* un_paquete = crear_paquete(tipo_interrupción);
    // no tiene nada en el buffer.
	enviar_paquete(un_paquete,fd_cpu_interrupt);
    eliminar_paquete(un_paquete);
} 
void eliminar_proceso(int pid){
    //solicitar a la memoria que libere todas las estructuras asociadas 
    //liberar recursos
    //enviar mensaje a lucas(OO CODE FIN DE PROCESO,pid);
    //al proceso y marque como libre todo el espacio que este ocupaba.
    
    sem_post(&sem_grado_multiprogram);
}

void detener_planificacion(){
    if(flag_detener_planificacion){
        sem_wait(&sem_detener_planificacion);
    }
}
