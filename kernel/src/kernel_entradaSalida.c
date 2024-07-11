#include "../includes/kernel_entradaSalida.h"

t_peticion* recibir_peticion(t_paquete* paquete){ 
    t_peticion* peticion = malloc(sizeof(t_peticion));
    void* buffer = paquete->buffer;

    peticion->instruccion = leer_string_del_buffer(buffer);
    peticion->interfaz = leer_string_del_buffer(buffer);
    peticion->parametros = leer_parametros(paquete, peticion->instruccion);

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
           leer_parametros_lista_de_accesos(parametros, buffer);
           return parametros;
      }else if (strcmp(instruccion,"IO_STDOUT_WRITE") == 0)
      {
           leer_parametros_lista_de_accesos(parametros, buffer);
           return parametros;
      }else if (strcmp(instruccion,"IO_FS_CREATE") == 0)
      {
            parametros->archivo= leer_string_del_buffer(buffer);
            return parametros;
      }else if (strcmp(instruccion,"IO_FS_DELETE") == 0)
      {
            parametros->archivo= leer_string_del_buffer(buffer);
            return parametros;
      }else if (strcmp(instruccion,"IO_FS_TRUNCATE") == 0)
      {
            parametros->archivo= leer_string_del_buffer(buffer);
            parametros->registroTamanio = leer_int_del_buffer(buffer);
            return parametros;
      }else if (strcmp(instruccion,"IO_FS_WRITE") == 0)
      {
            parametros->archivo= leer_string_del_buffer(buffer);
            leer_parametros_lista_de_accesos(parametros, buffer);
            parametros->registroTamanio = leer_int_del_buffer(buffer);
            parametros->registroPunteroArchivo= leer_int_del_buffer(buffer);
            return parametros;
      }else //DEFALUT IO_FS_READ
      {
            parametros->archivo= leer_string_del_buffer(buffer);
            leer_parametros_lista_de_accesos(parametros, buffer);
            parametros->registroTamanio = leer_int_del_buffer(buffer);
            parametros->registroPunteroArchivo= leer_int_del_buffer(buffer);
            return parametros;
      }
}           

