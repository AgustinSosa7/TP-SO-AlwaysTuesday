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
bool validar_interfaz_admite_instruccion(char* interfaz, char* instruccion){
      t_paquete* paquete = crear_paquete(RECONOCER_INSTRUCCION);
      agregar_string_a_paquete(paquete,instruccion);
      enviar_paquete(paquete, fd_entradasalida);
      eliminar_paquete(paquete);
      bool acepta_la_instruccion = recibir_mensaje(fd_cpu_dispatch);
      if(acepta_la_instruccion){
            return true;
      }else{
            log_error(kernel_logger,"Interfaz %s: No reconozco esta instruccion. Proceso enviado a EXIT.\n", interfaz);
            return false;
      }
}
