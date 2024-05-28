#include <stdlib.h>
#include <stdio.h>
#include <../includes/kernel.h>
#include <../includes/pcb.h>

int main()
{   
    
    // Inicializar KERNEL
    inicializar_kernel();


/////////////////////////--INICIAR PLANIFICADORES-//////////////////////
    pthread_t plani_largo_plazo;
    pthread_create(&plani_largo_plazo,NULL,(void*) planif_largo_plazo,NULL);
    pthread_detach(plani_largo_plazo);

    pthread_t plani_corto_plazo;
    pthread_create(&plani_corto_plazo,NULL,(void*) planif_corto_plazo,NULL);
    pthread_detach(plani_corto_plazo);



/////////////////////////--CONEXIONES--//////////////////////

    conexion_kernel_memoria();
    char* pseudocogido = "PLANI1.txt"; 
    log_info(kernel_log_debug, "ENVIO DE PSEUDO.%s",pseudocogido);
    enviar_direccion_pesudocodigo(pseudocogido);
    sleep(5);

    conexion_kernel_cpu_dispatch();
    
    //conexion_kernel_cpu_interrupt();
    
    //conexion_kernel_entradaSalida();
    /////////////////-Prueba mandar y recibir PCB-///////////////////
    t_pcb * un_pcb;
    imprimir_pcb(un_pcb, kernel_logger);
    enviar_pcb_a(un_pcb, fd_cpu_dispatch);
    

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


