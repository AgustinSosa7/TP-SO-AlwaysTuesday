#include "../includes/kernel_cpu_dispatch.h"

void recibir_pcb_con_motivo()
{     int control_key = 1;
      while (control_key){
      int code_op = recibir_operacion(fd_cpu_dispatch);
      t_paquete* paquete = recibir_paquete(fd_cpu_dispatch);
      t_pcb* pcb_recibido = recibir_pcb(paquete);
      log_info(kernel_logger, "Se recibio algo de CPU_Dispatch : %d", code_op);
      switch (code_op)
      {
      case DESALOJO_QUANTUM:
            log_info(kernel_logger,"PID: <%d> - Desalojado por fin de Quantum",pcb->pid);
            cambiar_estado(pcb_recibido, READY);
            
            break;
      case PROCESO_EXIT:
            /* code */
            break;
      case PEDIDO_IO:               
            //pthread_t pedido_io;
            //pthread_create(&pedido_io, NULL, atender_pedido_io,(paquete,pcb_recibido)); //verificar como se envian estos parametros
            //pthread_detach(pedido_io);
            atender_pedido_io(paquete,pcb_recibido);
            break;
      case -1:
            log_error(kernel_logger, "Desconexion de CPU - DISPATCH");      
            control_key = 0;
            break;
      default:
            log_warning(kernel_logger, "Operacion desconocida de CPU - DISPATCH");
            break;
        }
        eliminar_paquete(paquete);    
      }
}


t_peticion* recibir_peticion(t_paquete* paquete){
    t_peticion* peticion = malloc(sizeof(t_peticion));
    void* stream = paquete->buffer->stream;
    peticion->instruccion = malloc(sizeof(char));
    peticion->instruccion = malloc(sizeof(char));

    //leer_string_del_stream(stream, peticion->instruccion);
    //leer_string_del_stream(stream, peticion->interfaz);
    
    peticion->parametros = leer_parametros(paquete,peticion->interfaz);

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






