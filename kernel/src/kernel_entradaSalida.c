#include "../includes/kernel_entradaSalida.h"
#include <../src/utils/utils_connections.h>

void atender_kernel_entradaSalida(){
    bool control_key = 1;
    while (control_key) {
        int cod_op = recibir_operacion(fd_entradasalida);
        t_paquete* paquete = recibir_paquete(fd_entradasalida);
        void* buffer = paquete->buffer;
        switch (cod_op)
        {
        case ME_IDENTIFICO:
                t_interfaz* interfaz = identificar_io(buffer);
                agregar_io(interfaz);
                gestionar_procesos_io(interfaz);
            break;
        case ESTOY_CONECTADO:
                char* instruccion = malloc(sizeof(char));
                t_list* instrucciones_posibles;
                instrucciones_posibles = list_create();

                t_pcb* pcb = recibir_pcb(paquete);
                leer_algo_del_stream(buffer, instrucciones_posibles, sizeof(t_list));
                instruccion = leer_string_del_stream(buffer);

                validar_interfaz_admite_instruccion(pcb, instrucciones_posibles, instruccion); 
            break;
        case FIN_PETICION:
                t_pcb* un_pcb = recibir_pcb(paquete);
                desbloquear_proceso(un_pcb);
                
            break;
        case -1:
          //  log_error(logger, "Desconexion de MEMORIA ");      
            control_key = 0;
        default:
          //  log_warning(logger, "Operacion desconocida de MEMORIA");
            break;
        }
    eliminar_paquete(paquete);    
    }
}

// ------------------------------ ME IDENTIFICO --------------------------------------

t_interfaz* identificar_io(t_buffer* buffer){
                t_interfaz* interfaz = malloc(sizeof(t_interfaz));
                interfaz->nombre = leer_string_del_stream(buffer);
                interfaz->tipo = leer_string_del_stream(buffer);
                interfaz->instrucciones_posibles = instrucciones_que_acepta(interfaz->tipo);
                interfaz->fd_interfaz = fd_entradasalida;
                interfaz->cola_procesos_blocked = queue_create();
                sem_init(interfaz->semaforo_cola_procesos_blocked, 0, 0); 
                pthread_mutex_t* mutex_cola_blocked = malloc(sizeof(pthread_mutex_t));
                pthread_mutex_init(mutex_cola_blocked, NULL);
                interfaz->mutex_cola_blocked = mutex_cola_blocked;
                return interfaz;
                }

t_list* instrucciones_que_acepta(char* TIPO_INTERFAZ){
    t_list* instrucciones_posibles;

           if (strcmp(TIPO_INTERFAZ, "GEN") == 0) {
			instrucciones_posibles = INSTRUCCIONES_GEN;
    } else if (strcmp(TIPO_INTERFAZ, "STDIN") == 0) {
			instrucciones_posibles = INSTRUCCIONES_STDIN; 
    } else if (strcmp(TIPO_INTERFAZ, "STDOUT") == 0) {
			instrucciones_posibles = INSTRUCCIONES_STDOUT;   
	} else if (strcmp(TIPO_INTERFAZ, "FS") == 0) {
			instrucciones_posibles = INSTRUCCIONES_FS;
		}
    return instrucciones_posibles;
}               

void agregar_io(t_interfaz* interfaz){
  pthread_mutex_lock(mutex_io);
  list_add(IOS_CONECTADOS, interfaz);
  pthread_mutex_unlock(mutex_io);
}


void gestionar_procesos_io(t_interfaz* interfaz){
  while(1){
    sem_wait(interfaz->semaforo_cola_procesos_blocked);

    pthread_mutex_lock(interfaz->mutex_cola_blocked);
    t_proceso_blocked* proceso_a_ejecutar = queue_pop(interfaz->cola_procesos_blocked);
    pthread_mutex_unlock(interfaz->mutex_cola_blocked);
    
    enviar_peticion_a_interfaz(proceso_a_ejecutar->peticion, interfaz, proceso_a_ejecutar->un_pcb);
  }
}

//------------------------------------ PETICION IO ------------------------------------------------------

