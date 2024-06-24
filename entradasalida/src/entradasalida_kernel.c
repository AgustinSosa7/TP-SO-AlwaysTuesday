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
           peticion->parametros->registroDireccion= leer_string_del_buffer(buffer);
           peticion->parametros->registroTamanio= leer_int_del_buffer(buffer);
      }else if (strcmp(instruccion,"IO_STDOUT_WRITE") == 0)
      {
           peticion->parametros->registroDireccion= leer_string_del_buffer(buffer);
           peticion->parametros->registroTamanio= leer_int_del_buffer(buffer);
      }else if (strcmp(instruccion,"IO_FS_CREATE") == 0)
      {     
            peticion->parametros->archivo = leer_string_del_buffer(buffer);
      }else if (strcmp(instruccion,"IO_FS_DELETE") == 0)
      {
            peticion->parametros->archivo = leer_string_del_buffer(buffer);
      }else if (strcmp(instruccion,"IO_FS_TRUNCATE") == 0)
      {     
            peticion->parametros->archivo = leer_string_del_buffer(buffer);
            peticion->parametros->registroTamanio = leer_int_del_buffer(buffer); // No es el tamanio del registro, si no del archivo.

      }else if (strcmp(instruccion,"IO_FS_WRITE") == 0)
      {     //Ver Orden de esto :D 
            peticion->parametros->archivo = leer_string_del_buffer(buffer); 
            peticion->parametros->registroDireccion = leer_string_del_buffer(buffer);
            peticion->parametros->registroTamanio = leer_int_del_buffer(buffer);
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
            if(guardar_en_memoria(leido, peticion->parametros->registroDireccion)){
                  log_info(entradasalida_logger,"¨%s¨ se gurardo correctamente en %s.", leido, peticion->parametros->registroDireccion);
            } else{
                  log_info(entradasalida_logger,"¨%s¨ no se gurardo correctamente en %s. (debido a problemas de memoria)", leido, peticion->parametros->registroDireccion);
            }

      }else if (strcmp(instruccion,"IO_STDOUT_WRITE") == 0)
      {
            char* escrito = pedir_a_memoria(peticion->parametros->registroDireccion, peticion->parametros->registroTamanio);
            log_info(entradasalida_logger,"En la posicion de memoria %s con tamanio %d se encuentra: %s", peticion->parametros->registroDireccion, peticion->parametros->registroTamanio, escrito);
      }else if (strcmp(instruccion,"IO_FS_CREATE") == 0)
      {
            char* nombre_archivo = peticion->parametros->archivo;
            crear_config(nombre_archivo); // archivo viene ewewe.txt
  		log_info(entradasalida_logger, "Crear Archivo: %s", nombre_archivo);
            
      }else if (strcmp(instruccion,"IO_FS_DELETE") == 0)
      {
            char* nombre_archivo = peticion->parametros->archivo;
            delete_archivo(nombre_archivo);
            log_info(entradasalida_logger, "Se eliminó: %s", nombre_archivo);


      }else if (strcmp(instruccion,"IO_FS_TRUNCATE") == 0)
      {     
            //CONSIGO DATOS DEL CONFIG CON EL NOMBRE 
            char* nombre_archivo = peticion->parametros->archivo;
            int tamanio_nuevo = peticion->parametros->registroTamanio;
      
            if(truncar_archivo(nombre_archivo,tamanio_nuevo)) {  // Falta la parte de Compactación
                  log_info(entradasalida_logger, "Se pudo truncar el archivo: %s", nombre_archivo);
            }else{ log_info(entradasalida_logger, "No se pudo truncar el archivo: %s", nombre_archivo);}
            

      }else if (strcmp(instruccion,"IO_FS_WRITE") == 0) // Recibo tamanio del mensaje, el mensaje, archivo.
      {
            /*  Ver que hace Pipe XD
       char* nombre_archivo = peticion->parametros->archivo;
       int registro_archivo = atoi(peticion->parametros->registroPunteroArchivo); // Ver como va a llegar :D.
       char* escrito = pedir_a_memoria(peticion->parametros->registroDireccion, peticion->parametros->registroTamanio);
       escribir_archivo(nombre_archivo,registro_archivo,escrito);
       */
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
			log_info(entradasalida_logger, "Valor ingresado correctamente.\n");
                  break;
		}
		free(leido);
		log_info(entradasalida_logger, "El valor ingresado no debe superar el tamanio del registro: %d. Por favor reingrese un nuevo valor...\n", registroTamanio);
		leido = readline("> ");
	}
      return leido;
}

bool validar_tamanio_leido(char* leido, int registroTamanio){
      return ( registroTamanio > strlen(leido));
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
      {     free(parametros->registroDireccion);

      }else if (strcmp(instruccion,"IO_STDOUT_WRITE") == 0)
      {     free(parametros->registroDireccion);

      }else if (strcmp(instruccion,"IO_FS_CREATE") == 0)

      {     free(parametros->archivo);

      }else if (strcmp(instruccion,"IO_FS_DELETE") == 0)
      {     free(parametros->archivo);

      }else if (strcmp(instruccion,"IO_FS_TRUNCATE") == 0)
      {     free(parametros->archivo);
            free(&(parametros->registroTamanio));

      }else if (strcmp(instruccion,"IO_FS_WRITE") == 0)
      {     free(parametros->archivo);
            free(parametros->registroDireccion);
            free(parametros->registroPunteroArchivo);

      }else //Es IO_FS_READ 
      {     free(parametros->archivo);
            free(parametros->registroDireccion);
            free(parametros->registroPunteroArchivo);
      }     
      
      free(parametros);
}

