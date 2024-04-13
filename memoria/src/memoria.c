#include <stdlib.h>
#include <stdio.h>
#include <memoria.h>

int main() {

//Inicializar memoria RUKA: Todo lo que estaba acá es´ta en inicializar memoria. 
inicializar_memoria();

// Iniciar servidor de memoria

fd_memoria = iniciar_servidor(PUERTO_ESCUCHA, memoria_logger, "MEMORIA INICIADA!"); // Crear shared

// Esperar conexion de ENTRADASALIDA

// Esperar conexion de CPU

log_info(memoria_logger, "ESperando a CPU...");
fd_cpu = esperar_cliente(fd_memoria, memoria_logger,"CPU"); 

// Esperar conexion de KERNEL

// Finalizar MEMORIA

return EXIT_SUCCESS;
}
