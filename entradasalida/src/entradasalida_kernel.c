#include "../includes/entradasalida_kernel.h"


void atender_entradasalida_kernel(){
    bool control_key = 1;
    while (control_key) {
        int cod_op = recibir_operacion(fd_kernel);
        t_paquete* paquete = recibir_paquete(fd_kernel);
	  log_info(entradasalida_logger, "Se recibio algo de Kernel..");

        switch (cod_op)
        {
        case ATENDER_PETICION_INTERFAZ_KERNEL:
            t_peticion* peticion = recibir_peticion(paquete); //Ya tengo en peticion todos los datos que necesito.
            t_pcb* un_pcb = recibir_pcb(paquete);
            procesar_peticion(peticion);
            finalizar_peticion(peticion, un_pcb);
            break;
        case ESTOY_CONECTADO:
            void* buffer = paquete->buffer;
            char* instruccion = malloc(sizeof(char));
            t_list* instrucciones_posibles;
            instrucciones_posibles = list_create();

            t_pcb* pcb = recibir_pcb(paquete);
            leer_algo_del_stream(buffer, instrucciones_posibles, sizeof(t_list));
            instruccion = leer_string_del_stream(buffer);

            crear_paquete(ESTOY_CONECTADO);
            agregar_pcb_al_paquete(paquete, pcb);
            agregar_algo_a_paquete(paquete, instrucciones_posibles);
            agregar_algo_a_paquete(paquete, instruccion);

            enviar_paquete(paquete, fd_kernel);
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
    peticion->instruccion = malloc(sizeof(char));

    void* stream = paquete->buffer->stream;
    leer_string_del_stream(stream);

    asignar_parametros_segun_tipo(peticion, stream);

    return peticion;
} 

void asignar_parametros_segun_tipo(t_peticion* peticion, void* stream){

      char* instruccion = peticion->instruccion;

      if(strcmp(instruccion,"IO_GEN_SLEEP") == 0){
            leer_algo_del_stream(stream, &peticion->parametros->tiempo_espera,sizeof(peticion->parametros->tiempo_espera));

      }else if (strcmp(instruccion,"IO_STDIN_READ") == 0)
      {
            /* code */
      }else if (strcmp(instruccion,"IO_STDOUT_WRITE") == 0)
      {
            /* code */
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
            usleep(tiempo_espera*1000);

      }else if (strcmp(instruccion,"IO_STDIN_READ") == 0)
      {
            /* code */
      }else if (strcmp(instruccion,"IO_STDOUT_WRITE") == 0)
      {
            /* code */
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

char* recibir_instruccion(t_paquete* paquete){
      char* instruccion = malloc(sizeof(char));
      void* buffer = paquete->buffer;

      instruccion = leer_string_del_stream(buffer);
      return instruccion;
}

void finalizar_peticion(t_peticion* peticion, t_pcb* pcb){
      t_paquete* paquete = crear_paquete(FIN_PETICION);
      agregar_pcb_al_paquete(paquete, pcb);
      enviar_paquete(paquete, fd_kernel);
      
      eliminar_peticion(peticion);
}

void eliminar_peticion(t_peticion* peticion){ 
      free(peticion->parametros->archivo);
      free(peticion->parametros->registro1);
      free(peticion->parametros->registro2);
      free(peticion->parametros->registro3);
      free(peticion->parametros);
      free(peticion);
}      

void agregar_pcb_al_paquete(t_paquete* paquete, t_pcb* pcb){      
            agregar_algo_a_paquete(paquete, &(pcb->pid));
            agregar_algo_a_paquete(paquete,&(pcb->quantum));
            agregar_registro_a_paquete(paquete, pcb->registros_cpu);
	      agregar_algo_a_paquete(paquete,&(pcb->estado_pcb));
            enviar_paquete(paquete, fd_kernel);
}