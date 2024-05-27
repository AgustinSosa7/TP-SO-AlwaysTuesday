#include "../includes/kernel_cpu_dispatch.h"


void atender_kernel_cpu_dispatch(){
    bool control_key = 1;
    while (control_key) {
        int cod_op = recibir_operacion(fd_cpu_dispatch);
        t_paquete* paquete = recibir_paquete(fd_cpu_dispatch);
	  log_info(kernel_logger, "Se recibio algo de CPU_Dispatch : %d", cod_op);


        switch (cod_op)
        {
        case ATENDER_PETICION_INTERFAZ_KERNEL:
        // podría ser que todo este CASE sea un "hilo" para que cada vez que se pida hacer una interfaz
        // se cree un hilo que lo maneje por separado, ya que como existen n interfaces, deberían
        // existin n hilos que se comuniquen con dichas interfaces
            t_peticion* peticion = recibir_peticion(paquete); 
            t_pcb* pcb = recibir_pcb(paquete, kernel_logger); // es probable que el pcb lo reciba antes
            t_interfaz* interfaz = validar_peticion(peticion, pcb);
            // enviar_proceso_a_blocked (funcion que crear mili)
            gestionar_lista_de_interfaz(peticion, interfaz);
            eliminar_peticion(peticion);
            recibir_fin_peticion();
            // desbloquear_proceso (funcion que crear mili)
            break;
        case -1:
            log_error(kernel_logger, "Desconexion de CPU - DISPATCH");      
            control_key = 0;
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

    leer_string_del_stream(stream, peticion->instruccion);
    leer_string_del_stream(stream, peticion->interfaz);
    
    peticion->parametros = leer_parametros(paquete,peticion->interfaz);

    return peticion;
} 

t_peticion_param* leer_parametros(t_paquete* paquete, char* instruccion){

    t_peticion_param* parametros = malloc(sizeof(t_peticion_param*));
    void* stream = paquete->buffer->stream;

    if(strcmp(instruccion,"IO_GEN_SLEEP") == 0){
        leer_algo_del_stream(stream, &parametros->tiempo_espera);
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






