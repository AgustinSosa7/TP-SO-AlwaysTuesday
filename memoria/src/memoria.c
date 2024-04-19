#include <stdlib.h>
#include <stdio.h>
#include "../includes/memoria.h"

int main() {

//Inicializar memoria RUKA: Todo lo que estaba acá esta en inicializar memoria. 
inicializar_memoria();

// Iniciar servidor de memoria

fd_memoria = iniciar_servidor(PUERTO_ESCUCHA, memoria_logger, IP_MEMORIA);  //Tercer argumento, ip del server

// Esperar conexion de CPU
log_info(memoria_logger, "Esperando a CPU...");
fd_cpu = esperar_cliente(fd_memoria, memoria_logger,"CPU");
// gestionar_handshake_como_server(fd_memoria, memoria_logger);
server_escucha();

// Esperar conexion de KERNEL
log_info(memoria_logger, "Esperando a Kernel...");
fd_cpu = esperar_cliente(fd_memoria, memoria_logger,"KERNEL"); 

// Esperar conexion de ENTRADASALIDA
log_info(memoria_logger, "Esperando a EntradaSalida...");
fd_entradasalida = esperar_cliente(fd_memoria, memoria_logger, "ENTRADA SALIDA");


// Finalizar MEMORIA

return EXIT_SUCCESS;
}


int server_escucha(){
	server_name = "Memoria";
	log_info(memoria_logger, "Iniciando servidor %s",server_name);
	while(1) {
		int cliente_socket = esperar_cliente(fd_memoria, memoria_logger, server_name );
		if(cliente_socket != -1){
			pthread_t hilo_cliente;
			int *args = malloc(sizeof(int));
			*args = cliente_socket;
			pthread_create(&hilo_cliente, NULL, (void*) saludar_cliente, args); // Pasamos args directamente
			log_info(memoria_logger, "[THREAD] Creo hilo para atender");
			pthread_detach(hilo_cliente);
		}
	}
	return EXIT_SUCCESS;
}
void saludar_cliente(void *void_args){
	int* conexion = (int*) void_args;
	//int cliente_socket = *args;

	int code_op = recibir_operacion(*conexion);
	switch (code_op) {
		case HANDSHAKE:
			void* coso_a_enviar = malloc(sizeof(int));
			int respuesta = 1;
			memcpy(coso_a_enviar, &respuesta, sizeof(int));
			send(*conexion, coso_a_enviar, sizeof(int),0);
			free(coso_a_enviar);

			//procesar_conexion(conexion);
			break;
		case -1:
			log_error(memoria_logger, "Desconexion en HANDSHAKE");
			break;
		default:
			log_error(memoria_logger, "ERROR EN HANDSHAKE: Operacion desconocida");
			break;
	}
}
