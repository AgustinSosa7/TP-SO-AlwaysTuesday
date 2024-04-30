#include <stdlib.h>
#include <stdio.h>
#include <../includes/kernel.h>

int main()
{
    // Inicializar KERNEL
    inicializar_kernel();

        // Conectarse con Memoria
    log_info(kernel_logger, "Creando conexion con MEMORIA...");
    fd_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA, kernel_logger);
    gestionar_handshake_como_cliente(fd_memoria, "MEMORIA", kernel_logger);

    // Conectarse con CPU
    log_info(kernel_logger, "Creando conexion con CPU...");
    //  DISPATCH
    fd_cpu_dispatch = crear_conexion(IP_CPU, PUERTO_CPU_DISPATCH, kernel_logger);
    gestionar_handshake_como_cliente(fd_cpu_dispatch, "CPU", kernel_logger);

    // INTERRUPT
    fd_cpu_interrupt = crear_conexion(IP_CPU, PUERTO_CPU_INTERRUPT, kernel_logger);
    gestionar_handshake_como_cliente(fd_cpu_interrupt, "CPU", kernel_logger);

    // INICIAR EL SERVIDOR PARA ENTRADA Y SALIDA.
    fd_kernel = iniciar_servidor(PUERTO_ESCUCHA, kernel_logger, IP_KERNEL);

    log_info(kernel_logger, "Esperando a Entradasalida...");
    fd_entradasalida = esperar_cliente(fd_kernel, kernel_logger, "ENTRADASALIDA");
    gestionar_handshake_como_server(fd_entradasalida, kernel_logger, "ENTRADA SALIDA");

    
/////////////////////////--ATENDER-MENSAJES--//////////////////////

    //Atender los mensajes de MEMORIA
    pthread_t hilo_memoria;
    pthread_create(&hilo_memoria, NULL, (void*) atender_kernel_memoria, NULL);
    pthread_join(hilo_memoria, NULL);

    //Atender los mensajes de CPU - DISPATCH
    pthread_t hilo_cpu_dispatch;
    pthread_create(&hilo_cpu_dispatch, NULL, (void*)atender_kernel_cpu_dispatch, NULL);
    pthread_detach(hilo_cpu_dispatch);

   //Atender los mensajes de CPU - INTERRUPT
    pthread_t hilo_cpu_interrupt;
    pthread_create(&hilo_cpu_interrupt, NULL, (void*)atender_kernel_cpu_interrupt, NULL);
    pthread_detach(hilo_cpu_interrupt);

   //Atender los mensajes de ENTRADA SALIDA
    pthread_t hilo_entradaSalida;
    pthread_create(&hilo_entradaSalida, NULL, (void*)atender_kernel_entradaSalida, NULL);
    pthread_detach(hilo_entradaSalida);

    return EXIT_SUCCESS;
}


