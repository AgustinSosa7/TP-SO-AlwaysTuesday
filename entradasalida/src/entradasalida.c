#include <stdlib.h>
#include <stdio.h>
#include "../includes/entradasalida.h"

int main() {

inicializar_entradasalida();

//Conectarme como cliente a Memoria
  log_info(entradasalida_logger, "Creando conexion con MEMORIA...");
  fd_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA, entradasalida_logger);
  gestionar_handshake_como_cliente(fd_memoria, "MEMORIA", entradasalida_logger);


//Conectarme como cliente a Kernel
  log_info(entradasalida_logger, "Creando conexion con KERNEL...");
  fd_kernel = crear_conexion(IP_KERNEL, PUERTO_KERNEL, entradasalida_logger);
  gestionar_handshake_como_cliente(fd_kernel, "KERNEL", entradasalida_logger);

/////////////////////////--ATENDER-MENSAJES--//////////////////////

//Atender los mensajes de KERNEL
    pthread_t hilo_kernel;
    pthread_create(&hilo_kernel, NULL, (void*)atender_entradasalida_kernel, NULL);
    pthread_detach(hilo_kernel);


return EXIT_SUCCESS;
}



