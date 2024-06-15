#include "../includes/kernel_entradaSalida.h"

t_peticion* recibir_peticion(t_paquete* paquete){ 
    t_peticion* peticion = malloc(sizeof(t_peticion));
    void* buffer = paquete->buffer;
    //peticion->instruccion = malloc(sizeof(char));
    //peticion->interfaz = malloc(sizeof(char));
    //peticion->parametros = malloc(sizeof(t_peticion_param));


    peticion->instruccion = leer_string_del_buffer(buffer);
    printf("Instruccion: %s.\n", peticion->instruccion);
    peticion->interfaz = leer_string_del_buffer(buffer);
    printf("Interfaz: %s.\n", peticion->interfaz);
    peticion->parametros = leer_parametros(paquete, peticion->instruccion);
    printf("Tiempo de espera: %d.\n", peticion->parametros->tiempo_espera);

    return peticion;
} 

t_peticion_param* leer_parametros(t_paquete* paquete, char* instruccion){

    t_peticion_param* parametros = malloc(sizeof(t_peticion_param));
    t_buffer* buffer = paquete->buffer;

    if(strcmp(instruccion,"IO_GEN_SLEEP") == 0){
           parametros->tiempo_espera= leer_int_del_buffer(buffer);
           return parametros;
      }
      else if (strcmp(instruccion,"IO_STDIN_READ") == 0)
      {
           parametros->registroDireccion= leer_string_del_buffer(buffer);
           parametros->registroTamanio= leer_string_del_buffer(buffer);
           return parametros;
      }else if (strcmp(instruccion,"IO_STDOUT_WRITE") == 0)
      {
           parametros->registroDireccion= leer_string_del_buffer(buffer);
           parametros->registroTamanio= leer_string_del_buffer(buffer);
           return parametros;
      }else if (strcmp(instruccion,"IO_FS_CREATE") == 0)
      {
            /* code */
            return parametros;
      }else if (strcmp(instruccion,"IO_FS_DELETE") == 0)
      {
            /* code */
            return parametros;
      }else if (strcmp(instruccion,"IO_FS_TRUNCATE") == 0)
      {
            /* code */
            return parametros;
      }else if (strcmp(instruccion,"IO_FS_WRITE") == 0)
      {
            /* code */
            return parametros;
      }else //DEFALUT IO_FS_READ
      {
            /* code */
            return parametros;
      }
}           

t_interfaz* validar_peticion(t_peticion* peticion, t_pcb* pcb){ 
      char* nombre_io = peticion->interfaz;
      char* instruccion = peticion->instruccion;

      t_interfaz* interfaz = existe_la_interfaz(nombre_io); 
      if (interfaz == NULL) {
            enviar_proceso_a_exit(pcb);
            log_error(kernel_logger, "No se ha encontrado la interfaz: %s.\n", nombre_io);
            return NULL;
      } else if(!(interfaz->esta_conectada)){
            enviar_proceso_a_exit(pcb);
            log_error(kernel_logger,"La interfaz %s no se encuentra conectada.\n", interfaz->nombre);
            return NULL;
      } else if(!(validar_interfaz_admite_instruccion(interfaz, instruccion))){
            enviar_proceso_a_exit(pcb);
            log_info(kernel_logger, "La interfaz %s no admite la instruccion %s.", interfaz->nombre, instruccion);
            return NULL;
      } else{
            return interfaz;
    }
} 


t_interfaz* existe_la_interfaz(char* nombre_io) {

    bool esta_la_io(void* io) {
      return esta_o_no(nombre_io, io);
    }

    return (list_find(IOS_CONECTADOS, esta_la_io)); 
}   


bool esta_o_no(char* nombre_io, t_interfaz* io){ 
        return (strcmp(nombre_io, io->nombre) == 0);
}


bool validar_interfaz_admite_instruccion(t_interfaz* interfaz, char* instruccion){
      return (contains_string(interfaz->instrucciones_posibles, instruccion));
}


void enviar_proceso_a_exit(t_pcb* un_pcb){ //Revisar (mili)

      cambiar_de_estado_y_de_lista(EXEC, EXIT);    // LIBERAR ESTRUCTURAS EN MEMORIA

      log_info(kernel_logger, "Finaliza el proceso <%d> - Motivo: <INVALID_INTERFACE>", un_pcb->pid);
}

