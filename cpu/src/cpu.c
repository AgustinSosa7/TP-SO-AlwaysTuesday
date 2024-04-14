#include "../includes/cpu.h"
#include <../src/utils/utils_connections.h>

int main() {

inicializar_cpu();

// Iniciar server de CPU - DISPATCH

fd_cpu_dispatch = iniciar_servidor(PUERTO_ESCUCHA_DISPATCH, cpu_logger, "CPU - DISPATCH INICIADO!!");

// Iniciar server de CPU - INTERRUPT

fd_cpu_interrupt= iniciar_servidor(PUERTO_ESCUCHA_INTERRUPT, cpu_logger, "CPU - INTERRUPT INICIADO!!");


// Conectarnos como cliente a MEMORIA

fd_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA); 

log_info(cpu_logger, "Conexion con MEMORIA exitosa!!"); 

// Esperar al cliente KERNEL en su puerto DISPATCH

// Esperar al cliente KERNEL en su puerto INTERRUPT



return EXIT_SUCCESS;
}