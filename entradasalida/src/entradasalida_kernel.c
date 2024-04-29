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
            t_peticion* peticion = peticion_deserializar(paquete->buffer); //Ya tengo en peticion todos los datos que necesito.
            procesar_peticion(peticion->instruccion, peticion->parametros);
            free(peticion);
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

t_peticion* peticion_deserializar(t_buffer* buffer){
    t_peticion* peticion = malloc(sizeof(t_peticion));;

    uint32_t tamanio_string = buffer_read_uint32(buffer);  //Se podria mejorar la manera de leer los strings.
    peticion->instruccion = malloc(tamanio_string);
    peticion->instruccion = buffer_read_string(buffer, tamanio_string);

    peticion->parametros->tiempo_espera = buffer_read_uint32(buffer);

    return peticion;
} 

void procesar_peticion(char* instruccion, t_peticion_param* parametros) {
      
      if(strcmp(instruccion,"IO_GEN_SLEEP") == 0){
            int tiempo_espera = TIEMPO_UNIDAD_TRABAJO * parametros->tiempo_espera;
            sleep(tiempo_espera);
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
      }else if (strcmp(instruccion,"IO_FS_READ") == 0)
      {
            /* code */
      }else{
            //log_error(entradasalida_logger,"Interfaz %s: Tipo de interfaz no soportado. Proceso enviado a EXIT.\n", NOMBRE_INTERFAZ);
            exit(EXIT_FAILURE);
      }
}      