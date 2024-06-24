#include "../includes/entradasalida_kernel.h"


void atender_entradasalida_kernel(){
    log_info(entradasalida_logger, "Atendiendo KERNEL...");  
    bool control_key = 1;
    while (control_key) {
        int cod_op = recibir_operacion(fd_kernel);
        t_paquete* paquete = recibir_paquete(fd_kernel);
	  log_info(entradasalida_logger, "Se recibio algo de Kernel..");
        log_info(entradasalida_logger, "CODIGO DE OPERACION: %d", cod_op);

        switch (cod_op)
        {
        case ATENDER_PETICION_INTERFAZ_KERNEL:
            t_peticion* peticion = recibir_peticion(paquete); //Ya tengo en peticion todos los datos que necesito.
            procesar_peticion(peticion);
            finalizar_peticion(peticion);
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
    t_buffer* buffer = paquete->buffer;

    peticion->instruccion = malloc(sizeof(char));
    peticion->instruccion = leer_string_del_buffer(buffer);

    asignar_parametros_segun_tipo(peticion, buffer); 

    return peticion;
} 

void asignar_parametros_segun_tipo(t_peticion* peticion, t_buffer* buffer){

      char* instruccion = peticion->instruccion;
      peticion->parametros = malloc(sizeof(t_peticion_param));
      
      if(strcmp(instruccion,"IO_GEN_SLEEP") == 0){
           peticion->parametros->tiempo_espera= leer_int_del_buffer(buffer);

      }else if (strcmp(instruccion,"IO_STDIN_READ") == 0)
      {
           peticion->parametros->registroDireccion= leer_int_del_buffer(buffer);
           peticion->parametros->registroTamanio= leer_int_del_buffer(buffer);
      }else if (strcmp(instruccion,"IO_STDOUT_WRITE") == 0)
      {
           peticion->parametros->registroDireccion= leer_int_del_buffer(buffer);
           peticion->parametros->registroTamanio= leer_int_del_buffer(buffer);
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
            printf("Estoy por dormir...ZZZ...\n");
            usleep(tiempo_espera*1000); 
            printf("Ya dormi mi tiempo.\n");

      }else if (strcmp(instruccion,"IO_STDIN_READ") == 0)
      {
            char* leido = iniciar_la_consola(peticion->parametros->registroTamanio);
            guardar_en_memoria(leido, peticion->parametros->registroDireccion, peticion->parametros->registroTamanio);
            log_info(entradasalida_logger,"¨%s¨ se gurardo correctamente.\n", leido);

      }else if (strcmp(instruccion,"IO_STDOUT_WRITE") == 0)
      {
            log_info(entradasalida_logger,"Voy a pedirle algo a memoria");
            char* escrito = pedir_a_memoria(peticion->parametros->registroDireccion, peticion->parametros->registroTamanio);
            log_info(entradasalida_logger,"%s", escrito);

      }else if (strcmp(instruccion,"IO_FS_CREATE") == 0)
      {
            char* nombre_archivo = peticion->parametros->archivo;
            crear_fcb(nombre_archivo); // archivo viene ewewe.txt???
            
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

char* iniciar_la_consola(int registroTamanio){
      char* leido;
	leido = readline("> ");
	while(strcmp(leido,"\0") != 0){
		if(validar_tamanio_leido(leido, registroTamanio)){
                  break;
		}
		free(leido);
		log_info(entradasalida_logger, "El valor ingresado debe tener %d caracteres. Por favor reingrese un nuevo valor...\n", registroTamanio);
		leido = readline("> ");
	}
      return leido;
}

bool validar_tamanio_leido(char* leido, int registroTamanio){
      return (registroTamanio = strlen(leido));
}

void finalizar_peticion(t_peticion* peticion){
      enviar_bool_mensaje(true, fd_kernel);
      
      eliminar_peticion(peticion);
}

void eliminar_peticion(t_peticion* peticion){
      eliminar_parametros_segun_instruccion(peticion->instruccion, peticion->parametros);
      free(peticion->instruccion);
      free(peticion);
}

void eliminar_parametros_segun_instruccion(char* instruccion, t_peticion_param* parametros){
      if(strcmp(instruccion,"IO_GEN_SLEEP") == 0){

      }else if (strcmp(instruccion,"IO_STDIN_READ") == 0)
      {     

      }else if (strcmp(instruccion,"IO_STDOUT_WRITE") == 0)
      {     

      }else if (strcmp(instruccion,"IO_FS_CREATE") == 0)
      {     free(parametros->archivo);

      }else if (strcmp(instruccion,"IO_FS_DELETE") == 0)
      {     free(parametros->archivo);

      }else if (strcmp(instruccion,"IO_FS_TRUNCATE") == 0)
      {     free(parametros->archivo);

      }else if (strcmp(instruccion,"IO_FS_WRITE") == 0)
      {     free(parametros->archivo);
            free(parametros->registroPunteroArchivo);

      }else //Es IO_FS_READ 
      {     free(parametros->archivo);
            free(parametros->registroPunteroArchivo);
      }     
      
      free(parametros);
}