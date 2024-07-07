#include "../includes/entradasalida_memoria.h"

void guardar_en_memoria(char* leido, int registroDireccion, int registroTamanio){ //escribir
    //Enviar al MEMORIA: [GUARDAR_REGISTRO][Leido][RegistroDireccion][RegistroTamanio]
    t_paquete* paquete = crear_paquete(GUARDAR_REGISTRO);
    agregar_int_a_paquete(paquete, registroDireccion);
    agregar_int_a_paquete(paquete, registroTamanio);
    agregar_void_a_paquete(paquete, leido, registroTamanio);  

    log_info(entradasalida_logger,"registroDireccion:%d. \n", registroDireccion);
    log_info(entradasalida_logger,"registroTamanio:%d. \n", registroTamanio);
    
    enviar_paquete(paquete, fd_memoria);
    eliminar_paquete(paquete);

    recibir_bool_mensaje(fd_memoria); //memoria me envia un ok
}


void partir_y_guardar_en_memoria(char* leido, t_list* lista_de_accesos){

    int cantidad_de_accesos = list_size(lista_de_accesos);

    for(int i = 0; i < cantidad_de_accesos; i++){   
      t_direccion_a_operar* acceso = list_get(lista_de_accesos, i);

      char* leido_chico = malloc(sizeof(char) * acceso->bytes_disponibles);
      
      strncpy(leido_chico, leido, acceso->bytes_disponibles);
      
      guardar_en_memoria(leido_chico, acceso->direccion_fisica, acceso->bytes_disponibles);
      
      leido += acceso->bytes_disponibles; 
      
      free(leido_chico);
      free(acceso);
    }
    free(leido);
    list_destroy(lista_de_accesos);
}




char* pedir_a_memoria(int registroDireccion, int registroTamanio){ //leer
    //Enviar al MEMORIA: [GUARDAR_REGISTRO][RegistroDireccion][RegistroTamanio]
    t_paquete* paquete = crear_paquete(PEDIR_REGISTRO);
    agregar_int_a_paquete(paquete, registroDireccion);
    agregar_int_a_paquete(paquete, registroTamanio);
    enviar_paquete(paquete, fd_memoria);
    eliminar_paquete(paquete);

    log_info(entradasalida_logger, "Envie a pedir algo a memoria..");

    //Respuesta:
    op_code code_op = recibir_operacion(fd_memoria);
    
    if(code_op == RESPUESTA_LEER_VALOR_EN_MEMORIA){
    t_paquete* paquete_respuesta = recibir_paquete(fd_memoria);
    t_buffer* buffer = paquete_respuesta->buffer;
    
    //Recibir unstring del mismo tamanio quel que pedi. Pasarlo a string.
    log_info(entradasalida_logger, "Recibi de memoria lo que le pedi..");

    char* leidoFinal = malloc(registroTamanio + 1);

    void* leido = (char*) leer_void_del_buffer(buffer,registroTamanio); //es un void. evaluar de castear.

    memcpy(leidoFinal, leido, registroTamanio);

    leidoFinal[registroTamanio] = '\0'; // Asegurar la terminación nula

    free(leido);
 
    return leidoFinal;
    }
    else
    {
        log_error(entradasalida_log_debug, "No se recibio una respuesta sobre la solicitud de leer un valor en memoria.");
        exit(EXIT_FAILURE);
    }
}

char* pedir_a_memoria_y_unir(t_list* lista_de_accesos){
    
    int cantidad_de_accesos = list_size(lista_de_accesos);

    char* escrito = malloc(sizeof(char) * tamanio_total_del_leido(lista_de_accesos));

    for(int i = 0; i < cantidad_de_accesos; i++){   
      t_direccion_a_operar* acceso = list_get(lista_de_accesos, i);
      
      strcat(escrito, pedir_a_memoria(acceso->direccion_fisica, acceso->bytes_disponibles));
        
      free(acceso);  
    }
    list_destroy(lista_de_accesos);

    return escrito;
}

int tamanio_total_del_leido(t_list* lista_de_accesos){
      int tamanio_total = 0;
      int cantidad_de_accesos = list_size(lista_de_accesos);

      for(int i = 0; i < cantidad_de_accesos; i++){
            t_direccion_a_operar* acceso = list_get(lista_de_accesos, i);
            tamanio_total += (acceso->bytes_disponibles);
      }
      return tamanio_total;
}