t_interfaz* validar_peticion(t_peticion* peticion){ 
      char* nombre_io = peticion->interfaz;
      char* instruccion = peticion->instruccion;

      t_interfaz* interfaz = existe_la_interfaz(nombre_io); 
      if (interfaz == NULL) {
            enviar_proceso_execute_a_exit();
            eliminar_peticion(peticion);
            log_error(kernel_logger, "No se ha encontrado la interfaz: %s.\n", nombre_io);
            return NULL;
      } else if(!(interfaz->esta_conectada)){
            enviar_proceso_execute_a_exit();
            eliminar_peticion(peticion);
            log_error(kernel_logger,"La interfaz %s no se encuentra conectada.\n", interfaz->nombre);
            return NULL;
      } else if(!(validar_interfaz_admite_instruccion(interfaz, instruccion))){
            enviar_proceso_execute_a_exit();
            eliminar_peticion(peticion);
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


void eliminar_interfaz(t_interfaz* interfaz){
      pthread_mutex_lock(&mutex_io);
      list_remove_element(IOS_CONECTADOS, interfaz);
      pthread_mutex_unlock(&mutex_io);

      free(interfaz->nombre);
      free(interfaz->tipo);
      sem_destroy(&(interfaz->semaforo_cola_procesos_blocked));
      pthread_mutex_destroy(&(interfaz->mutex_cola_blocked));
      free(interfaz);
}


bool validar_interfaz_admite_instruccion(t_interfaz* interfaz, char* instruccion){
      return (contains_string(interfaz->instrucciones_posibles, instruccion));
}


void enviar_proceso_execute_a_exit(){ 
      pthread_mutex_lock(&(struct_exec->mutex));
      t_pcb* pcb = list_get(struct_exec->lista, 0); //Revisar LOGICA (mili)
      pthread_mutex_unlock(&(struct_exec->mutex));
      cambiar_de_estado_y_de_lista(EXEC, EXIT);    

      eliminar_proceso(pcb, INVALID_INTERFACE);
      
}

void enviar_proceso_a_blocked(t_peticion* peticion, t_pcb* pcb, t_interfaz* interfaz)
{    
    printf("Enviando el proceso a blocked...\n"); 
    pthread_mutex_lock(&(struct_exec->mutex));
    t_pcb* un_pcb = list_remove(struct_exec->lista,0);
    pthread_mutex_unlock(&(struct_exec->mutex));
    eliminar_pcb(un_pcb);
    estado_pcb estado_anterior = pcb->estado_pcb;
    pcb->estado_pcb = BLOCKED;

    t_proceso_blocked* proceso_blocked = malloc(sizeof(t_proceso_blocked));
 
    proceso_blocked->peticion = peticion;
    proceso_blocked->un_pcb = pcb;
    
    pthread_mutex_lock(&(interfaz->mutex_cola_blocked));
    queue_push(interfaz->cola_procesos_blocked, proceso_blocked);
    pthread_mutex_unlock(&(interfaz->mutex_cola_blocked));

    sem_post(&(interfaz->semaforo_cola_procesos_blocked));

    log_warning(kernel_logger,"Cambio de Estado: PID: <%d> - Estado Anterior: <%s> - Estado Actual: <%s>", pcb->pid, enum_a_string(estado_anterior), enum_a_string(pcb->estado_pcb));
    log_info(kernel_logger, "Motivo de Bloqueo: PID: <%d> - Bloqueado por: <%s>", pcb->pid, interfaz->nombre); 
}


bool enviar_peticion_a_interfaz(t_proceso_blocked* proceso_blocked, t_interfaz* interfaz){ 
      t_paquete* paquete = crear_paquete(ATENDER_PETICION_INTERFAZ_KERNEL);
      agregar_int_a_paquete(paquete, proceso_blocked->un_pcb->pid);
      agregar_string_a_paquete(paquete, proceso_blocked->peticion->instruccion); 
      agregar_parametros_a_paquete(paquete, proceso_blocked->peticion);
      eliminar_peticion(proceso_blocked->peticion); 

      int bytes = paquete->buffer->size + sizeof(op_code) + sizeof(int); 
	void* a_enviar = serializar_paquete(paquete, bytes);
      int err = send(interfaz->fd_interfaz, a_enviar, bytes, MSG_NOSIGNAL); 
      printf("Instruccion enviada...\n");

      if(err == -1){
        close(interfaz->fd_interfaz);
        interfaz->esta_conectada = false;
        enviar_cola_de_procesos_blocked_io_a_exit(interfaz);
        eliminar_interfaz(interfaz);
      return true; //salgo del while de "gestionar_procesos_io"
    } else{
	free(a_enviar);
      eliminar_paquete(paquete);
      return false;
   }   
} 

void agregar_parametros_a_paquete(t_paquete* paquete, t_peticion* peticion){
      char* instruccion = peticion->instruccion;

            if(strcmp(instruccion,"IO_GEN_SLEEP") == 0){
            agregar_int_a_paquete(paquete, peticion->parametros->tiempo_espera);

      }else if (strcmp(instruccion,"IO_STDIN_READ") == 0)
      {     agregar_lista_de_accesos_a_paquete(paquete, peticion->parametros->lista_de_accesos);

      }else if (strcmp(instruccion,"IO_STDOUT_WRITE") == 0)
      {     agregar_lista_de_accesos_a_paquete(paquete, peticion->parametros->lista_de_accesos);

      }else if (strcmp(instruccion,"IO_FS_CREATE") == 0)
      //Ver ACá que onda :D
      {     agregar_string_a_paquete(paquete, peticion->parametros->archivo);
            free(peticion->parametros->archivo);

      }else if (strcmp(instruccion,"IO_FS_DELETE") == 0)
      {     agregar_string_a_paquete(paquete, peticion->parametros->archivo);
            free(peticion->parametros->archivo);

      }else if (strcmp(instruccion,"IO_FS_TRUNCATE") == 0)
      {     agregar_string_a_paquete(paquete, peticion->parametros->archivo);
            agregar_int_a_paquete(paquete, peticion->parametros->registroTamanio); 
            free(peticion->parametros->archivo);

      }else if (strcmp(instruccion,"IO_FS_WRITE") == 0)
      {     agregar_string_a_paquete(paquete, peticion->parametros->archivo);
            agregar_lista_de_accesos_a_paquete(paquete, peticion->parametros->lista_de_accesos);
            agregar_int_a_paquete(paquete,peticion->parametros->registroTamanio);
            agregar_int_a_paquete(paquete, peticion->parametros->registroPunteroArchivo);
            free(peticion->parametros->archivo);

      }else //Es IO_FS_READ 
      {     agregar_string_a_paquete(paquete, peticion->parametros->archivo);
            agregar_lista_de_accesos_a_paquete(paquete, peticion->parametros->lista_de_accesos);
            agregar_int_a_paquete(paquete,peticion->parametros->registroTamanio);
            agregar_int_a_paquete(paquete, peticion->parametros->registroPunteroArchivo);  
            free(peticion->parametros->archivo);
      }       
}


void enviar_cola_de_procesos_blocked_io_a_exit(t_interfaz* interfaz){

      void enviar_proceso_de_blocked_io_a_exit(void* proceso){
            return enviar_proceso_blocked_io_a_exit(proceso);
      }
      printf("Cantidad de procesos en blocked: %d.\n", queue_size(interfaz->cola_procesos_blocked));

      queue_destroy_and_destroy_elements(interfaz->cola_procesos_blocked, enviar_proceso_de_blocked_io_a_exit);

      log_error(kernel_logger,"La interfaz %s se ha desconectado repentinamente. Procesos en cola de BLOCKED enviados a EXIT.\n", interfaz->nombre);
}


void enviar_proceso_blocked_io_a_exit(t_proceso_blocked* proceso_blocked){
      
      eliminar_peticion(proceso_blocked->peticion);

      t_pcb* pcb = proceso_blocked->un_pcb;

      estado_pcb estado_anterior = pcb->estado_pcb;

      pcb->estado_pcb = EXIT;  

      eliminar_proceso(pcb, INVALID_INTERFACE);

      log_warning(kernel_logger,"Cambio de Estado: PID: <%d> - Estado Anterior: <%s> - Estado Actual: <%s>",pcb->pid, enum_a_string(estado_anterior),enum_a_string(pcb->estado_pcb));
      pthread_mutex_lock(&(struct_exit->mutex));
      list_add(struct_exit->lista, pcb);
      pthread_mutex_unlock(&(struct_exit->mutex));
      
}

bool recibir_fin_peticion(t_interfaz* interfaz){
    bool fin_peticion;
    int err = recv(interfaz->fd_interfaz, &fin_peticion, sizeof(bool), MSG_WAITALL);
    if(err == 0){
        close(interfaz->fd_interfaz);
        interfaz->esta_conectada = false;
        enviar_cola_de_procesos_blocked_io_a_exit(interfaz);
        eliminar_interfaz(interfaz);
        return true;
      } else { return false;}
}


void desbloquear_proceso(t_interfaz* interfaz){
      
      pthread_mutex_lock(&(interfaz->mutex_cola_blocked));
      t_proceso_blocked* proceso_blocked = queue_pop(interfaz->cola_procesos_blocked);    
      pthread_mutex_unlock(&(interfaz->mutex_cola_blocked));

      estado_pcb estado_anterior = proceso_blocked->un_pcb->estado_pcb;
      enviar_proceso_a_ready_o_ready_plus(proceso_blocked->un_pcb);
     
      log_warning(kernel_logger,"Cambio de Estado: PID: <%d> - Estado Anterior: <%s> - Estado Actual: <%s>",proceso_blocked->un_pcb->pid, enum_a_string(estado_anterior),enum_a_string(proceso_blocked->un_pcb->estado_pcb));
      free(proceso_blocked);
      sem_post(&sem_planificador_corto_plazo); 
      
}

void enviar_proceso_a_ready_o_ready_plus(t_pcb* un_pcb){
      if(strcmp(ALGORITMO_PLANIFICACION,"VRR") == 0){
            if(un_pcb->tiempo_transcurrido < un_pcb->quantum){
                  enviar_proceso_blocked_a_ready_plus(un_pcb);
            }else {
                  un_pcb->tiempo_transcurrido = 0;
                  enviar_proceso_blocked_a_ready(un_pcb);
                  }
      } else{ 
            un_pcb->tiempo_transcurrido = 0;
            enviar_proceso_blocked_a_ready(un_pcb);
            
            }
}

void enviar_proceso_blocked_a_ready_plus(t_pcb* un_pcb){
      un_pcb->quantum = un_pcb->quantum - un_pcb->tiempo_transcurrido;
      un_pcb->estado_pcb = READYPLUS;

      pthread_mutex_lock(&(struct_ready_plus->mutex));
      list_add(struct_ready_plus->lista,un_pcb); 
      pthread_mutex_unlock(&(struct_ready_plus->mutex));
}

void eliminar_peticion(t_peticion* peticion){
      //eliminar_parametros_segun_instruccion(peticion->instruccion, peticion->parametros);
      free(peticion->instruccion);
      free(peticion->interfaz);
      free(peticion);
}

void eliminar_parametros_segun_instruccion(char* instruccion, t_peticion_param* parametros){
      if(strcmp(instruccion,"IO_GEN_SLEEP") == 0){

      }else if (strcmp(instruccion,"IO_STDIN_READ") == 0)
      {    

      }else if (strcmp(instruccion,"IO_STDOUT_WRITE") == 0)
      {     

      }else if (strcmp(instruccion,"IO_FS_CREATE") == 0)
      {    

      }else if (strcmp(instruccion,"IO_FS_DELETE") == 0)
      {     

      }else if (strcmp(instruccion,"IO_FS_TRUNCATE") == 0)
      {     

      }else if (strcmp(instruccion,"IO_FS_WRITE") == 0)
      {     

      }else //Es IO_FS_READ 
      {    
      }     
      
      free(parametros);
}