void enviar_proceso_a_blocked(t_peticion* peticion, t_pcb* pcb, t_interfaz* interfaz)
{    
    estado_pcb estado_anterior = pcb->estado_pcb;
    pcb->estado_pcb = BLOCKED;

    t_proceso_blocked* proceso_blocked = malloc(sizeof(t_proceso_blocked));
 
    proceso_blocked->peticion = peticion;
    proceso_blocked->un_pcb = pcb;
    
    pthread_mutex_lock(&(interfaz->mutex_cola_blocked));
    queue_push(interfaz->cola_procesos_blocked, proceso_blocked);
    pthread_mutex_unlock(&(interfaz->mutex_cola_blocked));

    sem_post(&(interfaz->semaforo_cola_procesos_blocked));

    log_info(kernel_logger,"Cambio de Estado: PID: <%d> - Estado Anterior: <%s> - Estado Actual: <%s>", pcb->pid, enum_a_string(estado_anterior), enum_a_string(pcb->estado_pcb));
    log_info(kernel_logger, "Motivo de Bloqueo: PID: <%d> - Bloqueado por: <%s>", pcb->pid, interfaz->nombre);
}


void enviar_peticion_a_interfaz(t_proceso_blocked* proceso_blocked, t_interfaz* interfaz){ 
      t_paquete* paquete = crear_paquete(ATENDER_PETICION_INTERFAZ_KERNEL);
      printf("Instruccion a enviar: %s.\n",proceso_blocked->peticion->instruccion);
      agregar_string_a_paquete(paquete, proceso_blocked->peticion->instruccion); 
      agregar_parametros_a_paquete(paquete, proceso_blocked->peticion); 

      int bytes = paquete->buffer->size + sizeof(op_code) + sizeof(int); 
	void* a_enviar = serializar_paquete(paquete, bytes);
      int err = send(interfaz->fd_interfaz, a_enviar, bytes, SIGPIPE); 
      if(err == -1){
        close(interfaz->fd_interfaz);
        interfaz->esta_conectada = false;
        enviar_procesos_blocked_a_exit(interfaz->cola_procesos_blocked);
    }
	free(a_enviar);
      eliminar_paquete(paquete);
} 

void agregar_parametros_a_paquete(t_paquete* paquete, t_peticion* peticion){
      char* instruccion = peticion->instruccion;

            if(strcmp(instruccion,"IO_GEN_SLEEP") == 0){
            agregar_int_a_paquete(paquete, peticion->parametros->tiempo_espera);

      }else if (strcmp(instruccion,"IO_STDIN_READ") == 0)
      {     agregar_string_a_paquete(paquete, peticion->parametros->registroDireccion);
            agregar_string_a_paquete(paquete, peticion->parametros->registroTamanio);

      }else if (strcmp(instruccion,"IO_STDOUT_WRITE") == 0)
      {     agregar_string_a_paquete(paquete, peticion->parametros->registroDireccion);
            agregar_string_a_paquete(paquete, peticion->parametros->registroTamanio);

      }else if (strcmp(instruccion,"IO_FS_CREATE") == 0)
      {     agregar_string_a_paquete(paquete, peticion->parametros->archivo);

      }else if (strcmp(instruccion,"IO_FS_DELETE") == 0)
      {     agregar_string_a_paquete(paquete, peticion->parametros->archivo);

      }else if (strcmp(instruccion,"IO_FS_TRUNCATE") == 0)
      {     agregar_string_a_paquete(paquete, peticion->parametros->archivo);
            agregar_string_a_paquete(paquete, peticion->parametros->registroTamanio); 

      }else if (strcmp(instruccion,"IO_FS_WRITE") == 0)
      {     agregar_string_a_paquete(paquete, peticion->parametros->archivo);
            agregar_string_a_paquete(paquete, peticion->parametros->registroDireccion);
            agregar_string_a_paquete(paquete, peticion->parametros->registroTamanio);
            agregar_string_a_paquete(paquete, peticion->parametros->registroPunteroArchivo);

      }else //Es IO_FS_READ 
      {     agregar_string_a_paquete(paquete, peticion->parametros->archivo);
            agregar_string_a_paquete(paquete, peticion->parametros->registroDireccion);
            agregar_string_a_paquete(paquete, peticion->parametros->registroTamanio);
            agregar_string_a_paquete(paquete, peticion->parametros->registroPunteroArchivo);      
      }       
}



