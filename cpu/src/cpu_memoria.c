#include "../includes/cpu_memoria.h"

void pedir_info_inicial_a_memoria(){
    // Enviar
    enviar_opcode(SOLICITUD_INFO_INICIAL_A_MEMORIA,fd_memoria);

    // Recibir
    op_code code_op = recibir_operacion(fd_memoria);
    t_paquete* paquete_recibido = recibir_paquete(fd_memoria);
    t_buffer* buffer = paquete_recibido->buffer;
    if(code_op == RESPUESTA_INFO_INICIAL_A_CPU)
    {
        tamanio_pagina = leer_int_del_buffer(buffer);
        tamanio_memoria = leer_int_del_buffer(buffer);
        log_info(cpu_log_debug, "Tamanio de las Paginas de memoria = %d", tamanio_pagina);
        log_info(cpu_log_debug, "Tamanio de la Memoria = %d", tamanio_memoria);
        eliminar_paquete(paquete_recibido);
    }
    else
    {   
        log_error(cpu_log_debug, "No se recibio la informacion inicial de memoria.");
        eliminar_paquete(paquete_recibido);
        exit(EXIT_FAILURE);
    }
}

void pedir_instruccion_pseudocodigo(int pid,int pc){
    t_paquete* paquete = crear_paquete(PEDIDO_PSEUDOCODIGO);
    agregar_int_a_paquete(paquete,pid);
    agregar_int_a_paquete(paquete,pc);
    enviar_paquete(paquete, fd_memoria);
    eliminar_paquete(paquete);
}

char* recibir_instruccion_pseudocodigo(){
    op_code code_op = recibir_operacion(fd_memoria);
    t_paquete* paquete_recibido = recibir_paquete(fd_memoria);
    t_buffer* buffer = paquete_recibido->buffer;
    if(code_op == PSEUDOCODIGO)
    {
        char* instruccion_pseudocodigo = leer_string_del_buffer(buffer);//REVISAR POR QUE NO FUNCIONA. Si no funciona es por lo mismo que el pcb_global!
        printf("Instruccion_pseudocodigo: %s \n", instruccion_pseudocodigo);
        eliminar_paquete(paquete_recibido);
        return instruccion_pseudocodigo;
    }
    else
    {   
        log_error(cpu_log_debug, "No se recibio un pseudocodigo.");
        eliminar_paquete(paquete_recibido);
        exit(EXIT_FAILURE);
    }
};

int pedir_numero_de_marco_a_memoria(int numero_de_pagina){
    // Enviar
	t_paquete* paquete = crear_paquete(SOLICITUD_NUMERO_DE_MARCO_A_MEMORIA);
    agregar_int_a_paquete(paquete, pcb_global->pid);
    agregar_int_a_paquete(paquete, numero_de_pagina);
    enviar_paquete(paquete, fd_memoria);
    eliminar_paquete(paquete);

	// Recibir
    op_code code_op = recibir_operacion(fd_memoria);
    t_paquete* paquete_recibido = recibir_paquete(fd_memoria);
    t_buffer* buffer = paquete_recibido->buffer;
    if(code_op == RESPUESTA_NUMERO_DE_MARCO_A_CPU)
    {
        int numero_de_marco = leer_int_del_buffer(buffer);
        log_info(cpu_log_debug, "Numero de marco obtenido = %d", numero_de_marco);
        eliminar_paquete(paquete_recibido);
        return numero_de_marco;
    }
    else
    {   
        log_error(cpu_log_debug, "No se recibio un marco de pagina de memoria.");
        eliminar_paquete(paquete_recibido);
        exit(EXIT_FAILURE);
    }
}

u_int32_t leer_valor_de_memoria(int pid, int direccion_fisica)
{
	// Enviar
	t_paquete* paquete = crear_paquete(SOLICITUD_LEER_VALOR_EN_MEMORIA);
    //agregar_int_a_paquete(paquete, pid);
    agregar_int_a_paquete(paquete, direccion_fisica);
    agregar_int_a_paquete(paquete, sizeof(uint32_t));//tamanio de lo que quiero leer.
    enviar_paquete(paquete, fd_memoria);
    eliminar_paquete(paquete);
    
    // Recibir
    op_code code_op = recibir_operacion(fd_memoria);
    //t_paquete* paquete_recibido2 = recibir_paquete(fd_memoria);
    t_buffer* buffer = paquete->buffer; // Cambiar "paquete" por "paquete_recibido"
    if(code_op == RESPUESTA_LEER_VALOR_EN_MEMORIA)
    {
        uint32_t valor_leido = leer_uint_32_del_buffer(buffer); // Hacer lo mismo para uint_8
        log_info(cpu_log_debug, "Lectura de valor obtenida = %d", valor_leido);
        //eliminar_paquete(paquete_recibido);
        return valor_leido;
    }
    else
    {   
        log_error(cpu_log_debug, "No se recibio una lectura de valor de memoria.");
        //eliminar_paquete(paquete_recibido);
        exit(EXIT_FAILURE);
    }
    
}

void escribir_valor_en_memoria(int pid, int direccion_fisica, int tamanio, u_int32_t valor_a_escribir){
    // Enviar
    t_paquete* paquete = crear_paquete(SOLICITUD_ESCRIBIR_VALOR_EN_MEMORIA);
    //agregar_int_a_paquete(paquete, pid);
    agregar_int_a_paquete(paquete, direccion_fisica);
    agregar_int_a_paquete(paquete, sizeof(uint32_t)); //tamanio de lo que quiero leer.
    agregar_uint32_a_paquete(paquete, valor_a_escribir);
    enviar_paquete(paquete, fd_memoria);
    eliminar_paquete(paquete);
}

int pedir_ajustar_tamanio_del_proceso(int pid, int tamanioNuevo){
    // Enviar
    t_paquete* paquete = crear_paquete(SOLICITUD_MODIFICAR_TAMANIO);
    agregar_int_a_paquete(paquete,pid);
    agregar_int_a_paquete(paquete,tamanioNuevo);
    enviar_paquete(paquete, fd_memoria);
    eliminar_paquete(paquete);
    // Recibir
    op_code code_op_recibido = recibir_operacion(fd_memoria);
    t_paquete* paquete_recibido = recibir_paquete(fd_memoria);
    t_buffer* buffer = paquete_recibido->buffer;
    if(code_op_recibido == RESPUESTA_MODIFICAR_TAMANIO)
    {
        int nuevo_tamanio = leer_int_del_buffer(buffer);
        log_info(cpu_log_debug, "El nuevo tamanio del proceso en Memoria es de = %d", nuevo_tamanio);
        eliminar_paquete(paquete_recibido);
        return nuevo_tamanio;
    }
    else
    {   
        log_error(cpu_log_debug, "No se recibio una respuesta sobre la modificacion del tamanio del proceso en memoria.");
        eliminar_paquete(paquete_recibido);
        exit(EXIT_FAILURE);
    }
}
