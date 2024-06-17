#include "../includes/memoria_cpu.h"

void enviar_info_inicial(){
    //Recibir
    op_code code_op_recibido = recibir_operacion(fd_cpu);
    //Enviar
    if(code_op_recibido == SOLICITUD_INFO_INICIAL_A_MEMORIA)
    {
        t_paquete* paquete = crear_paquete(RESPUESTA_INFO_INICIAL_A_CPU);
        agregar_int_a_paquete(paquete,TAM_PAGINA);
        agregar_int_a_paquete(paquete,TAM_MEMORIA);
        enviar_paquete(paquete, fd_cpu);
        eliminar_paquete(paquete);
        log_info(memoria_log_debug, "Se envio la informacion inicial al CPU.");
    }
    else
    {   
        log_error(memoria_log_debug, "No se recibio el pedido de la informacion inicial.");
        exit(EXIT_FAILURE);
    }
}

void recibir_pedido_instruccion_y_enviar(){
    t_paquete* paquete = recibir_paquete(fd_cpu);
    t_buffer* buffer = paquete->buffer;
    int pid = leer_int_del_buffer(buffer);
    int pc = leer_int_del_buffer(buffer);
    printf("Busco el proceso...\n");
    //t_proceso* proceso = malloc(sizeof(proceso));
    t_proceso* proceso = buscar_proceso_en_memoria(pid);
    printf("encontre el proceso %d",proceso->pid);
    enviar_instruccion_pesudocodigo((proceso->instrucciones),pc);
    free(buffer);
    free(paquete);
    log_info(memoria_logger,"Pseudocodigo enviado. PID: %d PROGRAM COUNTER: %d",pid,pc); //BORRAR
};

void recibir_pedido_marco_y_enviar(){
    t_paquete* paquete = recibir_paquete(fd_cpu);
    t_buffer* buffer = paquete->buffer;
    int pid = leer_int_del_buffer(buffer);
    int numero_de_pagina = leer_int_del_buffer(buffer);
    int marco_pedido = traer_numero_marco(buscar_proceso_en_memoria(pid)->pid,numero_de_pagina);
    free(buffer);
    free(paquete);

    t_paquete* paquete_a_enviar = crear_paquete(RESPUESTA_NUMERO_DE_MARCO_A_CPU);
    agregar_int_a_paquete(paquete_a_enviar, marco_pedido);
    enviar_paquete(paquete_a_enviar, fd_cpu);
    eliminar_paquete(paquete_a_enviar);
    log_info(memoria_logger,"Marco enviado. PID: %d PAGINA: %d MARCO: %d",pid,numero_de_pagina,marco_pedido); //BORRAR
};

void atender_cpu(){
    
    enviar_info_inicial();
    while(1){
        op_code code_op = recibir_operacion(fd_cpu);
        sleep(2);
        printf("OP Code recibido %d\n",code_op);
            if(code_op == PEDIDO_PSEUDOCODIGO) 
            { 
                recibir_pedido_instruccion_y_enviar();
            }
            else if (code_op == SOLICITUD_NUMERO_DE_MARCO_A_MEMORIA)
            {
                recibir_pedido_marco_y_enviar();
            }
            else if (code_op == SOLICITUD_MODIFICAR_TAMANIO)
            {
                printf("PEDIDO DE RESIZE\n"); //BORRAR
            }
            else
            {
                log_error(memoria_logger,"NO ENTIENDO QUE ME PIDE EL CPU"); //BORRAR
                exit(EXIT_FAILURE);
            }
    }
    printf("FIN ATENDER CPU");
}

