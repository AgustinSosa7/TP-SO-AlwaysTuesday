#include <stdlib.h>
#include <stdio.h>
#include <../includes/kernel.h>
#include <../includes/pcb.h>

int main()
{   
    
    // Inicializar KERNEL
    inicializar_kernel();

//SOLO PARA PROBAR
    t_pcb un_pcb;

    un_pcb.pid=1;
    un_pcb.QUANTUM=2;
    un_pcb.registros_CPU.PC=3;
    un_pcb.registros_CPU.AX=4;
    un_pcb.registros_CPU.BX=5;
    un_pcb.registros_CPU.CX=6;
    un_pcb.registros_CPU.DX=7;
    un_pcb.registros_CPU.EAX=8;
    un_pcb.registros_CPU.EBX=9;
    un_pcb.registros_CPU.ECX=10;
    un_pcb.registros_CPU.EDX=11;
    un_pcb.registros_CPU.SI=12;
    un_pcb.registros_CPU.DI=13;

/////////////////////////--CONEXIONES--//////////////////////

    conexion_kernel_memoria();
    char* pseudocogido = "PLANI1.txt"; //VER POR QUE NO RECIBE LOS ULTIMOS 2 CARACTERES!
    log_info(kernel_log_debug, "ENVIO DE PSEUDO.%s",pseudocogido);
    enviar_direccion_pesudocodigo(pseudocogido);
    sleep(5);

    conexion_kernel_cpu_dispatch();
    
    //conexion_kernel_cpu_interrupt();
    
    //conexion_kernel_entradaSalida();
    imprimir_pcb(&un_pcb, kernel_logger);
    enviar_pcb_a(&un_pcb, fd_cpu_dispatch);
    

/////////////////////////--ATENDER-MENSAJES--//////////////////////
    //Atender los mensajes de MEMORIA
    /*pthread_t hilo_memoria;
    pthread_create(&hilo_memoria, NULL, (void*) atender_kernel_memoria, NULL);
    pthread_detach(hilo_memoria);*/

    //Atender los mensajes de CPU - DISPATCH
    pthread_t hilo_cpu_dispatch;
    pthread_create(&hilo_cpu_dispatch, NULL, (void*)atender_kernel_cpu_dispatch, NULL);
    pthread_detach(hilo_cpu_dispatch);

   //Atender los mensajes de CPU - INTERRUPT 
   /*
    pthread_t hilo_cpu_interrupt;
    pthread_create(&hilo_cpu_interrupt, NULL, (void*)atender_kernel_cpu_interrupt, NULL);
    pthread_detach(hilo_cpu_interrupt);

   //Atender los mensajes de ENTRADA SALIDA
    pthread_t hilo_entradaSalida;
    pthread_create(&hilo_entradaSalida, NULL, (void*)atender_kernel_entradaSalida, NULL);
    pthread_join(hilo_entradaSalida,NULL); //Pruena MILI :3
*/
    //leer consola interactiva
    // iniciar_consola();
    // leer_consola();


    return EXIT_SUCCESS;
}


