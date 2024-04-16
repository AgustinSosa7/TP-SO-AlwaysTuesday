#include "../includes/entradasalida.h"

int main() {

 inicializar_entradasalida();

//Conectarme como cliente a Memoria
	fd_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);
  log_info(entradasalida_logger, "Conexion con MEMORIA exitosa.");


//Conectarme como cliente a Kernel
	fd_kernel = crear_conexion(IP_KERNEL, PUERTO_KERNEL);
  log_info(entradasalida_logger, "Conexion con KERNEL exitosa.");

return EXIT_SUCCESS;
}

 
 

	




