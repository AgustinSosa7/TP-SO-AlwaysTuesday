#include "../includes/cpu.h"


int main() {

    // Inicializar CPU
    inicializar_cpu();

    // Conectarnos como cliente a MEMORIA
    gestionar_conexion_como_cliente(IP_MEMORIA, PUERTO_MEMORIA, cpu_logger, "MEMORIA");

    // Iniciar server de CPU - DISPATCH
    gestionar_conexion_como_server(IP_CPU, PUERTO_ESCUCHA_DISPATCH, cpu_logger, "DISPATCH");

    // Iniciar server de CPU - INTERRUPT
    gestionar_conexion_como_server(IP_CPU, PUERTO_ESCUCHA_INTERRUPT, cpu_logger, "INTERRUPT");

    
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