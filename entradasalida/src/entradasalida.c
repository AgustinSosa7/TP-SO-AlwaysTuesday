#include <stdlib.h>
#include <stdio.h>
#include "../includes/entradasalida.h"

int main(int argc, char** argv) {

if (argc < 3) {
    fprintf(stderr, "Uso: %s <ruta_archivo_configuracion>\n", argv[0]);
    return EXIT_FAILURE;
}
printf("nombre de interfaz: %s \n",argv[1]);
printf("direccion de config: %s \n",argv[2]);
inicializar_entradasalida(argv[2]);

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
  atender_entradasalida_kernel();


return EXIT_SUCCESS;
}



