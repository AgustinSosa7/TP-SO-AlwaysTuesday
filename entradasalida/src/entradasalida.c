#include "../includes/entradasalida.h"

int main() {

 inicializar_entradasalida();

//Conectarme como cliente a Memoria
	fd_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);
  gestionar_handshake_como_cliente(fd_memoria, "MEMORIA", entradasalida_logger);
  log_warning(entradasalida_logger, "HANDSHAKE CON MEMORIA [EXITOSO]"); 


//Conectarme como cliente a Kernel
	fd_kernel = crear_conexion(IP_KERNEL, PUERTO_KERNEL);
  gestionar_handshake_como_cliente(fd_kernel, "MEMORIA", entradasalida_logger);
  log_warning(entradasalida_logger, "HANDSHAKE CON KERNEL [EXITOSO]"); 

return EXIT_SUCCESS;
}

 
 

	




