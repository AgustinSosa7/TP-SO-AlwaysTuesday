#include <stdlib.h>
#include <stdio.h>
#include <../includes/kernel.h>

int main()
{
    // Inicializar KERNEL
    inicializar_kernel();

    // Conectarse con Memoria
    gestionar_conexion_como_cliente(IP_MEMORIA, PUERTO_MEMORIA, kernel_logger, "MEMORIA");

    // Conectarse con CPU - DISPATCH
    gestionar_conexion_como_cliente(IP_CPU, PUERTO_CPU_DISPATCH, kernel_logger, "CPU-DISPATCH");

    // Conectarse con CPU- INTERRUPT
    gestionar_conexion_como_cliente(IP_CPU, PUERTO_CPU_INTERRUPT, kernel_logger, "CPU-INTERRUPT");

    // INICIAR EL SERVIDOR PARA ENTRADA Y SALIDA.
    gestionar_conexion_como_server(IP_KERNEL, PUERTO_ESCUCHA, kernel_logger, "ENTRADASALIDA");

    return EXIT_SUCCESS;
}
