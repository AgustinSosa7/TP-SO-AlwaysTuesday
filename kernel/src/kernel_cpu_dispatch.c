#include "../includes/kernel_cpu_dispatch.h"


void atender_kernel_cpu_dispatch(){
    bool control_key = 1;
    while (control_key) {
        int cod_op = recibir_operacion(fd_cpu_dispatch);
        t_paquete* paquete = recibir_paquete(fd_cpu_dispatch);
	  log_info(kernel_logger, "Se recibio algo de CPU_Dispatch");

        switch (cod_op)
        {
        case ATENDER_PETICION_INTERFAZ_KERNEL:
            t_peticion* peticion = recibir_peticion(paquete); 
            validar_peticion(peticion);
            enviar_peticion_a_interfaz(peticion);
            eliminar_peticion(peticion);
            // enviar_proceso_a_blocked
            char* peticion_exitosa = recibir_mensaje(fd_entradasalida);
            log_error(kernel_logger, "%s", peticion_exitosa);
            // desbloquear_proceso
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
    t_peticion* peticion = malloc(sizeof(t_peticion));;

    leer_string_del_paquete(paquete, &peticion->instruccion);
    leer_string_del_paquete(paquete, &peticion->interfaz);
    
    peticion->parametros = leer_parametros(paquete,peticion->interfaz);

    return peticion;
} 

t_peticion_param* leer_parametros(t_paquete* paquete, char* instruccion){

    t_peticion_param* parametros = malloc(sizeof(t_peticion_param*));

    if(strcmp(instruccion,"IO_GEN_SLEEP") == 0){
        leer_del_paquete(paquete, (void**)&parametros->tiempo_espera, sizeof(int));
        return parametros;
      }else if (strcmp(instruccion,"IO_STDIN_READ") == 0)
      {
            return parametros;
      }else if (strcmp(instruccion,"IO_STDOUT_WRITE") == 0)
      {
            return parametros;
      }else if (strcmp(instruccion,"IO_FS_CREATE") == 0)
      {
            return parametros;
      }else if (strcmp(instruccion,"IO_FS_DELETE") == 0)
      {
            return parametros;
      }else if (strcmp(instruccion,"IO_FS_TRUNCATE") == 0)
      {
            return parametros; 
      }else if (strcmp(instruccion,"IO_FS_WRITE") == 0)
      {
            return parametros; 
      }else if (strcmp(instruccion,"IO_FS_READ") == 0)
      {
            return parametros;
      }
}

void enviar_peticion_a_interfaz(t_peticion* peticion){ 
      t_paquete* paquete = crear_paquete(ATENDER_PETICION_INTERFAZ_KERNEL);
      agregar_string_a_paquete(paquete, peticion->instruccion);
      agregar_a_paquete(paquete, peticion->parametros, sizeof(t_peticion_param));
      enviar_paquete(paquete, fd_entradasalida);
      eliminar_paquete(paquete);
}            

void eliminar_peticion(t_peticion* peticion){
      free(peticion->instruccion);
      free(peticion->interfaz);
      free(peticion->parametros->archivo);
      free(peticion->parametros->registro1);
      free(peticion->parametros->registro2);
      free(peticion->parametros->registro3);
      free(peticion->parametros);
      free(peticion);

}

void validar_peticion(t_peticion* peticion){
      char* interfaz = peticion->interfaz;
      char* instruccion = peticion->instruccion;

      if(/*existe_la_interfaz(interfaz) && esta_conectada_la_interfaz(interfaz) && */validar_interfaz_admite_instruccion(interfaz, instruccion)){
//      }else{
//            enviar_proceso_a_exit
//      }
}

//bool existe_la_interfaz(interfaz){
//      
//}         //Supongo que esto se va a poder hacer cuando este creado el pcb(?
//
//bool esta_conectada_la_interfaz(char* interfaz){
//
//}

bool validar_interfaz_admite_instruccion(char* interfaz, char* instruccion){
      t_paquete* paquete = crear_paquete(RECONOCER_INSTRUCCION);
      agregar_string_a_paquete(paquete, peticion->instruccion);
      enviar_paquete(paquete, fd_entradasalida);
      eliminar_paquete(paquete);
      bool acepta_la_instruccion = recibir_mensaje(fd_cpu_dispatch);
      if(acepta_la_instruccion){
            return true;
      }else{
            log_error(kernel_logger,"Interfaz %s: No reconozco esta instruccion. Proceso enviado a EXIT.\n", interfaz);
            EXIT_FAILURE;
      }
}




//void _enviar_pcb_a_CPU_por_dispatch(t_pcb* una_pcb){  //Faltaría crear el PCB
//	t_paquete* un_paquete = crear_super_paquete(EJECUTAR_PROCESO_KC); //Ejecutar, ver si tiene ese nombre;
//	cargar_int_al_super_paquete(un_paquete, una_pcb->ticket);
//	cargar_int_al_super_paquete(un_paquete, una_pcb->program_counter);
//	cargar_choclo_al_super_paquete(un_paquete, &(una_pcb->registros_CPU.AX), sizeof(uint32_t)); //Cambiamos nombres?
//	cargar_choclo_al_super_paquete(un_paquete, &(una_pcb->registros_CPU.BX), sizeof(uint32_t));
//	cargar_choclo_al_super_paquete(un_paquete, &(una_pcb->registros_CPU.CX), sizeof(uint32_t));
//	cargar_choclo_al_super_paquete(un_paquete, &(una_pcb->registros_CPU.DX), sizeof(uint32_t));
//	cargar_int_al_super_paquete(un_paquete, una_pcb->pid); //Verificar los prox 3 nombres
//
//	enviar_paquete(un_paquete, fd_cpu_dispatch);
//	eliminar_paquete(un_paquete);
//} //Resumen: Ver el nombre de las funciones y ver si son lo mismo :D