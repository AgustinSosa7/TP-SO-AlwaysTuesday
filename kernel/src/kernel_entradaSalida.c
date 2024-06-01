#include "../includes/kernel_entradaSalida.h"
#include <../src/utils/utils_connections.h>

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

void enviar_proceso_a_blocked(t_proceso_blocked* proceso_a_ejecutar, t_interfaz* interfaz)
{
    proceso_a_ejecutar->un_pcb->estado_pcb = BLOCKED;
    
    pthread_mutex_lock(interfaz->mutex_cola_blocked);
    queue_push(interfaz->cola_procesos_blocked, proceso_a_ejecutar->un_pcb);
    pthread_mutex_unlock(interfaz->mutex_cola_blocked);

    sem_post(interfaz->semaforo_cola_procesos_blocked);
}



t_interfaz* validar_peticion(t_peticion* peticion, t_pcb* pcb){ 
      char* nombre_io = peticion->interfaz;
      char* instruccion = peticion->instruccion;

    t_interfaz* interfaz = existe_la_interfaz(nombre_io, pcb);
    if(esta_conectada_la_interfaz(interfaz, pcb)){
        validar_interfaz_admite_instruccion(nombre_io, instruccion, pcb); 
    }else{
        //matar al hilo en el que me encuentro?
    }
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

bool esta_conectada_la_interfaz(t_interfaz* interfaz, t_pcb* pcb){
    t_paquete* paquete = crear_paquete(ESTOY_CONECTADO); 
    bool mensaje = true;
    agregar_algo_a_paquete(paquete, &mensaje);
    int bytes = sizeof(uint32_t) * 2 + sizeof(bool);
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
        cambiar_estado(pcb, EXIT);
        return false;
    }else{
        bool respuesta = recibir_mensaje(fd_entradasalida);
        return respuesta;
    }

}

void validar_interfaz_admite_instruccion(char* interfaz, char* instruccion, t_pcb* un_pcb){
      t_paquete* paquete = crear_paquete(RECONOCER_INSTRUCCION);
      agregar_string_a_paquete(paquete, instruccion);
      enviar_paquete(paquete, fd_entradasalida);
      eliminar_paquete(paquete);

      bool acepta_la_instruccion = recibir_mensaje(fd_entradasalida);
      if(acepta_la_instruccion){

      }else{
            log_error(kernel_logger,"Interfaz %s: No reconozco esta instruccion. Proceso enviado a EXIT.\n", interfaz);
            cambiar_estado(un_pcb, EXIT);
            //matar al hilo en el que me encuentro?
      }
}

void enviar_peticion_a_interfaz(t_peticion* peticion, t_interfaz* interfaz){ 
      t_paquete* paquete = crear_paquete(ATENDER_PETICION_INTERFAZ_KERNEL);
      agregar_string_a_paquete(paquete, peticion->instruccion);
      agregar_algo_a_paquete(paquete, peticion->parametros);
      enviar_paquete(paquete, interfaz->fd_interfaz);
      eliminar_paquete(paquete);
} 

void recibir_fin_peticion(){
    bool fin_peticion;
    recv(fd_entradasalida, &fin_peticion, sizeof(bool), MSG_WAITALL);
}


void eliminar_peticion(t_peticion* peticion){
      free(peticion->instruccion);
      free(peticion->interfaz);
      free(peticion->parametros->archivo);
      free(peticion->parametros->registro1);
      free(peticion->parametros->registro2);
      free(peticion->parametros->registro3);
      free(peticion->parametros);
      free(peticion);
}