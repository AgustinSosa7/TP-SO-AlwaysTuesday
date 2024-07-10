#include <stdlib.h>
#include <stdio.h>
#include "../includes/entradasalida.h"

int main(int argc, char** argv) {

if(validar_parametros_incorrectos_main(argc, 2, argv)){
    return EXIT_FAILURE;
 }

inicializar_entradasalida(argv);

if((strcmp(TIPO_INTERFAZ, "STDOUT") == 0) || (strcmp(TIPO_INTERFAZ, "STDIN") == 0)|| (strcmp(TIPO_INTERFAZ, "FS") == 0)){
//Conectarme como cliente a Memoria
  fd_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA, entradasalida_logger);
  gestionar_handshake_como_cliente(fd_memoria, "MEMORIA", entradasalida_logger);
}
//Conectarme como cliente a Kernel
  log_info(entradasalida_logger, "Creando conexion con KERNEL...");
  fd_kernel = crear_conexion(IP_KERNEL, PUERTO_KERNEL, entradasalida_logger);
  gestionar_handshake_como_cliente(fd_kernel, "KERNEL", entradasalida_logger);
  
  enviar_mensaje_string(NOMBRE_INTERFAZ, fd_kernel);
  enviar_mensaje_string(TIPO_INTERFAZ, fd_kernel);

/////////////////////////--ATENDER-MENSAJES--//////////////////////

//Atender los mensajes de KERNEL
  atender_entradasalida_kernel();


return EXIT_SUCCESS;
}



