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
    gestionar_handshake_como_cliente(fd_cpu_dispatch, "CPU", kernel_logger);
    log_warning(kernel_logger, "HANDSHAKE DISPATCH CON CPU [EXITOSO]"); 

    // INTERRUPT
    fd_cpu_interrupt = crear_conexion(IP_CPU, PUERTO_CPU_INTERRUPT);
    gestionar_handshake_como_cliente(fd_cpu_interrupt, "CPU", kernel_logger);
    log_warning(cpu_logger, "HANDSHAKE INTERRUPT CON CPU [EXITOSO]"); 

    // INICIAR EL SERVIDOR PARA ENTRADA Y SALIDA.
    fd_kernel = iniciar_servidor(PUERTO_ESCUCHA, kernel_logger, IP_KERNEL);

    log_info(kernel_logger, "Esperando a Entradasalida...");
    fd_entradasalida = esperar_cliente(fd_kernel, kernel_logger, "ENTRADASALIDA");
    saludar_cliente(&fd_entradasalida);
    // Atender los mensajes de Entrada y salida

    return EXIT_SUCCESS;
}
