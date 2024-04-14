#include <stdlib.h>
#include <stdio.h>
#include "../includes/memoria.h"

int main() {

//Inicializar memoria RUKA: Todo lo que estaba acá esta en inicializar memoria. 
inicializar_memoria();

// Iniciar servidor de memoria

fd_memoria = iniciar_servidor(PUERTO_ESCUCHA, memoria_logger, "MEMORIA INICIADA!"); 

// Esperar conexion de ENTRADASALIDA
log_info(memoria_logger, "Esperando a EntradaSalida...");
//fd_entradasalida = esperar_cliente(fd_memoria, memoria_logger, "ENTRADA SALIDA");

// Esperar conexion de CPU

log_info(memoria_logger, "ESperando a CPU...");
//fd_cpu = esperar_cliente(fd_memoria, memoria_logger,"CPU"); 

// Esperar conexion de KERNEL

// Finalizar MEMORIA

return EXIT_SUCCESS;
}
