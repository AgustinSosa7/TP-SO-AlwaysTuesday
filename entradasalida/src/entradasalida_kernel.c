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

    peticion->pid = leer_int_del_buffer(buffer);
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
           leer_parametros_lista_de_accesos(peticion->parametros, buffer);
      }else if (strcmp(instruccion,"IO_STDOUT_WRITE") == 0)
      {
           leer_parametros_lista_de_accesos(peticion->parametros, buffer);
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
            leer_parametros_lista_de_accesos(peticion->parametros, buffer);
            peticion->parametros->registroTamanio = leer_int_del_buffer(buffer);
            peticion->parametros->registroPunteroArchivo = leer_int_del_buffer(buffer);

      }else //DEFALUT IO_FS_READ
      {
            peticion->parametros->archivo = leer_string_del_buffer(buffer); 
            leer_parametros_lista_de_accesos(peticion->parametros, buffer);
            peticion->parametros->registroTamanio = leer_int_del_buffer(buffer);
            peticion->parametros->registroPunteroArchivo = leer_int_del_buffer(buffer);
      }
}


void procesar_peticion(t_peticion* peticion) {

      char* instruccion = peticion->instruccion;

      if(strcmp(instruccion,"IO_GEN_SLEEP") == 0){
            int tiempo_espera = TIEMPO_UNIDAD_TRABAJO * peticion->parametros->tiempo_espera; 
            printf("Estoy durmiendo...ZZZ...\n");
            usleep(tiempo_espera*1000); 
            printf("Ya dormi mi tiempo.\n");

      }else if (strcmp(instruccion,"IO_STDIN_READ") == 0)
      {
            char* leido = iniciar_la_consola(tamanio_total_del_leido(peticion->parametros->lista_de_accesos));
                       
            partir_y_guardar_en_memoria(leido, peticion->parametros->lista_de_accesos, peticion->pid);

            log_info(entradasalida_logger,"¨%s¨ se gurardo correctamente.\n", leido);

            free(leido);

      }else if (strcmp(instruccion,"IO_STDOUT_WRITE") == 0)
      {     
            log_info(entradasalida_logger,"Voy a pedirle algo a memoria");

            char* escrito = pedir_a_memoria_y_unir(peticion->parametros->lista_de_accesos, peticion->pid);

            log_warning(entradasalida_logger,"¨%s¨", escrito);

            free(escrito);      

      }else if (strcmp(instruccion,"IO_FS_CREATE") == 0)
      {     
            usleep(1000*TIEMPO_UNIDAD_TRABAJO);
            char* nombre_archivo = peticion->parametros->archivo;
            crear_config(nombre_archivo); // archivo viene ewewe.txt
  		log_info(entradasalida_logger, "Se creo el archivo: %s", nombre_archivo);
            
      }else if (strcmp(instruccion,"IO_FS_DELETE") == 0)
      {     
            usleep(1000*TIEMPO_UNIDAD_TRABAJO);
            char* nombre_archivo = peticion->parametros->archivo;
            delete_archivo(nombre_archivo);
            log_info(entradasalida_logger, "Se eliminó: %s", nombre_archivo);


      }else if (strcmp(instruccion,"IO_FS_TRUNCATE") == 0)
      {     
            //CONSIGO DATOS DEL CONFIG CON EL NOMBRE 
            usleep(1000*TIEMPO_UNIDAD_TRABAJO);
            char* nombre_archivo = peticion->parametros->archivo;
            int tamanio_nuevo = peticion->parametros->registroTamanio;
      
            if(truncar_archivo(nombre_archivo,tamanio_nuevo)) {  
                  log_info(entradasalida_logger, "Se pudo truncar el archivo: %s", nombre_archivo);
            }else{ log_info(entradasalida_logger, "No se pudo truncar el archivo: %s", nombre_archivo);}
            

      }else if (strcmp(instruccion,"IO_FS_WRITE") == 0) // Recibo tamanio del mensaje, el mensaje, archivo.
      {     
            log_info(entradasalida_logger,"Ejecutando IO_FS_WRITE");
            usleep(1000*TIEMPO_UNIDAD_TRABAJO);
            char* nombre_archivo = peticion->parametros->archivo;
            int registro_archivo = peticion->parametros->registroPunteroArchivo; 
            int tamanio_text = peticion->parametros->registroTamanio; // El nombre no corresponde pero si el tipo de dato XD
            
            char* escrito = pedir_a_memoria_y_unir(peticion->parametros->lista_de_accesos, peticion->pid);

            log_warning(entradasalida_logger,"¨%s¨", escrito);

            if(escribir_archivo(nombre_archivo,registro_archivo,escrito,tamanio_text)){
            log_info(entradasalida_logger,"Se ha escrito %s en el archivo %s",escrito,nombre_archivo);

            }else{
                 log_info(entradasalida_logger,"No se ha podido escribir %s en el archivo %s",escrito,nombre_archivo);
            }

            free(escrito);
       
      }else //DEFALUT IO_FS_READ
      {                 
            log_info(entradasalida_logger,"Ejecutando IO_FS_READ");
            usleep(1000*TIEMPO_UNIDAD_TRABAJO);
            char* nombre_archivo = peticion->parametros->archivo;
            int registro_archivo = peticion->parametros->registroPunteroArchivo;
            int tamanio = tamanio_total_del_leido(peticion->parametros->lista_de_accesos);
            
            char* leido = leer_archivo(nombre_archivo,registro_archivo,tamanio); 
            
            if (strcmp(leido,"")==0){ 
                  log_info(entradasalida_logger,"No se pudo guardar correctamente.\n");
            }else{
                  partir_y_guardar_en_memoria(leido, peticion->parametros->lista_de_accesos, peticion->pid);
                  log_info(entradasalida_logger,"¨%s¨ se guardo correctamente.\n", leido);
            }     
            //free(leido);
      }
}      

char* iniciar_la_consola(int registroTamanio) {
    char* leido = NULL;

    while (1) {
        if (leido != NULL) {
            free(leido);
        }
        leido = readline("> ");
        if (leido == NULL || strcmp(leido, "\0") == 0) {
            // Entrada vacía o error en readline
            if (leido != NULL) {
                free(leido);
            }
            return NULL;
        }
        if (validar_tamanio_leido(leido, registroTamanio)) {
            break;
        }
        log_info(entradasalida_logger, "El valor ingresado debe tener %d caracteres. Por favor reingrese un nuevo valor...\n", registroTamanio);
    }
    return leido;
}

bool validar_tamanio_leido(char* leido, int registroTamanio){
      return (registroTamanio == strlen(leido));
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
            

      }else //Es IO_FS_READ 
      {     free(parametros->archivo);
           
      }     
      
      free(parametros);
}

