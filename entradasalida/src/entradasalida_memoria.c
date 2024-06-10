#include "../includes/entradasalida_memoria.h"

void guardar_en_memoria(char* leido, char* registroDireccion){
    //Enviar al MEMORIA: [GUARDAR_REGISTRO][Leido][RegistroDireccion]
    t_paquete* paquete = crear_paquete(GUARDAR_REGISTRO);
    agregar_string_a_paquete(paquete, leido);
    agregar_string_a_paquete(paquete, registroDireccion);
    enviar_paquete(paquete, fd_memoria);
    eliminar_paquete(paquete);
}

char* pedir_a_memoria(char* registroDireccion, int registroTamanio){
    //Enviar al MEMORIA: [GUARDAR_REGISTRO][RegistroDireccion][RegistroTamanio]
    t_paquete* paquete = crear_paquete(PEDIR_REGISTRO);
    agregar_string_a_paquete(paquete, registroDireccion);
    agregar_int_a_paquete(paquete, registroTamanio);
    enviar_paquete(paquete, fd_memoria);
    eliminar_paquete(paquete);
    char* escrito = recibir_mensaje_string(fd_memoria); // MEMORIA le envia al string con la funcion enviar_mensaje();
}
