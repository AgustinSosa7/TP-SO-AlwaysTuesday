#include "../includes/kernel_memoria.h"
#include <../src/utils/utils_connections.h>



void atender_kernel_memoria(){
    bool control_key = 1;
    while (control_key) {
        int cod_op = recibir_operacion(fd_memoria);
        switch (cod_op)
        {
        case MENSAJE:
            //
            break;
        case -1:
          //  log_error(logger, "Desconexion de MEMORIA ");      
            control_key = 0;
        default:
          //  log_warning(logger, "Operacion desconocida de MEMORIA");
            break;
        }
    }
}

void enviar_direccion_pesudocodigo(char* pseudocogido){
      log_info(kernel_log_debug,"creo paquete...");
      t_paquete* paquete = crear_paquete(PSEUDOCODIGO);
      log_info(kernel_log_debug,"agrego el string al paquete...");
      agregar_string_a_paquete(paquete, pseudocogido);  

    /* BORRAR ES PARA LAS PRUEBAS  */

      int tamanio_string = strlen(pseudocogido);
      char *string = malloc((tamanio_string * sizeof(char)) + 1); // En caso de pisar algun valor, hacerle free antes
	    printf("offset:%d\n",paquete->buffer->offset);
	    printf("tam del string (malloc): %ld\n",strlen(string));
	    memcpy(string, paquete->buffer->stream + sizeof(int), tamanio_string); //cambiar por offset el size of int
	    paquete->buffer->offset += strlen(string);//tamanio_string;
      printf("Longitud de lo guardado en el string: %ld\n",strlen(string));
	    printf("offset: %d\n",paquete->buffer->offset);
	    printf("Se guardo en el string:%s\n",string);

    /* FIN DE BORRAR ES PARA LAS PRUEBAS  */

      log_info(kernel_log_debug,"envio el  paquete");
      enviar_paquete(paquete, fd_memoria);
      //log_info(kernel_log_debug,"elimino el paquetede memoria...");
      eliminar_paquete(paquete);
      log_info(kernel_log_debug,"Listo c:");
}