#include "../includes/cpu.h"


int main() {

    inicializar_cpu();


    // Conectarnos como cliente a MEMORIA
    log_info(cpu_logger, "Creando conexion con MEMORIA...");
    fd_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA, cpu_logger);  //cerrar
    gestionar_handshake_como_cliente(fd_memoria, "MEMORIA", cpu_logger);    
    log_warning(cpu_logger, "HANDSHAKE CON MEMORIA [EXITOSO]"); 


    // Conectarnos como cliente a MEMORIA

    log_info(cpu_logger, "Esperando a Kernel");

    // Iniciar server de CPU - DISPATCH

    fd_cpu_dispatch = iniciar_servidor(PUERTO_ESCUCHA_DISPATCH, cpu_logger, IP_CPU);

    log_info(cpu_logger, "Esperando a DISPATCH...");
    fd_kernel_dispatch = esperar_cliente(fd_cpu_dispatch, cpu_logger,"DISPATCH");
    saludar_cliente(&fd_kernel_dispatch, cpu_logger);

    // Iniciar server de CPU - INTERRUPT

    fd_cpu_interrupt= iniciar_servidor(PUERTO_ESCUCHA_INTERRUPT,cpu_logger, IP_CPU);

    log_info(cpu_logger, "Esperando a INTERRUPT...");
    fd_kernel_interrupt = esperar_cliente(fd_cpu_interrupt, cpu_logger,"INTERRUPT");
    saludar_cliente(&fd_kernel_interrupt, cpu_logger);

    
    //Atender los mensajes de Kernel - Dispatch
    //pthread_t hilo_kernel_dispatch;
    //pthread_create(&hilo_kernel_dispatch, NULL, (void*)atender_cpu_kernel_dispatch,NULL);
    //pthread_detach(hilo_kernel_dispatch);
//
    ////Atender los mensajes de Kernel - Interrupt
    //pthread_t hilo_kernel_interrupt;
    //pthread_create(&hilo_kernel_interrupt,NULL,(void*)atender_cpu_kernel_interrupt,NULL);
    //pthread_detach(hilo_kernel_interrupt);
//
    ////Atender los mensajes de Memoria
    //pthread_t hilo_memoria;
    //pthread_create(&hilo_memoria, NULL, (void*)atender_cpu_memoria, NULL);
    //pthread_join(hilo_memoria, NULL);
//

    return EXIT_SUCCESS;
}