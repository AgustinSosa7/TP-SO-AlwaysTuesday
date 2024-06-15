#include "../includes/cpu_memoria.h"

void pedir_info_inicial_a_memoria(){
    // Enviar
	t_paquete* paquete1 = crear_paquete(SOLICITUD_INFO_INICIAL_A_MEMORIA);
    enviar_paquete(paquete1, fd_memoria);
    eliminar_paquete(paquete1);

    // Recibir
    op_code code_op = recibir_operacion(fd_memoria);
    t_paquete* paquete2 = recibir_paquete(fd_memoria);
    t_buffer* buffer = paquete2->buffer;
    if(code_op == RESPUESTA_INFO_INICIAL_A_CPU)
    {
        int tamanio_pagina = leer_int_del_buffer(buffer);
        int tamanio_memoria = leer_int_del_buffer(buffer);
        log_info(cpu_log_debug, "Tamanio de las Paginas de memoria = %d", tamanio_pagina);
        log_info(cpu_log_debug, "Tamanio de la Memoria = %d", tamanio_memoria);
    }
    else
    {   
        log_error(cpu_log_debug, "No se recibio la informacion inicial de memoria.");
        free(paquete2);
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
    t_paquete* paquete = recibir_paquete(fd_memoria);
    t_buffer* buffer = paquete->buffer;
    if(code_op == PSEUDOCODIGO)
    {
        char* instruccion_pseudocodigo = leer_string_del_buffer(buffer);//REVISAR POR QUE NO FUNCIONA. Si no funciona es por lo mismo que el pcb_global!
        printf("Instruccion_pseudocodigo: %s \n", instruccion_pseudocodigo);
        free(paquete);
        return instruccion_pseudocodigo;
    }
    else
    {   
        log_error(cpu_log_debug, "No se recibio un pseudocodigo.");
        free(paquete);
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
    t_paquete* paquete2 = recibir_paquete(fd_memoria);
    t_buffer* buffer = paquete2->buffer;
    if(code_op == RESPUESTA_NUMERO_DE_MARCO_A_CPU)
    {
        int numero_de_marco = leer_int_del_buffer(buffer);
        log_info(cpu_log_debug, "Numero de marco obtenido = %d", numero_de_marco);
        return numero_de_marco;
    }
    else
    {   
        log_error(cpu_log_debug, "No se recibio un marco de pagina de memoria.");
        free(paquete);
        exit(EXIT_FAILURE);
    }
}

u_int32_t leer_valor_de_memoria(int pid, int direccion_fisica)
{
	// Enviar
	t_paquete* paquete = crear_paquete(SOLICITUD_LEER_VALOR_EN_MEMORIA);
    agregar_int_a_paquete(paquete, pid);
    agregar_int_a_paquete(paquete, direccion_fisica);
    enviar_paquete(paquete, fd_memoria);
    eliminar_paquete(paquete);
    
    // Recibir
    op_code code_op = recibir_operacion(fd_memoria);
    t_paquete* paquete2 = recibir_paquete(fd_memoria);
    t_buffer* buffer = paquete->buffer;
    if(code_op == RESPUESTA_LEER_VALOR_EN_MEMORIA)
    {
        uint32_t valor_leido = leer_uint_32_del_buffer(buffer);
        log_info(cpu_log_debug, "Lectura de valor obtenida = %d", valor_leido);
        return valor_leido;
    }
    else
    {   
        log_error(cpu_log_debug, "No se recibio una lectura de valor de memoria.");
        free(paquete);
        exit(EXIT_FAILURE);
    }
}

void escribir_valor_en_memoria(int pid, int direccion_fisica, u_int32_t valor_a_escribir){
    // Enviar
    t_paquete* paquete = crear_paquete(SOLICITUD_ESCRIBIR_VALOR_EN_MEMORIA);
    agregar_int_a_paquete(paquete, pid);
    agregar_int_a_paquete(paquete, direccion_fisica);
    agregar_uint32_a_paquete(paquete, valor_a_escribir);
    enviar_paquete(paquete, fd_memoria);
    eliminar_paquete(paquete);
    
    // Recibir
    op_code code_op = recibir_operacion(fd_memoria);
    t_paquete* paquete2 = recibir_paquete(fd_memoria);
    t_buffer* buffer = paquete->buffer;
    if(code_op == RESPUESTA_ESCRIBIR_VALOR_EN_MEMORIA)
    {
        log_info(cpu_log_debug, "Se escribio el valor en memoria correctamente!");
    }
    else
    {   
        log_error(cpu_log_debug, "No se recibio una respuesta sobre la solicitud de escribir un valor en memoria.");
        free(paquete);
        exit(EXIT_FAILURE);
    }
}