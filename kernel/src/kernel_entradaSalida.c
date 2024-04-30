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


void validar_peticion(t_peticion* peticion){
      char* interfaz = peticion->interfaz;
      char* instruccion = peticion->instruccion;

      if(/*existe_la_interfaz(interfaz) && esta_conectada_la_interfaz(interfaz) && */validar_interfaz_admite_instruccion(interfaz, instruccion)){

//      }else{
//            enviar_proceso_a_exit
//      }
    }
}
//bool existe_la_interfaz(interfaz){
//      
//}         //Supongo que esto se va a poder hacer cuando este creado el pcb(?

//bool esta_conectada_la_interfaz(char* interfaz){
//
//}

bool validar_interfaz_admite_instruccion(char* interfaz, char* instruccion){
      t_paquete* paquete = crear_paquete(RECONOCER_INSTRUCCION);
      agregar_string_a_paquete(paquete,instruccion);
      enviar_paquete(paquete, fd_entradasalida);
      eliminar_paquete(paquete);
      bool acepta_la_instruccion = recibir_mensaje(fd_entradasalida);
      if(acepta_la_instruccion){
            return true;
      }else{
            log_error(kernel_logger,"Interfaz %s: No reconozco esta instruccion. Proceso enviado a EXIT.\n", interfaz);
            return false;
      }
}

void enviar_peticion_a_interfaz(t_peticion* peticion){ 
      t_paquete* paquete = crear_paquete(ATENDER_PETICION_INTERFAZ_KERNEL);
      agregar_string_a_paquete(paquete, peticion->instruccion);
      agregar_a_paquete(paquete, peticion->parametros, sizeof(t_peticion_param));
      enviar_paquete(paquete, fd_entradasalida);
      eliminar_paquete(paquete);
} 

void recibir_mensaje_fin_peticion(){
    char* peticion_exitosa = recibir_mensaje(fd_entradasalida);
    log_error(kernel_logger, "%s", peticion_exitosa);
}