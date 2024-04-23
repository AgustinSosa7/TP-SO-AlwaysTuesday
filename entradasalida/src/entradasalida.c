#include "../includes/entradasalida.h"

int main() {

 inicializar_entradasalida();

//Conectarme como cliente a Memoria
gestionar_conexion_como_cliente(IP_MEMORIA, PUERTO_MEMORIA, entradasalida_logger, "MEMORIA");

//Conectarme como cliente a Kernel
gestionar_conexion_como_cliente(IP_KERNEL, PUERTO_KERNEL, entradasalida_logger, "KERNEL");

return EXIT_SUCCESS;
}

 
 

	




