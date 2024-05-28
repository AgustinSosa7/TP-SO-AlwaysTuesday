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


t_interfaz* validar_peticion(t_peticion* peticion, t_pcb* pcb){ 
      char* nombre_io = peticion->interfaz;
      char* instruccion = peticion->instruccion;

    t_interfaz* interfaz = existe_la_interfaz(nombre_io, pcb);
    if(esta_conectada_la_interfaz(interfaz, pcb)){
    validar_interfaz_admite_instruccion(interfaz, instruccion); 
    }else{
    //enviar_proceso_a_exit();
    }
    return interfaz;
}
t_interfaz* existe_la_interfaz(char* nombre_io, t_pcb* pcb){
    bool esta_la_io(void* interfaz){
        return (strcmp(nombre_io,interfaz->nombre) == 0);
    }
    t_interfaz* interfaz = list_find(IOS_CONECTADOS, esta_la_io(io, nombre_io)); 

    if(interfaz != NULL){ //Es correcta esta condicion?
        return interfaz;
    }else{
            //enviar_proceso_a_exit();
          }
}         
bool esta_conectada_la_interfaz(t_interfaz* interfaz, t_pcb* pcb){
    t_paquete* paquete = crear_paquete(ESTOY_CONECTADO); 
    agregar_algo_a_paquete(paquete, true);
    int bytes = sizeof(uint32_t) * 2 + sizeof(bool);
	void* a_enviar = serializar_paquete(paquete, bytes);
	int err = send(fd_entradasalida, a_enviar, bytes, SIGPIPE);
	free(a_enviar);      
    eliminar_paquete(paquete);

    if(err == -1){
        log_error(kernel_logger,"La interfaz %s no se encuentra conectada.", interfaz->nombre);
        //enviar_proceso_a_exit();
        return false;
    }else{
        bool respuesta = recibir_mensaje(fd_entradasalida);
        return respuesta;
    }

}

void validar_interfaz_admite_instruccion(char* interfaz, char* instruccion, t_pcb* pcb){
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
      enviar_paquete(paquete, interfaz->fd_interfaz);
      eliminar_paquete(paquete);
} 

void recibir_fin_peticion(){
    bool fin_peticion;
    recv(fd_entradasalida, &fin_peticion, sizeof(bool), MSG_WAITALL);
 
}

void gestionar_lista_de_interfaz(t_peticion* peticion, t_interfaz* interfaz){

    // si la interfaz no tiene procesos bloqueados, lo ejecuto
    // si la interfaz tiene procesos bloqueados, lo agrego a la lista

    // Hilo que maneja la cola de bloqueados:
    // la cola de execute es un while(1) que se le avisa que ingreso un proceso a su cola mediante 
    // un semaforo; este procede a hacerle pop, mandarlo a la cpu, esperar a recibir el contexto actualizado 
    // y asi sucesivamente.

 //   while(1){
 //       wait(io->semaforo_cola_procesos_blocked);
 //       pcb = pop(io->cola_procesos_blocked);        
 //   }
//
 //   // wait(se_libero_io); se inicializa en 1
 //   enviar_peticion_a_interfaz(peticion, interfaz);
}



struct{
    char* nombre;
    int fd_entradasalida;
    t_queue cola_procesos_blocked;
}t_interfaz

struct{
    char* nombre;
    int fd_entradasalida;
    t_queue cola_procesos_blocked;
}bloqueados
// int1 ----------------fd_entradasalida--------------- kernel -------------------cpu
// 
//                     p3 sleep 1500                           
//                                                      
//                                                      p4 sleep 4000