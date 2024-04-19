#include "../includes/cpu.h"


int main() {

    inicializar_cpu();


    // Conectarnos como cliente a MEMORIA

    fd_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);  //cerrar
    
    gestionar_handshake_como_cliente(fd_memoria, "MEMORIA", cpu_logger);
    
    log_info(cpu_logger, "Conexion con MEMORIA exitosa!!"); 

    // Iniciar server de CPU - DISPATCH

   // fd_cpu_dispatch = iniciar_servidor(PUERTO_ESCUCHA_DISPATCH, cpu_logger, "CPU - DISPATCH INICIADO!!");

    // Iniciar server de CPU - INTERRUPT

    // fd_cpu_interrupt= iniciar_servidor(PUERTO_ESCUCHA_INTERRUPT,cpu_logger, "CPU - INTERRUPT INICIADO!!");


    
    // Esperar al cliente KERNEL 
    log_info(cpu_logger, "Esperando a Kernel");

    // Esperar al cliente KERNEL en su puerto DISPATCH
    fd_kernel_dispatch = esperar_cliente(fd_cpu_dispatch,cpu_logger,"KERNEL - Dispatch"); //cerrar
    
    // realizarHandshakeServidor(fd_kernel_dispatch,CPU_DISPATCH);

    // Esperar al cliente KERNEL en su puerto INTERRUPT
    fd_kernel_interrupt = esperar_cliente(fd_cpu_interrupt, cpu_logger,"KERNEL - Interrupt"); //cerrar

    //Atender los mensajes de Kernel - Dispatch
    pthread_t hilo_kernel_dispatch;
    pthread_create(&hilo_kernel_dispatch, NULL, (void*)atender_cpu_kernel_dispatch,NULL);
    pthread_detach(hilo_kernel_dispatch);

    //Atender los mensajes de Kernel - Interrupt
    pthread_t hilo_kernel_interrupt;
    pthread_create(&hilo_kernel_interrupt,NULL,(void*)atender_cpu_kernel_interrupt,NULL);
    pthread_detach(hilo_kernel_interrupt);

    //Atender los mensajes de Memoria
    pthread_t hilo_memoria;
    pthread_create(&hilo_memoria, NULL, (void*)atender_cpu_memoria, NULL);
    pthread_join(hilo_memoria, NULL);


    return EXIT_SUCCESS;
}