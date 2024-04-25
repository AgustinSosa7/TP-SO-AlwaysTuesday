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

    //Atender los mensajes de CPU - DISPATCH
    pthread_t hilo_cpu_dispatch;
    pthread_create(&hilo_cpu_dispatch, NULL, (void*), NULL);
    pthread_detach(hilo_cpu_dispatch);

   //Atender los mensajes de CPU - INTERRUPT
    pthread_t hilo_cpu_interrupt;
    pthread_create(&hilo_cpu_interrupt, NULL, (void*), NULL);
    pthread_detach(hilo_cpu_interrupt);

   //Atender los mensajes de ENTRADA SALIDA
    pthread_t hilo_entradaSalida;
    pthread_create(&hilo_entradaSalida, NULL, (void*), NULL);
    pthread_detach(hilo_entradaSalida);

    //Atender los mensajes de MEMORIA
    pthread_t hilo_memoria;
    pthread_create(&hilo_memoria, NULL, (void*), NULL);
    pthread_join(hilo_memoria, NULL);
    return EXIT_SUCCESS;
}
