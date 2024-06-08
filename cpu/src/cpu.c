#include "../includes/cpu.h"


int main(int argc, char** argv){

    //DESCOMENTAR
   //if(validar_parametros_main(argc, 2, argv)){
   //     	    return EXIT_FAILURE;
    //}

    // Inicializar CPU
    inicializar_cpu(argv[1]);

    // Conectarnos como cliente a MEMORIA

    log_info(cpu_logger, "Creando conexion con MEMORIA...");
    fd_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA, cpu_logger);  
    gestionar_handshake_como_cliente(fd_memoria, "MEMORIA", cpu_logger);    

    //printf("Guardo las nuevas variables?\n");
    //contexto_ejecucion->pid = 3;
    //contexto_ejecucion->registros_cpu->PC = 0;
    //printf("Si!\n");

    // Iniciar server de CPU - DISPATCH

    // crear un hilo para cada uno
    
    fd_cpu_dispatch = iniciar_servidor(PUERTO_ESCUCHA_DISPATCH, cpu_logger, IP_CPU);
    log_info(cpu_logger, "Esperando a DISPATCH...");
    fd_kernel_dispatch = esperar_cliente(fd_cpu_dispatch, cpu_logger,"KERNEL-DISPATCH");
    gestionar_handshake_como_server(fd_kernel_dispatch, cpu_logger, "KERNEL-DISPATCH");

    // Iniciar server de CPU - INTERRUPT

    fd_cpu_interrupt= iniciar_servidor(PUERTO_ESCUCHA_INTERRUPT,cpu_logger, IP_CPU);
    log_info(cpu_logger, "Esperando a INTERRUPT...");
    fd_kernel_interrupt = esperar_cliente(fd_cpu_interrupt, cpu_logger,"KERNEL-INTERRUPT");
    gestionar_handshake_como_server(fd_kernel_interrupt, cpu_logger, "KERNEL-INTERRUPT");
    
    
    //Atender los mensajes de Kernel - Dispatch
    pthread_t hilo_kernel_dispatch;
    pthread_create(&hilo_kernel_dispatch, NULL, (void*)atender_cpu_kernel_dispatch,NULL);
    pthread_detach(hilo_kernel_dispatch);
    

    //Atender los mensajes de Kernel - Interrupt
    pthread_t hilo_kernel_interrupt;
    pthread_create(&hilo_kernel_interrupt,NULL,(void*)atender_interrupciones,NULL);
    pthread_join(hilo_kernel_interrupt, NULL);

    //Atender los mensajes de Memoria
   // pthread_t hilo_memoria;
   // pthread_create(&hilo_memoria, NULL, (void*)atender_cpu_memoria, NULL);
   // pthread_join(hilo_memoria, NULL);
//
    //Ciclos de instruccion
    printf("Antes de crear el hilo fetch\n");
    pthread_t hilo_fetch;
    pthread_create(&hilo_fetch, NULL, (void*)ciclo_instruccion, NULL);
    pthread_join(hilo_fetch, NULL);
  
    return EXIT_SUCCESS;
}
/*
// iniciar estructuras
void iniciar_estructuras(){
	string= string_array_new();
	string_array_push(&string, "SET");
	string_array_push(&string, "SUM");
	string_array_push(&string, "SUB");
	string_array_push(&string, "JNZ");
	string_array_push(&string, "IO_GEN_SLEEP");
	
	interrupt_proceso_id = NULL;
	interrupt_proceso_ticket = NULL;
	interrupt_motivo = NULL;
	mochila = NULL;
}

//inicializacion de samaforos
void iniciar_semaforo(){
    semaforo_init(&semaforo_control_fetch_decode,0,0);
    semaforo_init(&semaforo_control_decode_execute,0,0);
}

*/
//ciclo de instruccion(fetch-decode-execute-check_interrupt)
void ciclo_instruccion(){                          //crear dos hilos: uno para decode, otro para execute
    
    while(1){
    
    //FETCH  Me debería taraer instrucciones para decodificar

    char ** instruccion = ciclo_instruccion_fetch();  // tiene que ser una lista de Strings, no sé si está bien. acá iria lo que recibe del Fetch. Hablar con Lucas como llega
    
    //DECODE, Ver como afecta al PID 

    
    cod_instruccion instruccion_a_ejecutar = decodificacion_instruccion(instruccion[0]); //Va una lista de Strings

    //EXECUTE

    ejecucion_proceso(instruccion_a_ejecutar, instruccion); // Podría hacerse dentro el decode

    //CHECK INTERRUPT
    //verif del flag que modifico el kernel luego de mandar la interrupcion
    }  // El Check I. debe estar en el while????
    //atender interrupciones
}

