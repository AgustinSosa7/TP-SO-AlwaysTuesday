#include "../includes/memoria_entradasalida.h"


void atender_entradasalida()
{     int control_key = 1;
      log_info(memoria_logger, "Atendiendo Entradasalida...");
      while (control_key){
      int code_op = recibir_operacion(fd_entradasalida);
      t_paquete* paquete = recibir_paquete(fd_entradasalida);
      log_info(memoria_logger, "Se recibio algo de EntradaSalida : %d", code_op);
      t_buffer* buffer = paquete->buffer;
      switch (code_op)
      {
      case GUARDAR_REGISTRO:
            char* leido = leer_string_del_buffer(buffer);  //(lo que quiero escribir)
            int registroDireccion = leer_int_del_buffer(buffer);//int
            //int registroTamanio = leer_string_del_buffer(buffer); //si lo necesito se lo pido a pipe
            if(guardar_leido_en_registro(leido, registroDireccion)){ // definir comportamiento de la funcion
                  enviar_bool_mensaje(true, fd_entradasalida); 
            } else{
                  enviar_bool_mensaje(false, fd_entradasalida);}
            break;
      case PEDIR_REGISTRO:
            char* registro_Direccion = leer_string_del_buffer(buffer); //int
            int registroTamanio = leer_int_del_buffer(buffer); //int
            char* escrito = devolver_registro(registro_Direccion, registroTamanio); // definir comportamiento de la funcion
            enviar_mensaje_string(escrito, fd_entradasalida);
            break;
    //  case :          
    //    
    //        break;
      case -1:
            log_error(memoria_logger, "Desconexion de ENTRADASALIDA");      
            control_key = 0;
            break;
      default:
            log_warning(memoria_logger, "Operacion desconocida de ENTRADASALIDA");
            break;
        }
        eliminar_paquete(paquete);    
      }
}

bool guardar_leido_en_registro(char* leido, int registroDireccion){ 
      return NULL;
}

char* devolver_registro(char* registro_Direccion, int registroTamanio){
    return NULL;
}