void enviar_procesos_blocked_a_exit(t_queue* cola_procesos_blocked){
      while(!(queue_is_empty(cola_procesos_blocked))){
            t_proceso_blocked* proceso = queue_pop(cola_procesos_blocked);
            enviar_proceso_a_exit(proceso->un_pcb);
            eliminar_peticion(proceso->peticion);
            free(proceso);
      }
}


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
      
     pthread_mutex_lock(&(interfaz->mutex_cola_blocked));
     t_proceso_blocked* proceso_blocked = queue_pop(interfaz->cola_procesos_blocked);    
     pthread_mutex_unlock(&(interfaz->mutex_cola_blocked));

     estado_pcb estado_anterior = proceso_blocked->un_pcb->estado_pcb;

     if(tiempo_transcurrido < proceso_blocked->un_pcb->quantum){ //REVISAR PQ SI ES FIFO NO LO QUIERO ENVIAR A READY PLUS
       enviar_proceso_blocked_a_ready_plus(proceso_blocked->un_pcb);
     } else{
       enviar_proceso_blocked_a_ready(proceso_blocked->un_pcb);
     }
      log_info(kernel_logger, "PID: <%d> - Estado Anterior: <%s> - Estado Actual: <%s>",proceso_blocked->un_pcb->pid, enum_a_string(estado_anterior),enum_a_string(proceso_blocked->un_pcb->estado_pcb));
}

void enviar_proceso_blocked_a_ready(t_pcb* un_pcb){
      un_pcb->estado_pcb = READY;

      pthread_mutex_lock(&mutex_ready);
      list_add(lista_ready,un_pcb); 
      pthread_mutex_unlock(&mutex_ready);
}

void enviar_proceso_blocked_a_ready_plus(t_pcb* un_pcb){
      un_pcb->quantum = un_pcb->quantum - tiempo_transcurrido;
      un_pcb->estado_pcb = READYPLUS;

      pthread_mutex_lock(&mutex_ready_plus);
      list_add(lista_ready_plus,un_pcb); 
      pthread_mutex_unlock(&mutex_ready_plus);
}

void eliminar_peticion(t_peticion* peticion){
      eliminar_parametros_segun_instruccion(peticion->instruccion, peticion->parametros);
      free(peticion->instruccion);
      free(peticion->interfaz);
      free(peticion);
}

void eliminar_parametros_segun_instruccion(char* instruccion, t_peticion_param* parametros){
      if(strcmp(instruccion,"IO_GEN_SLEEP") == 0){

      }else if (strcmp(instruccion,"IO_STDIN_READ") == 0)
      {     free(parametros->registroDireccion);
            free(parametros->registroTamanio);

      }else if (strcmp(instruccion,"IO_STDOUT_WRITE") == 0)
      {     free(parametros->registroDireccion);
            free(parametros->registroTamanio);

      }else if (strcmp(instruccion,"IO_FS_CREATE") == 0)
      {     free(parametros->archivo);

      }else if (strcmp(instruccion,"IO_FS_DELETE") == 0)
      {     free(parametros->archivo);

      }else if (strcmp(instruccion,"IO_FS_TRUNCATE") == 0)
      {     free(parametros->archivo);
            free(parametros->registroTamanio);

      }else if (strcmp(instruccion,"IO_FS_WRITE") == 0)
      {     free(parametros->archivo);
            free(parametros->registroDireccion);
            free(parametros->registroTamanio);
            free(parametros->registroPunteroArchivo);

      }else //Es IO_FS_READ 
      {     free(parametros->archivo);
            free(parametros->registroDireccion);
            free(parametros->registroTamanio);
            free(parametros->registroPunteroArchivo);
      }     
      
      free(parametros);
}