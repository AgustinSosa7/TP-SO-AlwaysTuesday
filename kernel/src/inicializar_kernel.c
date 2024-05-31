#include <../includes/inicializar_kernel.h>



void inicializar_kernel(char* path){
    inicializar_logs();
    inicializar_configs(path);
	inicializar_listas();
	inicializar_semaforos();
	inicializar_pid_y_procesos_activos();
}

void inicializar_logs(void){

kernel_logger = log_create("kernel.log","KERNEL_LOG",1,LOG_LEVEL_INFO);

if (kernel_logger == NULL) {
	perror("Algo raro pasa con el log. No se pudo crear o encontrar el archivo.");
	exit(EXIT_FAILURE);
    }

log_info(kernel_logger, "Se inicializo el kernel logger"); //Sacar eventualmente


kernel_log_debug = log_create("kernel_debug.log","KERNEL_DEBUG_LOG",1,LOG_LEVEL_INFO);
if (kernel_log_debug == NULL) {
	perror("Algo raro pasa con el log. No se pudo crear o encontrar el archivo.");
	exit(EXIT_FAILURE);
   }

log_info(kernel_log_debug, "Se inicializo el kernel debug logger"); //Sacar eventualmente

}    
    
void inicializar_configs(char* path) {

kernel_config = config_create(path);
if (kernel_config == NULL) {
	perror("Error al intentar cargar el config.");
	exit(EXIT_FAILURE);
    } 

PUERTO_ESCUCHA = config_get_string_value(kernel_config,"PUERTO_ESCUCHA");
IP_MEMORIA = config_get_string_value(kernel_config,"IP_MEMORIA");
IP_KERNEL = config_get_string_value(kernel_config,"IP_MEMORIA");
PUERTO_MEMORIA = config_get_string_value(kernel_config,"PUERTO_MEMORIA");
IP_CPU = config_get_string_value(kernel_config,"IP_CPU");
PUERTO_CPU_DISPATCH= config_get_string_value(kernel_config,"PUERTO_CPU_DISPATCH");
PUERTO_CPU_INTERRUPT = config_get_string_value(kernel_config,"PUERTO_CPU_INTERRUPT");
ALGORITMO_PLANIFICACION = config_get_string_value(kernel_config,"ALGORITMO_PLANIFICACION");
QUANTUM = config_get_int_value(kernel_config,"QUANTUM");
RECURSOS = config_get_array_value(kernel_config, "RECURSOS"); // REVISAR POR FAVOR EL TIPO DE DATO QUE SE MANEJA (kernel.h)
INSTANCIAS_RECURSOS = config_get_array_value(kernel_config, "INSTANCIAS_RECURSOS"); // REVISAR POR FAVOR EL TIPO DE DATO QUE SE MANEJA (kernel.h)
GRADO_MULTIPROGRAMACION = config_get_int_value(kernel_config,"GRADO_MULTIPROGRAMACION");

log_info(kernel_log_debug, "Se inicializan las configs..."); //Sacar eventualmente

log_info(kernel_log_debug, "PUERTO_ESCUCHA: %s", PUERTO_ESCUCHA); //Sacar eventualmente
log_info(kernel_log_debug, "IP_MEMORIA: %s", IP_MEMORIA); //Sacar eventualmente
log_info(kernel_log_debug, "PUERTO_MEMORIA: %s", PUERTO_MEMORIA); //Sacar eventualmente
log_info(kernel_log_debug, "IP_CPU: %s", IP_CPU); //Sacar eventualmente
log_info(kernel_log_debug, "PUERTO_CPU_DISPATCH: %s", PUERTO_CPU_DISPATCH); //Sacar eventualmente
log_info(kernel_log_debug, "PUERTO_CPU_INTERRUPT: %s", PUERTO_CPU_INTERRUPT); //Sacar eventualmente
//log_info(kernel_log_debug, "ALGORITMO_PLANIFICACION: %s", ALGORITMO_PLANIFICACION); //Sacar eventualmente
//log_info(kernel_log_debug, "QUANTUM: %d", QUANTUM); //Sacar eventualmente
//log_info(kernel_log_debug, "Falta RECURSOS:" %d); //Sacar eventualmente
//log_info(kernel_log_debug, "Falta INSTANCIAS_RECURSOS: %d", INSTANCIAS_RECURSOS); //Sacar eventualmente
//log_info(kernel_log_debug, "GRADO_MULTIPROGRAMACION: %d", GRADO_MULTIPROGRAMACION); //Sacar eventualmente

log_info(kernel_log_debug, "Se inicializaron las configs"); //Sacar eventualmente

}

void inicializar_listas(void){
	lista_new = list_create();
	lista_ready = list_create();
	lista_exec = list_create();
	//lista_blocked = list_create();
	lista_exit = list_create();
	lista_ready_plus = list_create();
}

void inicializar_semaforos(void){
	mutex_pid = 1;
}

void inicializar_pid_y_procesos_activos(){
	pid_global = 0;
	procesos_activos = 0;
	}