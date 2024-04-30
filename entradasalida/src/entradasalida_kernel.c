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
            procesar_peticion(peticion->instruccion, peticion->parametros);
            free(peticion);
            enviar_mensaje("Peticion exitosa!", fd_kernel);
            break;
        case RECONOCER_TIPO_INSTRUCCION:
      //      char* tipo_instruccion = recibir_tipo_instruccion(paquete);
      //      validar_tipo_instruccion(tipo_instruccion);    
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

    leer_string_del_paquete(paquete, &peticion->instruccion);
    leer_del_paquete(paquete, (void**)&peticion->parametros->tiempo_espera, sizeof(int));

    return peticion;
} 

void procesar_peticion(char* instruccion, t_peticion_param* parametros) {
      
      if(strcmp(instruccion,"IO_GEN_SLEEP") == 0){
            int tiempo_espera = TIEMPO_UNIDAD_TRABAJO * parametros->tiempo_espera;
            sleep(tiempo_espera);
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
      }else if (strcmp(instruccion,"IO_FS_READ") == 0)
      {
            /* code */
      }else{
            //log_error(entradasalida_logger,"Interfaz %s: Tipo de interfaz no soportado. Proceso enviado a EXIT.\n", NOMBRE_INTERFAZ);
            exit(EXIT_FAILURE);
      }
}      

//char* recibir_tipo_instruccion(t_paquete* paquete){
//      char* tipo_instruccion;
//      leer_string_del_paquete(paquete, &tipo_instruccion);
//return tipo_instruccion;
//}
//
//void validar_tipo_instruccion(char* tipo_instruccion){
//      
//}
//int estaEnLista(int elemento, int lista[], int longitud) {
//    int i;
//    for (i = 0; i < longitud; i++) {
//        if (lista[i] == elemento) {
//            return 1; // Retorna 1 si el elemento está en la lista
//        }
//    }
//    return 0; // Retorna 0 si el elemento no está en la lista
//}
//int longitud = sizeof(lista) / sizeof(lista[0]);
//int lista[] = {1, 2, 3, 4, 5};
//t_list*