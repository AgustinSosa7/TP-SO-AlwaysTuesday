#include "../includes/memoria_kernel.h"

t_proceso* crear_proceso_nuevo(){
    //op_code code_op = recibir_operacion(fd_kernel);
    t_paquete* paquete = recibir_paquete(fd_kernel);
    t_buffer* buffer = paquete->buffer;
    t_proceso* procesoNuevo = malloc(sizeof(t_proceso));

        //int pid = leer_int_del_buffer(buffer); //cambiar a leer algo del buffer
        //char* direccion_pseudocodigo = leer_string_del_buffer(buffer);
        procesoNuevo->pid = leer_int_del_buffer(buffer);
        procesoNuevo->direccion_pseudocodigo = leer_string_del_buffer(buffer);
        procesoNuevo->instrucciones = list_create();
        procesoNuevo->instrucciones = leer_archivo_pseudocodigo(procesoNuevo->direccion_pseudocodigo);
        procesoNuevo->long_tabla_pags = 0;
        procesoNuevo->tabla_de_paginas = NULL;
        //free(paquete);
        log_info(memoria_logger, "Cree el proceso: %d",procesoNuevo->pid);
        log_info(memoria_logger, "Direccion el proceso: %s",procesoNuevo->direccion_pseudocodigo);
        return procesoNuevo;
};

void finalizar_proceso(){
    t_paquete* paquete = recibir_paquete(fd_kernel);
    t_buffer* buffer = paquete->buffer;

    int pid = leer_int_del_buffer(buffer);

    t_proceso* proceso_eliminado = buscar_proceso_en_memoria(pid);
    liberar_marcos_memoria(proceso_eliminado,0); //el tamanio nuevo es 0 para que libere todos los marcos
    
    if(list_remove_element(procesos_memoria,proceso_eliminado)){
        log_info(memoria_logger, "Finalizo el proceso: %d",proceso_eliminado->pid);
        free(proceso_eliminado);
    }
    else
    {
        log_error(memoria_logger, "Error al finalizar el proceso: %d",pid);
    }
}

void atender_kernel(){
    while(1){
        op_code code_op_recibido = recibir_operacion(fd_kernel);
        sem_wait(&ejecucion);
        if(code_op_recibido == CREAR_PROCESO){
            list_add(procesos_memoria, crear_proceso_nuevo());
            //DEVOLVER OK A MILI. CREAR_PROCESO
        }
        else if(code_op_recibido == FINALIZAR_PROCESO_MEMORIA){
            finalizar_proceso(); // ESto debe abuscar un proceso en memoria. Marcar libres todos los marcos del proceso, y luego de esto hacerle free.
            log_info(memoria_logger, "Se pidio finalizar un proceso");
            //DEVOLVER OK A MILI. FINALIZAR_PROCESO_MEMORIA
        }
        else{
            log_error(memoria_logger, "No se recibio un pedido correcto de kernel.");
            exit(EXIT_FAILURE);
        }
        sem_post(&retardo);
    }
}