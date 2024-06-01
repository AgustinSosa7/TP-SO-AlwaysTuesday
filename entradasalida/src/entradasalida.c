#include <stdlib.h>
#include <stdio.h>
#include "../includes/entradasalida.h"

int main(int argc, char** argv) {

if(validar_parametros_incorrectos_main(argc, 3, argv)){
    return EXIT_FAILURE;
 }

inicializar_entradasalida(argv);

//Conectarme como cliente a Memoria
  log_info(entradasalida_logger, "Creando conexion con MEMORIA...");
  fd_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA, entradasalida_logger);
  gestionar_handshake_como_cliente(fd_memoria, "MEMORIA", entradasalida_logger);

//Conectarme como cliente a Kernel
  log_info(entradasalida_logger, "Creando conexion con KERNEL...");
  fd_kernel = crear_conexion(IP_KERNEL, PUERTO_KERNEL, entradasalida_logger);
  gestionar_handshake_como_cliente(fd_kernel, "KERNEL", entradasalida_logger);
  enviar_mensaje_string(NOMBRE_INTERFAZ, fd_kernel);

/////////////////////////--ATENDER-MENSAJES--//////////////////////

//Atender los mensajes de KERNEL
  atender_entradasalida_kernel();


return EXIT_SUCCESS;
}



