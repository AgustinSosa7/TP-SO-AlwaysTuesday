#include <stdlib.h>
#include <stdio.h>
#include "../includes/memoria.h"

int main() {

//Inicializar MEMORIA
inicializar_memoria();

// Iniciar servidor de memoria
fd_memoria = iniciar_servidor(PUERTO_ESCUCHA, memoria_logger, IP_MEMORIA); 

// Esperar conexion de CPU
gestionar_conexion_como_server_memoria(fd_memoria, memoria_logger, "CPU");

// Esperar conexion de KERNEL
gestionar_conexion_como_server_memoria(fd_memoria, memoria_logger, "KERNEL");

// Esperar conexion de ENTRADASALIDA
gestionar_conexion_como_server_memoria(fd_memoria, memoria_logger, "ENTRADASALIDA");

// Finalizar MEMORIA

return EXIT_SUCCESS;
}



