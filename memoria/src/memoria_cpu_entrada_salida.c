#include "../includes/memoria_cpu_entrada_salida.h"

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
    //t_proceso* proceso = malloc(sizeof(proceso));
    t_proceso* proceso = buscar_proceso_en_memoria(pid);
    enviar_instruccion_pesudocodigo((proceso->instrucciones),pc);
    //free(buffer);
    eliminar_paquete(paquete);//valgrind
    log_info(memoria_log_debug,"PID: %d PROGRAM COUNTER: %d",pid,pc); //BORRAR
};

void recibir_pedido_marco_y_enviar(){
    t_paquete* paquete = recibir_paquete(fd_cpu);
    t_buffer* buffer = paquete->buffer;
    int pid = leer_int_del_buffer(buffer);
    int numero_de_pagina = leer_int_del_buffer(buffer);

    pthread_mutex_lock(&mutex_tabla_paginas);
    int marco_pedido = traer_numero_marco(buscar_proceso_en_memoria(pid),numero_de_pagina);
    pthread_mutex_unlock(&mutex_tabla_paginas);

    t_paquete* paquete_a_enviar = crear_paquete(RESPUESTA_NUMERO_DE_MARCO_A_CPU);
    agregar_int_a_paquete(paquete_a_enviar, marco_pedido);
    enviar_paquete(paquete_a_enviar, fd_cpu);

    eliminar_paquete(paquete);
    eliminar_paquete(paquete_a_enviar);
    
    log_info(memoria_logger,"PID: %d PAGINA: %d MARCO: %d",pid,numero_de_pagina,marco_pedido);

};

void recibir_modificacion_de_tamanio(){
    t_paquete* paquete = recibir_paquete(fd_cpu);
    t_buffer* buffer = paquete->buffer;
    int pid = leer_int_del_buffer(buffer);
    int tamanio_nuevo = calcular_paginas_necesarias(leer_int_del_buffer(buffer));
    //int paginas_necesarias = calcular_paginas_necesarias(tamanio_nuevo);
    t_proceso* proceso = buscar_proceso_en_memoria(pid);

    int resultado_ajuste = ajustar_tamanio_proceso(proceso,tamanio_nuevo);

    eliminar_paquete(paquete);

    t_paquete* paquete_a_enviar = crear_paquete(RESPUESTA_MODIFICAR_TAMANIO);
    if(resultado_ajuste == 0){
        //mandar ok
        agregar_int_a_paquete(paquete_a_enviar,proceso->long_tabla_pags);
    }
    else
    {
        //mandar out of memory
        log_info(memoria_logger, "PID: %d - Out of memory\n", pid);//Ver como unificar los logs de escritura y lectura.

        agregar_int_a_paquete(paquete_a_enviar,resultado_ajuste);// -1
    }
    enviar_paquete(paquete_a_enviar, fd_cpu);
    eliminar_paquete(paquete_a_enviar);
}

void recibir_solicitud_de_lectura(int socket){
    t_paquete* paquete = recibir_paquete(socket);
    t_buffer* buffer = paquete->buffer;

    int pid = leer_int_del_buffer(buffer);
    int direccion_fisica = leer_int_del_buffer(buffer);
    int tamanio = leer_int_del_buffer(buffer);

    //Log obligatorio. (Creación / destrucción de Tabla de Páginas:)
    log_info(memoria_logger, "PID: %d - Accion: LEER - Direccion fisica: %d - Tamaño %d", pid, direccion_fisica,tamanio);//Ver como unificar los logs de escritura y lectura.
    void* leido = leer_espacio_usuario(direccion_fisica,tamanio);

    t_paquete* paquete_a_enviar = crear_paquete(RESPUESTA_LEER_VALOR_EN_MEMORIA);
    agregar_void_a_paquete(paquete_a_enviar,leido,tamanio);
    enviar_paquete(paquete_a_enviar, socket);
    
    eliminar_paquete(paquete_a_enviar);
    eliminar_paquete(paquete);
    free(leido);
}

void recibir_solicitud_de_escritura(int socket){
    t_paquete* paquete = recibir_paquete(socket);
    t_buffer* buffer = paquete->buffer;

    int pid = leer_int_del_buffer(buffer);
    int direccion_fisica = leer_int_del_buffer(buffer);
    int tamanio = leer_int_del_buffer(buffer);
    void* a_escribir = leer_void_del_buffer(buffer,tamanio);

    //Log obligatorio. (Creación / destrucción de Tabla de Páginas:)
    log_info(memoria_logger, "PID: %d - Accion: ESCRIBIR - Direccion fisica: %d - Tamaño %d", pid ,direccion_fisica,tamanio); //Ver como unificar los logs de escritura y lectura.
    escribir_espacio_usuario(direccion_fisica,tamanio,a_escribir); 

    //mientras todo este ok se manda un ok.
    enviar_opcode(RESPUESTA_ESCRIBIR_VALOR_EN_MEMORIA,socket);


    free(a_escribir);
    eliminar_paquete(paquete);
}


void atender_cpu(){
    int control_key = 1;
    log_info(memoria_logger, "Atendiendo CPU...");

    usleep(RETARDO_RESPUESTA * 1000); //retardo
    enviar_info_inicial();

    while(control_key){
        int code_op = recibir_operacion(fd_cpu); //ver de cambiar a opcode
        //log_info(memoria_log_debug, "Se recibio algo de CPU: %d", code_op);

        usleep(RETARDO_RESPUESTA * 1000); //retardo
        
        switch (code_op)
            {
            case PEDIDO_PSEUDOCODIGO:
                recibir_pedido_instruccion_y_enviar();
                break;
            case SOLICITUD_NUMERO_DE_MARCO_A_MEMORIA:
                recibir_pedido_marco_y_enviar();
                break;
            case SOLICITUD_MODIFICAR_TAMANIO:
                recibir_modificacion_de_tamanio();  
                break;
            case SOLICITUD_LEER_VALOR_EN_MEMORIA:
                recibir_solicitud_de_lectura(fd_cpu);
                break;
            case SOLICITUD_ESCRIBIR_VALOR_EN_MEMORIA:
                recibir_solicitud_de_escritura(fd_cpu);
                break;
            case -1:
                log_error(memoria_logger, "Desconexion de CPU");      
                control_key = 0;
                break;
            default:
                log_warning(memoria_logger, "Operacion desconocida de CPU");
                break;
            }
        //sem_post(&retardo);   
    }
}


void atender_entradasalida(void* fd_io){
      int fd_entradasalida = *((int*)fd_io);
      int control_key = 1;
      log_info(memoria_logger, "Atendiendo Entradasalida...");
      while (control_key){
      int code_op = recibir_operacion(fd_entradasalida);
      log_info(memoria_log_debug, "Se recibio algo de EntradaSalida: %d. \n", code_op);
      
      usleep(RETARDO_RESPUESTA * 1000);

      switch (code_op)
      {
      case PEDIR_REGISTRO:
            //printf("PEDIDO DE LECTURA\n"); //BORRAR 
            recibir_solicitud_de_lectura(fd_entradasalida);
            break;
        case  GUARDAR_REGISTRO:
            recibir_solicitud_de_escritura(fd_entradasalida);
            break;
        case -1:
            log_error(memoria_logger, "Desconexion de ENTRADASALIDA");      
            control_key = 0;
            break;
        default:
            log_warning(memoria_logger, "Operacion desconocida de ENTRADASALIDA");
            break;
        }
        
      }
      //retornar el void* definido en la función para que no genere warning
}