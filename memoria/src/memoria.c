#include <stdlib.h>
#include <stdio.h>
#include <memoria.h>

int main() {

//Inicializar memoria
memoria_logger = log_create("memoria.log","MEMORIA_LOG",1,LOG_LEVEL_INFO);
if (memoria_logger == NULL) {
	perror("Algo raro pasa con el log. No se pudo crear o encontrar el archivo.");
	exit(EXIT_FAILURE);
	}


memoria_log_debug = log_create("memoria.log","MEMORIA_LOG",1,LOG_LEVEL_TRACE);
if (memoria_log_debug == NULL) {
	perror("Algo raro pasa con el log. No se pudo crear o encontrar el archivo.");
	exit(EXIT_FAILURE);
	}

memoria_config = config_create("home/utnso/tp-2024-1c-AlwaysTuesday/memoria/memoria.config");
if (memoria_config == NULL) {
	perror("Error al intentar cargar el config.");
	exit(EXIT_FAILURE);
	}

PUERTO_ESCUCHA = config_get_int_value(memoria_config,"PUERTO_ESCUCHA");
TAM_MEMORIA = config_get_int_value(memoria_config,"TAM_MEMORIA");
TAM_PAGINA = config_get_int_value(memoria_config,"TAM_PAGINA");
PATH_INSTRUCCIONES = config_get_string_value(memoria_config,"PATH_INSTRUCCIONES");
RETARDO_RESPUESTA = config_get_int_value(memoria_config,"RETARDO_RESPUESTA");

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
