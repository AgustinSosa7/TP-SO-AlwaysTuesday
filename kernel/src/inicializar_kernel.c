#include <../includes/inicializar_kernel.h>



void inicializar_kernel(){
    kernel_logger = inicializar_logs();
    kernel_config =  inicializar_configs();
}

t_log* inicializar_logs(void){
kernel_logger = log_create("kernel.log","KERNEL_LOG",1,LOG_LEVEL_INFO);
if (kernel_logger == NULL) {
	perror("Algo raro pasa con el log. No se pudo crear o encontrar el archivo.");
	exit(EXIT_FAILURE);
    }


kernel_log_debug = log_create("kernel.log","KERNEL_LOG",1,LOG_LEVEL_TRACE);
if (kernel_log_debug == NULL) {
	perror("Algo raro pasa con el log. No se pudo crear o encontrar el archivo.");
	exit(EXIT_FAILURE);
    }
}    
    
t_config* inicializar_configs(void) {

kernel_config = config_create("/home/utnso/Desktop/tp-2024-1c-AlwaysTuesday/kernel/kernel.config");
if (kernel_config == NULL) {
	perror("Error al intentar cargar el config.");
	exit(EXIT_FAILURE);
    } 

PUERTO_ESCUCHA = config_get_string_value(kernel_config,"PUERTO_ESCUCHA");
IP_MEMORIA = config_get_string_value(kernel_config,"IP_MEMORIA");
PUERTO_MEMORIA = config_get_string_value(kernel_config,"PUERTO_MEMORIA");
IP_CPU = config_get_string_value(kernel_config,"IP_CPU");
PUERTO_CPU_DISPATCH= config_get_int_value(kernel_config,"PUERTO_CPU_DISPATCH");
PUERTO_CPU_INTERRUPT = config_get_int_value(kernel_config,"PUERTO_CPU_INTERRUPT");
ALGORITMO_PLANIFICACION = config_get_string_value(kernel_config,"ALGORITMO_PLANIFICACION");
QUANTUM = config_get_int_value(kernel_config,"QUANTUM");
RECURSOS = config_get_array_value(kernel_config, "RECURSOS"); // REVISAR POR FAVOR EL TIPO DE DATO QUE SE MANEJA (kernel.h)
INSTANCIAS_RECURSOS = config_get_array_value(kernel_config, "INSTANCIAS_RECURSOS"); // REVISAR POR FAVOR EL TIPO DE DATO QUE SE MANEJA (kernel.h)
GRADO_MULTIPROGRAMACION = config_get_int_value(kernel_config,"GRADO_MULTIPROGRAMACION");

}