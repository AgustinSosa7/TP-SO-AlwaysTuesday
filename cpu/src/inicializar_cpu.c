#include "../includes/inicializar_cpu.h"


void inicializar_cpu(){
    inicializar_logs();
    iniciar_config();
    imprimir_config();

}

void inicializar_logs(){
    cpu_logger = log_create("cpu.log","CPU_LOG",1,LOG_LEVEL_INFO);
    if (cpu_logger == NULL) {
	    perror("Algo raro pasa con el log. No se pudo crear o encontrar el archivo.");
	    exit(EXIT_FAILURE);
	}


    cpu_log_debug = log_create("cpu.log","CPU_LOG",1,LOG_LEVEL_TRACE);
    if (cpu_log_debug == NULL) {
	    perror("Algo raro pasa con el log. No se pudo crear o encontrar el archivo.");
	    exit(EXIT_FAILURE);
	}
}

void iniciar_config(){
    cpu_config = config_create("/home/utnso/Desktop/tp-2024-1c-AlwaysTuesday/cpu/cpu.config");
    if (cpu_config == NULL) {
        perror("Error al intentar cargar el config.");
        exit(EXIT_FAILURE);
    }

    IP_MEMORIA = config_get_string_value(cpu_config,"IP_MEMORIA");
    PUERTO_MEMORIA = config_get_int_value(cpu_config,"PUERTO_MEMORIA");
    PUERTO_ESCUCHA_DISPATCH = config_get_int_value(cpu_config,"PUERTO_ESCUCHA_DISPATCH");
    PUERTO_ESCUCHA_INTERRUPT = config_get_int_value(cpu_config,"PUERTO_ESCUCHA_INTERRUPT");
    CANTIDAD_ENTRADAS_TLB = config_get_int_value(cpu_config,"CANTIDAD_ENTRADAS_TLB");
    ALGORITMO_TLB = config_get_string_value(cpu_config,"ALGORITMO_TLB");    

}

void imprimir_config(){
    log_info(cpu_logger, "IP_MEMORIA: %s", IP_MEMORIA);
    log_warning(cpu_log_debug, "PUERTO_MEMORIA: %d", PUERTO_MEMORIA);
    log_debug(cpu_log_debug, "PUERTO_ESCUCHA_DISPATCH: %d", PUERTO_ESCUCHA_DISPATCH);
    log_trace(cpu_log_debug,"PUERTO_ESCUCHA_INTERRUPT: %d", PUERTO_ESCUCHA_INTERRUPT);
}