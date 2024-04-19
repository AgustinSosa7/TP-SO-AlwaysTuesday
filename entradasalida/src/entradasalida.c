#include "../includes/entradasalida.h"

int main() {

 inicializar_entradasalida();

//Conectarme como cliente a Memoria
  log_info(entradasalida_logger, "Creando conexion con MEMORIA...");
	fd_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA, entradasalida_logger);
  gestionar_handshake_como_cliente(fd_memoria, "MEMORIA", entradasalida_logger);
  log_warning(entradasalida_logger, "HANDSHAKE CON MEMORIA [EXITOSO]"); 


//Conectarme como cliente a Kernel
  log_info(entradasalida_logger, "Creando conexion con KERNEL...");
	fd_kernel = crear_conexion(IP_KERNEL, PUERTO_KERNEL, entradasalida_logger);
  gestionar_handshake_como_cliente(fd_kernel, "MEMORIA", entradasalida_logger);
  log_warning(entradasalida_logger, "HANDSHAKE CON KERNEL [EXITOSO]"); 

return EXIT_SUCCESS;
}

 
 

	




