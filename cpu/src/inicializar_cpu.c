#include "../includes/inicializar_cpu.h"

void inicializar_cpu(char* path){
    inicializar_logs();
    iniciar_config(path);
    imprimir_config();
    inicializar_estructuras();
    inicializar_semaforos();
}

void inicializar_logs(){
    cpu_logger = log_create("cpu.log","CPU_LOG",1,LOG_LEVEL_INFO);
    if (cpu_logger == NULL) {
	    perror("Algo raro pasa con el log. No se pudo crear o encontrar el archivo.");
	    exit(EXIT_FAILURE);
	}

    cpu_log_debug = log_create("cpu_debug.log","CPU_LOG",1,LOG_LEVEL_INFO);
    if (cpu_log_debug == NULL) {
	    perror("Algo raro pasa con el log. No se pudo crear o encontrar el archivo.");
	    exit(EXIT_FAILURE);
	}
}

void iniciar_config(char* path){
	char* PATH = string_new();

    string_append(&PATH, "/home/utnso/tp-2024-1c-AlwaysTuesday/cpu/configs/");
	string_append(&PATH, path);
	string_append(&PATH, ".config");

	cpu_config = config_create(PATH);

    free(PATH);

    if (cpu_config == NULL) {
        perror("Error al intentar cargar el config.");
        exit(EXIT_FAILURE);
    }

    IP_MEMORIA = config_get_string_value(cpu_config,"IP_MEMORIA");
    IP_CPU = config_get_string_value(cpu_config, "IP_CPU"); 
    PUERTO_MEMORIA = config_get_string_value(cpu_config,"PUERTO_MEMORIA");
    PUERTO_ESCUCHA_DISPATCH = config_get_string_value(cpu_config,"PUERTO_ESCUCHA_DISPATCH");
    PUERTO_ESCUCHA_INTERRUPT = config_get_string_value(cpu_config,"PUERTO_ESCUCHA_INTERRUPT");
    CANTIDAD_ENTRADAS_TLB = config_get_int_value(cpu_config,"CANTIDAD_ENTRADAS_TLB");
    ALGORITMO_TLB = config_get_string_value(cpu_config,"ALGORITMO_TLB");    
}

void imprimir_config(){
    log_info(cpu_log_debug, "IP_MEMORIA: %s", IP_MEMORIA);
    log_info(cpu_log_debug, "PUERTO_MEMORIA: %s", PUERTO_MEMORIA);
    log_info(cpu_log_debug, "PUERTO_ESCUCHA_DISPATCH: %s", PUERTO_ESCUCHA_DISPATCH);
    log_info(cpu_log_debug,"PUERTO_ESCUCHA_INTERRUPT: %s", PUERTO_ESCUCHA_INTERRUPT);
}

void inicializar_estructuras(){
    //PCB
    pcb_global = malloc(sizeof(t_pcb));
    pcb_global->registros_cpu = malloc(sizeof(t_registros_cpu));

    // Info de la Memoria
    tamanio_pagina = -1;
    tamanio_memoria = -1;

    // Flags del CPU
    dejar_de_ejecutar = false;
    ocurrio_interrupcion = false;
    
    // Motivo de Interrupción
    motivo_interrupcion = -1;

    // TLB
    tlb = list_create();
}

void inicializar_semaforos(){
    sem_init(&sem_ciclo_de_instruccion,false,0);
    pthread_mutex_init(&mutex_ocurrio_interrupcion, NULL);
}
