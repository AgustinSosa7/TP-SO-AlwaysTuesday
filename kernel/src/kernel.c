#include <stdlib.h>
#include <stdio.h>
#include <../includes/kernel.h>

int main()
{
    // INICIAR KERNEL

    inicializar_kernel();

    // Conectarse con Memoria
    gestionar_conexion_como_cliente(IP_MEMORIA, PUERTO_MEMORIA, kernel_logger, "MEMORIA");

    //log_info(kernel_logger, "Creando conexion con MEMORIA...");
    //fd_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA, kernel_logger);
    //gestionar_handshake_como_cliente(fd_memoria, "MEMORIA", kernel_logger);
    //log_warning(kernel_logger, "HANDSHAKE CON MEMORIA [EXITOSO]");

    // Conectarse con CPU
    //  DISPATCH
    gestionar_conexion_como_cliente(IP_CPU, PUERTO_CPU_DISPATCH, kernel_logger, "CPU-DISPATCH");

    //fd_cpu_dispatch = crear_conexion(IP_CPU, PUERTO_CPU_DISPATCH, kernel_logger);
    //gestionar_handshake_como_cliente(fd_cpu_dispatch, "CPU", kernel_logger);
    //log_warning(kernel_logger, "HANDSHAKE DISPATCH CON CPU [EXITOSO]"); 

    // INTERRUPT
    gestionar_conexion_como_cliente(IP_CPU, PUERTO_CPU_INTERRUPT, kernel_logger, "CPU-INTERRUPT");

    //fd_cpu_interrupt = crear_conexion(IP_CPU, PUERTO_CPU_INTERRUPT, kernel_logger);
    //gestionar_handshake_como_cliente(fd_cpu_interrupt, "CPU", kernel_logger);
    //log_warning(kernel_logger, "HANDSHAKE INTERRUPT CON CPU [EXITOSO]"); 

    // INICIAR EL SERVIDOR PARA ENTRADA Y SALIDA.
    
    gestionar_conexion_como_server(IP_KERNEL, PUERTO_ESCUCHA, kernel_logger, "ENTRADASALIDA");

    //fd_kernel = iniciar_servidor(PUERTO_ESCUCHA, kernel_logger, IP_KERNEL);
    //log_info(kernel_logger, "Esperando a Entradasalida...");
    //fd_entradasalida = esperar_cliente(fd_kernel, kernel_logger, "ENTRADASALIDA");
    //gestionar_handshake_como_server(fd_entradasalida, kernel_logger);
//
    return EXIT_SUCCESS;
}
