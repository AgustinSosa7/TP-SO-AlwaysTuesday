#include "../includes/kernel.h"


int main(int argc, char** argv)
{   
    if(validar_parametros_incorrectos_main(argc, 2, argv)){
        	    return EXIT_FAILURE;
    }
    // Inicializar KERNEL
    inicializar_kernel(argv[1]);


/////////////////////////--INICIAR PLANIFICADORES-//////////////////////

    pthread_t plani_largo_plazo;
    pthread_create(&plani_largo_plazo,NULL,(void*) planif_largo_plazo,NULL);
    pthread_detach(plani_largo_plazo);
    printf("se iniciaron los planificadores");
    pthread_t plani_corto_plazo;
    pthread_create(&plani_corto_plazo,NULL,(void*) planif_corto_plazo,NULL);
    pthread_detach(plani_corto_plazo);
    
/////////////////////////--CONEXIONES--//////////////////////

    conexion_kernel_memoria();

    conexion_kernel_cpu_dispatch();
    
    conexion_kernel_cpu_interrupt();
    
   // conexion_kernel_entradaSalida();

    // PRUEBA
    
    //t_pcb *pcb_prueba= crearPcb();
    //enviar_pcb_a(pcb_prueba, fd_cpu_dispatch, PCB);

// leer consola interactiva
     iniciar_consola();
     leer_consola();


//   int pid = 3;
//   char* pseudocogido = "PLANI1.txt"; 
//   log_info(kernel_log_debug, "ENVIO DE PSEUDO: %s con un pid de: %d\n",pseudocogido,pid);
//   enviar_direccion_pesudocodigo(pseudocogido,pid);
//   sleep(5);
   
    return EXIT_SUCCESS;
}


