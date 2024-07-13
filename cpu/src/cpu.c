#include "../includes/cpu.h"


int main(int argc, char** argv){

   if(validar_parametros_incorrectos_main(argc, 2, argv)){
        return EXIT_FAILURE;
    }
    
    // Para cerrar los File Descriptors en caso de EXIT.
    atexit(terminar_cpu);
    
    // Inicializar CPU
    inicializar_cpu(argv[1]);

    // Conectarnos como cliente a MEMORIA
    log_info(cpu_logger, "Creando conexion con MEMORIA...");
    fd_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA, cpu_logger);  
    gestionar_handshake_como_cliente(fd_memoria, "MEMORIA", cpu_logger);    
    
    // Pedimos el Tamaño de las Páginas y de la Memoria
    pedir_info_inicial_a_memoria();

    // Iniciar server de CPU - DISPATCH
    pthread_t hilo_conexion_dispatch;
    pthread_create(&hilo_conexion_dispatch, NULL, (void*)conexion_cpu_kernel_dispatch, NULL);
    pthread_detach(hilo_conexion_dispatch); 

    // Iniciar server de CPU - INTERRUPT
    pthread_t hilo_conexion_interrupt;
    pthread_create(&hilo_conexion_interrupt, NULL, (void*)conexion_cpu_kernel_interrupt, NULL);
    pthread_join(hilo_conexion_interrupt, NULL);

    // Atender los mensajes de Kernel - Dispatch
    pthread_t hilo_kernel_dispatch;
    pthread_create(&hilo_kernel_dispatch, NULL, (void*)atender_dispatch, NULL);
    pthread_detach(hilo_kernel_dispatch);    

    // Atender los mensajes de Kernel - Interrupt
    pthread_t hilo_kernel_interrupt;
    pthread_create(&hilo_kernel_interrupt, NULL, (void*)atender_interrupt, NULL);
    pthread_detach(hilo_kernel_interrupt);

    // Método principal
    pthread_t hilo_ciclo_instruccion;
    pthread_create(&hilo_ciclo_instruccion, NULL, (void*)ciclo_instruccion, NULL);
    pthread_join(hilo_ciclo_instruccion,NULL);

    return EXIT_SUCCESS;
}

void conexion_cpu_kernel_dispatch(){
    fd_cpu_dispatch = iniciar_servidor(PUERTO_ESCUCHA_DISPATCH, cpu_logger, IP_CPU);
    log_info(cpu_logger, "Esperando a DISPATCH...");
    fd_kernel_dispatch = esperar_cliente(fd_cpu_dispatch, cpu_logger, "KERNEL-DISPATCH");
    gestionar_handshake_como_server(fd_kernel_dispatch, cpu_logger, "KERNEL-DISPATCH");
}

void conexion_cpu_kernel_interrupt(){
    fd_cpu_interrupt= iniciar_servidor(PUERTO_ESCUCHA_INTERRUPT, cpu_logger, IP_CPU);
    log_info(cpu_logger, "Esperando a INTERRUPT...");
    fd_kernel_interrupt = esperar_cliente(fd_cpu_interrupt, cpu_logger, "KERNEL-INTERRUPT");
    gestionar_handshake_como_server(fd_kernel_interrupt, cpu_logger, "KERNEL-INTERRUPT");
}

void limpiar_tlb()
{
    if(list_size(tlb) > 0)
    {
        t_list_iterator *iterador = list_iterator_create(tlb);
	    while (list_iterator_has_next(iterador))
	    {
	    	t_entrada_tlb* entrada = list_iterator_next(iterador);
            temporal_destroy(entrada->tiempo_carga);
            temporal_destroy(entrada->tiempo_ultima_ref);
            free(entrada);
    	}
	    list_iterator_destroy(iterador);
        list_clean(tlb);
    }
    list_destroy(tlb);
}

void terminar_cpu()
{
	if (cpu_logger != NULL)
	{
		log_warning(cpu_logger, "Algo salio mal!");
		log_warning(cpu_logger, "Finalizando %s", "CPU");

        log_destroy(cpu_logger);
        log_destroy(cpu_log_debug);
	}

    if (cpu_config != NULL)
	{
		config_destroy(cpu_config);
        /*
        free(IP_MEMORIA);
        free(IP_CPU);
        free(PUERTO_MEMORIA);
        free(PUERTO_ESCUCHA_DISPATCH);
        free(PUERTO_ESCUCHA_INTERRUPT);
        free(CANTIDAD_ENTRADAS_TLB);
        free(ALGORITMO_TLB);*/
	}

    if (pcb_global != NULL)
    {
        free(pcb_global->registros_cpu);
        free(pcb_global);
    }

    limpiar_tlb();

	if (fd_memoria != -1)
	{
		close(fd_memoria);
	}

	if (fd_cpu_dispatch != -1)
	{
		close(fd_cpu_dispatch);
	}

	if (fd_kernel_dispatch != -1)
	{
		close(fd_kernel_dispatch);
	}

	if (fd_cpu_interrupt != -1)
	{
		close(fd_cpu_interrupt);
	}

	if (fd_kernel_interrupt != -1)
	{
		close(fd_kernel_interrupt);
	}
}