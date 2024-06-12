#include "../includes/cpu.h"


int main(int argc, char** argv){

   if(validar_parametros_incorrectos_main(argc, 2, argv)){
        return EXIT_FAILURE;
    }
    
    // Inicializar CPU
    inicializar_cpu(argv[1]);
    
    // Conectarnos como cliente a MEMORIA
    log_info(cpu_logger, "Creando conexion con MEMORIA...");
    fd_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA, cpu_logger);  
    gestionar_handshake_como_cliente(fd_memoria, "MEMORIA", cpu_logger);    

    // Iniciar server de CPU - DISPATCH
    pthread_t hilo_conexion_dispatch;
    pthread_create(&hilo_conexion_dispatch, NULL, (void*)conexion_cpu_kernel_dispatch, NULL);
    pthread_detach(hilo_conexion_dispatch); 

    // Iniciar server de CPU - INTERRUPT
    pthread_t hilo_conexion_interrupt;
    pthread_create(&hilo_conexion_interrupt, NULL, (void*)conexion_cpu_kernel_interrupt, NULL);
    pthread_join(hilo_conexion_interrupt, NULL);

    // Atender los mensajes de Kernel - Dispatch
    pthread_t hilo_kernel_dispatch;
    pthread_create(&hilo_kernel_dispatch, NULL, (void*)atender_cpu_kernel_dispatch, NULL);
    pthread_detach(hilo_kernel_dispatch);    

    // Atender los mensajes de Kernel - Interrupt
    pthread_t hilo_kernel_interrupt;
    pthread_create(&hilo_kernel_interrupt, NULL, (void*)atender_interrupciones, NULL);
    pthread_join(hilo_kernel_interrupt, NULL);

    // Método principal
    pthread_t hilo_ciclo_instruccion;
    pthread_create(&hilo_ciclo_instruccion, NULL, (void*)ciclo_instruccion, NULL);
    pthread_detach(hilo_ciclo_instruccion);

    return EXIT_SUCCESS;
}

void conexion_cpu_kernel_dispatch(){
    fd_cpu_dispatch = iniciar_servidor(PUERTO_ESCUCHA_DISPATCH, cpu_logger, IP_CPU);
    log_info(cpu_logger, "Esperando a DISPATCH...");
    fd_kernel_dispatch = esperar_cliente(fd_cpu_dispatch, cpu_logger, "KERNEL-DISPATCH");
    gestionar_handshake_como_server(fd_kernel_dispatch, cpu_logger, "KERNEL-DISPATCH");
}

void conexion_cpu_kernel_interrupt(){
    fd_cpu_interrupt= iniciar_servidor(PUERTO_ESCUCHA_INTERRUPT, cpu_logger, IP_CPU);
    log_info(cpu_logger, "Esperando a INTERRUPT...");
    fd_kernel_interrupt = esperar_cliente(fd_cpu_interrupt, cpu_logger, "KERNEL-INTERRUPT");
    gestionar_handshake_como_server(fd_kernel_interrupt, cpu_logger, "KERNEL-INTERRUPT");
}  