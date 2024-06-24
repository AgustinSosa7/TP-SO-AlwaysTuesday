#include "../includes/entradasalida_memoria.h"

bool guardar_en_memoria(char* leido, int registroDireccion){ //escribir
    //Enviar al MEMORIA: [GUARDAR_REGISTRO][Leido][RegistroDireccion]
    t_paquete* paquete = crear_paquete(GUARDAR_REGISTRO);
    agregar_string_a_paquete(paquete, leido);
    agregar_int_a_paquete(paquete, registroDireccion);
    enviar_paquete(paquete, fd_memoria);
    eliminar_paquete(paquete);
    bool fin_guardado = recibir_bool_mensaje(fd_memoria);
    return fin_guardado;
}

char* pedir_a_memoria(int registroDireccion, int registroTamanio){ //leer
    //Enviar al MEMORIA: [GUARDAR_REGISTRO][RegistroDireccion][RegistroTamanio]
    t_paquete* paquete = crear_paquete(PEDIR_REGISTRO);
    agregar_int_a_paquete(paquete, registroDireccion);
    agregar_int_a_paquete(paquete, registroTamanio);
    enviar_paquete(paquete, fd_memoria);
    eliminar_paquete(paquete);
    //Respuesta:
    op_code code_op = recibir_operacion(fd_memoria);
    
    if(code_op == RESPUESTA_LEER_VALOR_EN_MEMORIA){
    t_paquete* paquete_respuesta = recibir_paquete(fd_memoria);
    t_buffer* buffer = paquete_respuesta->buffer;
    
    //Recibir unstring del mismo tamanio quel que pedi. Pasarlo a string.
    char* leido = leer_void_del_buffer(buffer,registroTamanio); //es un void. evaluar de castear.

    return leido;
    }
    else
    {
        log_error(entradasalida_log_debug, "No se recibio una respuesta sobre la solicitud de leer un valor en memoria.");
        exit(EXIT_FAILURE);
    }
}
