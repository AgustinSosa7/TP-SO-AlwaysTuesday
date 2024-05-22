#include <stdlib.h>
#include <stdio.h>
#include <../includes/kernel.h>

int main()
{
    // Inicializar KERNEL
    inicializar_kernel();

/////////////////////////--CONEXIONES--//////////////////////

    conexion_kernel_memoria();
    char* pseudocogido = "PLANI1.txt12"; //VER POR QUE NO RECIBE LOS ULTIMOS 2 CARACTERES!
    log_info(kernel_log_debug, "ENVIO DE PSEUDO.%s",pseudocogido);
    enviar_direccion_pesudocodigo(pseudocogido);
    sleep(5);

    /*conexion_kernel_cpu_dispatch();
    conexion_kernel_cpu_interrupt();
    conexion_kernel_entradaSalida();
    */

/////////////////////////--ATENDER-MENSAJES--//////////////////////

    //Atender los mensajes de MEMORIA
    /*pthread_t hilo_memoria;
    pthread_create(&hilo_memoria, NULL, (void*) atender_kernel_memoria, NULL);
    pthread_detach(hilo_memoria);

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

    //leer consola interactiva
    // iniciar_consola();
    // leer_consola();
    */
    return EXIT_SUCCESS;
}


