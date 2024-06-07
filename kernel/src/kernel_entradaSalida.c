#include "../includes/kernel_entradaSalida.h"

t_peticion* recibir_peticion(t_paquete* paquete){
    t_peticion* peticion = malloc(sizeof(t_peticion));
    void* buffer = paquete->buffer;
    peticion->instruccion = malloc(sizeof(char));
    peticion->interfaz = malloc(sizeof(char));

    peticion->instruccion = leer_string_del_stream(buffer);
    peticion->interfaz = leer_string_del_stream(buffer);
    peticion->parametros = leer_parametros(paquete,peticion->instruccion);

    return peticion;
} 

t_peticion_param* leer_parametros(t_paquete* paquete, char* instruccion){

    t_peticion_param* parametros = malloc(sizeof(t_peticion_param*));
    void* stream = paquete->buffer->stream;

    if(strcmp(instruccion,"IO_GEN_SLEEP") == 0){
        leer_algo_del_stream(stream, &parametros->tiempo_espera,sizeof(parametros->tiempo_espera));
        return parametros;
      }else if (strcmp(instruccion,"IO_STDIN_READ") == 0)
      {
            return parametros;
      }else if (strcmp(instruccion,"IO_STDOUT_WRITE") == 0)
      {
            return parametros;
      }else if (strcmp(instruccion,"IO_FS_CREATE") == 0)
      {
            return parametros;
      }else if (strcmp(instruccion,"IO_FS_DELETE") == 0)
      {
            return parametros;
      }else if (strcmp(instruccion,"IO_FS_TRUNCATE") == 0)
      {
            return parametros; 
      }else if (strcmp(instruccion,"IO_FS_WRITE") == 0)
      {
            return parametros; 
      }else //Es IO_FS_READ 
      {
            return parametros;
      }
}           

t_interfaz* validar_peticion(t_peticion* peticion, t_pcb* pcb){ 
      char* nombre_io = peticion->interfaz;
      char* instruccion = peticion->instruccion;

    t_interfaz* interfaz = existe_la_interfaz(nombre_io, pcb);
    if(interfaz->esta_conectada){
        validar_interfaz_admite_instruccion(interfaz, instruccion, pcb); 
    }else{
      //enviar_proceso_blocked_a_exit(interfaz->cola_procesos_blocked);

      log_error(kernel_logger,"La interfaz %s no se encuentra conectada. Proceso enviado a EXIT.\n", interfaz->nombre);
      //matar al hilo en el que me encuentro?
    }
    return interfaz;
    }


t_interfaz* existe_la_interfaz(char* nombre_io, t_pcb* pcb) {
    bool esta_la_io(void* io) {
      return esta_o_no(nombre_io, io);
    }
    t_interfaz* interfaz = list_find(IOS_CONECTADOS, esta_la_io); 
    if (interfaz != NULL) {
        return interfaz;
    } else {
        log_error(kernel_logger, "No se ha encontrado la interfaz: %s. Proceso enviado a EXIT.\n", nombre_io);
        cambiar_estado(pcb, EXIT);
        return NULL; // Opcional: devuelve NULL para indicar que la interfaz no fue encontrada
    }
}   


bool esta_o_no(char* nombre_io, t_interfaz* io){ 
        return (strcmp(nombre_io, io->nombre) == 0);
}


void validar_interfaz_admite_instruccion(t_interfaz* interfaz, char* instruccion, t_pcb* un_pcb){
      if(contains_string(interfaz->instrucciones_posibles, instruccion)){

      }else{
      //enviar_proceso_blocked_a_exit(interfaz->cola_procesos_blocked);
            //matar al hilo en el que me encuentro?
      }
}


void enviar_proceso_a_blocked(t_peticion_pcb_interfaz* peticion_pcb_interfaz)
{
    peticion_pcb_interfaz->un_pcb->estado_pcb = BLOCKED;

    t_proceso_blocked* proceso_blocked = malloc(sizeof(t_proceso_blocked));
    proceso_blocked->peticion = peticion_pcb_interfaz->peticion;
    proceso_blocked->un_pcb = peticion_pcb_interfaz->un_pcb;
    
    pthread_mutex_lock(peticion_pcb_interfaz->interfaz->mutex_cola_blocked);
    queue_push(peticion_pcb_interfaz->interfaz->cola_procesos_blocked, proceso_blocked);
    pthread_mutex_unlock(peticion_pcb_interfaz->interfaz->mutex_cola_blocked);

    sem_post(peticion_pcb_interfaz->interfaz->semaforo_cola_procesos_blocked);

    eliminar_peticion(peticion_pcb_interfaz->peticion);
}


