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

void atender_cpu(){
    
    enviar_info_inicial();
    while(1){
        op_code code_op = recibir_operacion(fd_cpu);
        printf("OP Code recibido %d\n",code_op);
        if(code_op == PEDIDO_PSEUDOCODIGO) 
        { 
            t_pedido* pedido = recibir_instruccion_a_enviar();
            printf("Proceso pedido: %d\n",pedido->pid);
            //recv_cpu lo que tengo que pasarte. pid PC
            t_proceso* proceso1 = buscar_proceso_en_memoria(pedido->pid);
            printf("ENCONTRADO %d\n",proceso1->pid); //BORRAR
            enviar_instruccion_pesudocodigo(proceso1->instrucciones,pedido->pc);//PROGRAM_COUNTER_PEDIDO);
            free(pedido);
            sleep(3);
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