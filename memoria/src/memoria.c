#include <stdlib.h>
#include <stdio.h>
#include "../includes/memoria.h"

int main() {

//Inicializar memoria RUKA: Todo lo que estaba acá esta en inicializar memoria. 
inicializar_memoria();

// Iniciar servidor de memoria
fd_memoria = iniciar_servidor(PUERTO_ESCUCHA, memoria_logger, IP_MEMORIA); 

// Esperar conexion de CPU
gestionar_conexion_como_server_memoria(fd_memoria, memoria_logger, "CPU");

//log_info(memoria_logger, "Esperando a CPU...");
//fd_cpu = esperar_cliente(fd_memoria, memoria_logger,"CPU");
//gestionar_handshake_como_server(fd_cpu, memoria_logger);

// Esperar conexion de KERNEL
gestionar_conexion_como_server_memoria(fd_memoria, memoria_logger, "KERNEL");

//log_info(memoria_logger, "Esperando a KERNEL...");
//fd_kernel = esperar_cliente(fd_memoria, memoria_logger,"KERNEL"); 
//gestionar_handshake_como_server(fd_kernel, memoria_logger);

// Esperar conexion de ENTRADASALIDA
gestionar_conexion_como_server_memoria(fd_memoria, memoria_logger, "ENTRADASALIDA");

//log_info(memoria_logger, "Esperando a EntradaSalida...");
//fd_entradasalida = esperar_cliente(fd_memoria, memoria_logger, "ENTRADA SALIDA");
//gestionar_handshake_como_server(fd_entradasalida, memoria_logger);

// Finalizar MEMORIA

return EXIT_SUCCESS;
}