void enviar_peticion_a_interfaz(t_proceso_blocked* proceso_blocked, t_interfaz* interfaz){ 
      t_paquete* paquete = crear_paquete(ATENDER_PETICION_INTERFAZ_KERNEL);
      agregar_string_a_paquete(paquete, proceso_blocked->peticion->instruccion);
      agregar_algo_a_paquete(paquete, proceso_blocked->peticion->parametros,sizeof(proceso_blocked->peticion->parametros)); //revisar si hay que enviar parametro por parametro
      int bytes = paquete->buffer->size + sizeof(op_code) + sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);
      int err = send(fd_entradasalida, a_enviar, bytes, SIGPIPE);
      if(err == -1){
        close(interfaz->fd_interfaz);
        interfaz->esta_conectada = false;

        //enviar_proceso_blocked_a_exit(interfaz->cola_procesos_blocked);
        //matar al hilo en el que me encuentro?
    }
	free(a_enviar);
      eliminar_paquete(paquete);
} 

//void enviar_proceso_blocked_a_exit(t_queue* cola_procesos_blocked){
//      pthread_mutex_lock(interfaz->mutex_cola_blocked);
//      t_pcb* un_pcb = queue_pop(cola_procesos_blocked);    
//      pthread_mutex_unlock(interfaz->mutex_cola_blocked);
//
//      int estado_anterior = un_pcb->estado_pcb;
//
//      cambiar_estado(un_pcb,EXIT);
//      un_pcb->estado_pcb = EXIT;
//
//      pthread_mutex_lock(mutex_exit);
//      queue_push(cola_exit,un_pcb);
//      pthread_mutex_unlock(mutex_exit);
//      
//      log_info(kernel_logger, "PID: <%d> - Estado Anterior: <%s> - Estado Actual: <%s>",un_pcb->pid, enum_a_string(estado_anterior),enum_a_string(un_pcb->estado_pcb));
//
//      // LIBERAR ESTRUCTURAS EN MEMORIA
//}

void recibir_fin_peticion(t_interfaz* interfaz){
    bool fin_peticion;
    int err = recv(interfaz->fd_interfaz, &fin_peticion, sizeof(bool), MSG_WAITALL);
    if(err == 0){
        close(interfaz->fd_interfaz);
        interfaz->esta_conectada = false;

        log_error(kernel_logger,"La interfaz %s se ha desconectado repentinamente. Proceso enviado a READY.\n", interfaz->nombre);
      }
}



void desbloquear_proceso(t_interfaz* interfaz){
    pthread_mutex_lock(interfaz->mutex_cola_blocked);
    t_pcb* un_pcb = queue_pop(interfaz->cola_procesos_blocked);    
    pthread_mutex_unlock(interfaz->mutex_cola_blocked);

      estado_pcb estado_anterior = un_pcb->estado_pcb;

     if(tiempo_transcurrido < un_pcb->quantum){
       enviar_proceso_blocked_a_ready_plus(un_pcb);
    } else{
      enviar_proceso_blocked_a_ready(un_pcb);

    }
      log_info(kernel_logger, "PID: <%d> - Estado Anterior: <%s> - Estado Actual: <%s>",un_pcb->pid, enum_a_string(estado_anterior),enum_a_string(un_pcb->estado_pcb));

}

void enviar_proceso_blocked_a_ready(t_pcb* un_pcb){
      cambiar_estado(un_pcb,READY);
      un_pcb->estado_pcb = READY;

      pthread_mutex_lock(mutex_ready);
      queue_push(cola_ready,un_pcb); 
      pthread_mutex_unlock(mutex_ready);
}

void enviar_proceso_blocked_a_ready_plus(t_pcb* un_pcb){
      un_pcb->quantum = un_pcb->quantum - tiempo_transcurrido;
      un_pcb->estado_pcb = READYPLUS;
      pthread_mutex_lock(mutex_ready_plus);
      queue_push(cola_ready_plus,un_pcb); 
      pthread_mutex_unlock(mutex_ready_plus);
      // LIBERAR ESTRUCTURAS EN MEMORIA
}

void eliminar_peticion(t_peticion* peticion){
      free(peticion->instruccion);
      free(peticion->interfaz);
      free(peticion->parametros->archivo);
      free(peticion->parametros->registroDireccion);
      free(peticion->parametros->registroTamanio);
      free(peticion->parametros->registroPunteroArchivo);
      free(peticion->parametros);
      free(peticion);
}
