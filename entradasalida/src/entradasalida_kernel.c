#include "../includes/entradasalida_kernel.h"


void atender_entradasalida_kernel(){
    bool control_key = 1;
    while (control_key) {
        int cod_op = recibir_operacion(fd_kernel);
        t_paquete* paquete = recibir_paquete(fd_kernel);
	  log_info(entradasalida_logger, "Se recibio algo de Kernel..");

        switch (cod_op)
        {
        case ATENDER_PETICION_INTERFAZ_KERNEL:
            t_peticion* peticion = recibir_peticion(paquete); //Ya tengo en peticion todos los datos que necesito.
            procesar_peticion(peticion);
            finalizar_peticion(peticion);
            break;
        case RECONOCER_INSTRUCCION:
            char* instruccion = recibir_instruccion(paquete);
            validar_tipo_instruccion(instruccion);   
            free(instruccion); 
            break;
        case ESTOY_CONECTADO:
            bool respuesta = dar_respuesta_conectado(paquete); 
            enviar_mensaje(true, fd_kernel);            
            break;
        case -1:
          //  log_error(logger, "Desconexion de CPU - DISPATCH");      
            control_key = 0;
        default:
           // log_warning(logger, "Operacion desconocida de CPU - DISPATCH");
            break;
        }
        eliminar_paquete(paquete);
    }
}


t_peticion* recibir_peticion(t_paquete* paquete){
    t_peticion* peticion = malloc(sizeof(t_peticion));
    peticion->instruccion = malloc(sizeof(char));

    void* stream = paquete->buffer->stream;
    leer_string_del_stream(stream);

    asignar_parametros_segun_tipo(peticion, stream);

    return peticion;
} 

void asignar_parametros_segun_tipo(t_peticion* peticion, void* stream){

      char* instruccion = peticion->instruccion;

      if(strcmp(instruccion,"IO_GEN_SLEEP") == 0){
            leer_algo_del_stream(stream, &peticion->parametros->tiempo_espera,sizeof(peticion->parametros->tiempo_espera));

      }else if (strcmp(instruccion,"IO_STDIN_READ") == 0)
      {
            /* code */
      }else if (strcmp(instruccion,"IO_STDOUT_WRITE") == 0)
      {
            /* code */
      }else if (strcmp(instruccion,"IO_FS_CREATE") == 0)
      {
            /* code */
      }else if (strcmp(instruccion,"IO_FS_DELETE") == 0)
      {
            /* code */
      }else if (strcmp(instruccion,"IO_FS_TRUNCATE") == 0)
      {
            /* code */
      }else if (strcmp(instruccion,"IO_FS_WRITE") == 0)
      {
            /* code */
      }else //DEFALUT IO_FS_READ
      {
            /* code */
      }
}

void procesar_peticion(t_peticion* peticion) {

      char* instruccion = peticion->instruccion;

      if(strcmp(instruccion,"IO_GEN_SLEEP") == 0){
            int tiempo_espera = TIEMPO_UNIDAD_TRABAJO * peticion->parametros->tiempo_espera;
            usleep(tiempo_espera*1000);

      }else if (strcmp(instruccion,"IO_STDIN_READ") == 0)
      {
            /* code */
      }else if (strcmp(instruccion,"IO_STDOUT_WRITE") == 0)
      {
            /* code */
      }else if (strcmp(instruccion,"IO_FS_CREATE") == 0)
      {
            /* code */
      }else if (strcmp(instruccion,"IO_FS_DELETE") == 0)
      {
            /* code */
      }else if (strcmp(instruccion,"IO_FS_TRUNCATE") == 0)
      {
            /* code */
      }else if (strcmp(instruccion,"IO_FS_WRITE") == 0)
      {
            /* code */
      }else //DEFALUT IO_FS_READ
      {
            /* code */
      }
}      

char* recibir_instruccion(t_paquete* paquete){
      char* instruccion = malloc(sizeof(char));
      void* stream = paquete->buffer->stream;

      leer_string_del_stream(stream);
      return instruccion;
}

void validar_tipo_instruccion(char* instruccion){

      if(contains_string(INSTRUCCIONES_POSIBLES, instruccion)){
            bool resultado = true;
            enviar_mensaje(&resultado, fd_kernel);
      }else{
            bool resultado = false;
            enviar_mensaje(&resultado, fd_kernel);
            }
}

void finalizar_peticion(t_peticion* peticion){
      bool resultado = true;
      enviar_mensaje(&resultado, fd_kernel);
      
      eliminar_peticion(peticion);
}

void eliminar_peticion(t_peticion* peticion){ 
      free(peticion->parametros->archivo);
      free(peticion->parametros->registro1);
      free(peticion->parametros->registro2);
      free(peticion->parametros->registro3);
      free(peticion->parametros);
      free(peticion);
}      

 bool dar_respuesta_conectado(t_paquete* paquete){
      bool respuesta = malloc(sizeof(bool));
      void* stream = paquete->buffer->stream;

      leer_algo_del_stream(stream, &respuesta);
      return respuesta;
 }