void atender_pedido_io(t_paquete_y_pcb* paquete_y_pcb){
      t_paquete* paquete = paquete_y_pcb->paquete; 
      t_pcb* pcb_recibido = paquete_y_pcb->un_pcb; 

      t_peticion* peticion = recibir_peticion(paquete);  
      t_interfaz* interfaz = validar_peticion(peticion, pcb_recibido);

      t_proceso_blocked* proceso_a_ejecutar = malloc(sizeof(t_proceso_blocked));
      proceso_a_ejecutar->un_pcb = pcb_recibido;
      proceso_a_ejecutar->peticion = peticion;

      enviar_proceso_a_blocked(proceso_a_ejecutar, interfaz);

      eliminar_peticion(peticion);
      free(proceso_a_ejecutar);
}


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

    t_peticion_param* parametros = malloc(sizeof(t_peticion_param));
    void* buffer = paquete->buffer;

    if(strcmp(instruccion,"IO_GEN_SLEEP") == 0){
        leer_algo_del_stream(buffer, &parametros->tiempo_espera,sizeof(parametros->tiempo_espera));
        return parametros;
      }else if (strcmp(instruccion,"IO_STDIN_READ") == 0)
      {        
        parametros->registroDireccion = leer_string_del_stream(buffer);
        parametros->registroTamanio = leer_string_del_stream(buffer);
        return parametros;
      }else if (strcmp(instruccion,"IO_STDOUT_WRITE") == 0)
      {
        parametros->registroDireccion = leer_string_del_stream(buffer);
        parametros->registroTamanio = leer_string_del_stream(buffer);
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

void enviar_proceso_a_blocked(t_proceso_blocked* proceso_a_ejecutar, t_interfaz* interfaz)
{
    proceso_a_ejecutar->un_pcb->estado_pcb = BLOCKED;
    
    pthread_mutex_lock(interfaz->mutex_cola_blocked);
    queue_push(interfaz->cola_procesos_blocked, proceso_a_ejecutar->un_pcb);
    pthread_mutex_unlock(interfaz->mutex_cola_blocked);

    sem_post(interfaz->semaforo_cola_procesos_blocked);
}

// -------------------------------- VALIDAR PEDIDO IO -----------------------------------------------------

t_interfaz* validar_peticion(t_peticion* peticion, t_pcb* pcb){ 
      char* nombre_io = peticion->interfaz;
      char* instruccion = peticion->instruccion;

    t_interfaz* interfaz = existe_la_interfaz(nombre_io, pcb);
    esta_conectada_la_interfaz(pcb, interfaz, instruccion);
    
    return interfaz;
}
t_interfaz* existe_la_interfaz(char* nombre_io, t_pcb* pcb){
    
    bool esta_la_io(void* io){
        bool respuesta;
        t_interfaz* interfaz = io;
        respuesta = (strcmp(nombre_io, interfaz->nombre) == 0);
        return respuesta;
    }

    t_interfaz* interfaz = malloc(sizeof(t_interfaz));
    interfaz = list_find(IOS_CONECTADOS, esta_la_io); 

    if(interfaz != NULL){ //Es correcta esta condicion?
        return interfaz;
    }else{
            log_error(kernel_logger,"No se ha encontrado a la interfaz: %s. Proceso enviado a EXIT.\n", interfaz->nombre);
            cambiar_estado(pcb, EXIT);
            //matar al hilo en el que me encuentro?
          }
}         

void esta_conectada_la_interfaz(t_pcb* un_pcb, t_interfaz* interfaz, char* instruccion){
    t_paquete* paquete = crear_paquete(ESTOY_CONECTADO); 

	agregar_algo_a_paquete(paquete, &(un_pcb->pid));
    agregar_algo_a_paquete(paquete,&(un_pcb->quantum));
    agregar_registro_a_paquete(paquete, un_pcb->registros_cpu);
	agregar_algo_a_paquete(paquete,&(un_pcb->estado_pcb));

    agregar_algo_a_paquete(paquete, &(interfaz->instrucciones_posibles));
    agregar_algo_a_paquete(paquete, instruccion);

    int bytes = paquete->buffer->size + sizeof(op_code) + sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);
	int err = send(fd_entradasalida, a_enviar, bytes, SIGPIPE);
	free(a_enviar);      
    eliminar_paquete(paquete);

    if(err == -1){
        close(interfaz->fd_interfaz);

        pthread_mutex_lock(mutex_io);
        list_remove_element(IOS_CONECTADOS, interfaz);
        pthread_mutex_unlock(mutex_io);

        log_error(kernel_logger,"La interfaz %s no se encuentra conectada. Proceso enviado a EXIT.\n", interfaz->nombre);
        cambiar_estado(un_pcb, EXIT);
        //matar al hilo en el que me encuentro?
    }
}

void validar_interfaz_admite_instruccion(t_pcb* un_pcb, t_list* instrucciones_posibles, char* instruccion){

      if((contains_string(instrucciones_posibles, instruccion))){ 
            log_error(kernel_logger,"Interfaz: No reconozco esta instruccion. Proceso enviado a EXIT.\n");
            cambiar_estado(un_pcb, EXIT);
            //matar al hilo en el que me encuentro?
      }
}

//-------------------------------- ENVIO LA PETICION ---------------------------------------------------------

void enviar_peticion_a_interfaz(t_peticion* peticion, t_interfaz* interfaz, t_pcb* pcb){ 
      t_paquete* paquete = crear_paquete(ATENDER_PETICION_INTERFAZ_KERNEL);
      agregar_string_a_paquete(paquete, peticion->instruccion);
      agregar_algo_a_paquete(paquete, peticion->parametros);
      enviar_paquete(paquete, interfaz->fd_interfaz);
      eliminar_paquete(paquete);
} 


void eliminar_peticion(t_peticion* peticion){
      free(peticion->instruccion);
      free(peticion->interfaz);
      free(peticion->parametros->archivo);
      free(peticion->parametros->registroDireccion);
      free(peticion->parametros->registroPunteroArchivo);
      free(peticion->parametros->registroTamanio);
      free(peticion->parametros);
      free(peticion);
}

// ------------------------------ FIN PETICION -------------------------------------------------------------

void desbloquear_proceso(t_pcb* un_pcb){
    cambiar_estado(un_pcb, READY);
}
