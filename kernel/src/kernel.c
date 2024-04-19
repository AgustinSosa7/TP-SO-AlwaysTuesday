#include <stdlib.h>
#include <stdio.h>
#include <../includes/kernel.h>

int main()
{
    // INICIAR KERNEL

    inicializar_kernel();

    // Conectarse con Memoria
    fd_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);
    gestionar_handshake_como_cliente(fd_memoria, "MEMORIA", kernel_logger);

    log_warning(kernel_logger, "HANDSHAKE CON MEMORIA [EXITOSO]");

    // Conectarse con CPU
    //  DISPATCH
    fd_cpu_dispatch = crear_conexion(IP_CPU, PUERTO_CPU_DISPATCH);
    log_info(kernel_logger, "Conexion con CPU DISPATCH exitosa.");

    // INTERRUPT
    fd_cpu_interrupt = crear_conexion(IP_CPU, PUERTO_CPU_INTERRUPT);
    log_info(kernel_logger, "Conexion con CPU INTERRUPT exitosa.");

    // INICIAR EL SERVIDOR PARA ENTRADA Y SALIDA.
    fd_kernel = iniciar_servidor(PUERTO_ESCUCHA, kernel_logger, "KERNEL INICIADO!");

    log_info(kernel_logger, "Esperando a Entradasalida...");
    fd_entradasalida = esperar_cliente(fd_kernel, kernel_logger, "ENTRADASALIDA");

    // Atender los mensajes de Entrada y salida

    return EXIT_SUCCESS;
}
