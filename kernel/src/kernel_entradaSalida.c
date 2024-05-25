#include "../includes/kernel_entradaSalida.h"
#include <../src/utils/utils_connections.h>



void atender_kernel_entradaSalida(){
    bool control_key = 1;
    while (control_key) {
        int cod_op = recibir_operacion(fd_entradasalida);
        switch (cod_op)
        {
        case MENSAJE:
            //
            break;
        case -1:
          //  log_error(logger, "Desconexion de ENTRADASALIDA");      
            control_key = 0;
        default:
           // log_warning(logger, "Operacion desconocida de ENTRADASALIDA");
            break;
        }
    }
}


t_interfaz* validar_peticion(t_peticion* peticion){ //debe recibir pcb para poder mandarlo a exit?
      char* nombre_io = peticion->interfaz;
      char* instruccion = peticion->instruccion;

    t_interfaz* interfaz = existe_la_interfaz(nombre_io);
    esta_conectada_la_interfaz(interfaz);
    validar_interfaz_admite_instruccion(interfaz, instruccion);

    return interfaz;
}
t_interfaz* existe_la_interfaz(nombre_io){
      if(){
        // elem_exist(ios_conectadas, esta_la_io(io, nombre_io)); mediante funciones de orden superior
      }else{
            //enviar_proceso_a_exit();
            }
}         
void esta_conectada_la_interfaz(t_interfaz* interfaz){
      if(interfaz->fd_interfaz > 0){ //esta es la condicion correcta?
             
      }else{
            //enviar_proceso_a_exit();
            } 
}

void validar_interfaz_admite_instruccion(char* interfaz, char* instruccion){
      t_paquete* paquete = crear_paquete(RECONOCER_INSTRUCCION);
      agregar_string_a_paquete(paquete, instruccion);
      enviar_paquete(paquete, fd_entradasalida);
      eliminar_paquete(paquete);

      bool acepta_la_instruccion = recibir_mensaje(fd_entradasalida);
      if(acepta_la_instruccion){

      }else{
            log_error(kernel_logger,"Interfaz %s: No reconozco esta instruccion. Proceso enviado a EXIT.\n", interfaz);
            //enviar_proceso_a_exit();
      }
}

void enviar_peticion_a_interfaz(t_peticion* peticion, t_interfaz* interfaz){ 
      t_paquete* paquete = crear_paquete(ATENDER_PETICION_INTERFAZ_KERNEL);
      agregar_string_a_paquete(paquete, peticion->instruccion);
      agregar_algo_a_paquete(paquete, peticion->parametros);
      enviar_paquete(paquete, interfaz->);
      eliminar_paquete(paquete);
} 

void recibir_mensaje_fin_peticion(){
    bool fin_peticion;
    recv(fd_entradasalida, &fin_peticion, sizeof(bool), MSG_WAITALL);
}