#include "utils_connections.h"


// CONEXIONES DE CLIENTE 



void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

int crear_conexion(char* ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	// Ahora vamos a crear el socket.
	//int socket_cliente = 0;
	int socket_cliente = socket(server_info->ai_family,
								server_info->ai_socktype,
								server_info->ai_protocol);

	// Ahora que tenemos el socket, vamos a conectarlo
	connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);

	freeaddrinfo(server_info);

	return socket_cliente;
}

void enviar_mensaje(char* mensaje, int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
}


void crear_buffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete* crear_paquete(void)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = PAQUETE;
	crear_buffer(paquete);
	return paquete;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_paquete* paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}

// CONEXIONES DE SERVIDOR


int iniciar_servidor(char* puerto, t_log* logger, char* ip)
{
	if(puerto == NULL) {
		printf("No encuentra el puerto");
		return -1;
	}

		struct addrinfo hints;
		struct addrinfo *server_info;

		memset(&hints, 0, sizeof(hints));

		hints.ai_family = AF_UNSPEC;    // No importa si uso IPv4 o IPv6
		hints.ai_flags = AI_PASSIVE;		// Asigna el address del localhost: 127.0.0.1
		hints.ai_socktype = SOCK_STREAM;  // Indica que usaremos el protocolo TCP

		getaddrinfo(ip, puerto, &hints, &server_info);  // Si IP es NULL, usa el localhost

		int server_socket = socket(server_info->ai_family, server_info->ai_socktype,server_info->ai_protocol);

		int activado = 1;
		setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));

		if(server_socket == -1 || bind(server_socket, server_info->ai_addr, server_info->ai_addrlen) == -1){
			freeaddrinfo(server_info);
			printf("fallo el bindeo");
			return -1;
		}
		freeaddrinfo(server_info);
		if(listen(server_socket, BACKLOG) == -1){
			printf("fallo el listen");
			return -1;
		}
		return server_socket;
}

int esperar_cliente(int socket_servidor, t_log* logger, const char* msj)
{
	struct sockaddr_in dir_cliente;
    socklen_t tam_direccion = sizeof(struct sockaddr_in);

    int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

    log_info(logger, "Cliente conectado a %s", msj);

    return socket_cliente;
}


int recibir_operacion(int socket_cliente) // @suppress("No return")
{
	int cod_op;

	int bytesRecibidos = recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL);

	if(bytesRecibidos > 0) {
	    if(bytesRecibidos != sizeof(int)) {
	        printf("Recibidos %d bytes, esperaba %zu bytes\n", bytesRecibidos, sizeof(int));
	    }
	    return cod_op;
	} else if (bytesRecibidos == 0) {
		// El cliente cerró la conexión
		printf("El cliente cerró la conexión.\n");
		close(socket_cliente);
		return -1;
	} else {
		// Ocurrió un error al recibir datos
		printf("Error al recibir el codigo de operacion\n");
		close(socket_cliente);
		return -2;
	}

}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

//void recibir_mensaje(int socket_cliente)
//{
//	int size;
//	char* buffer = recibir_buffer(&size, socket_cliente);
//	log_info(logger, "Me llego el mensaje %s", buffer);
//	free(buffer);
//}

t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}

void gestionar_handshake_como_server(int conexion, t_log* logger){
	int code_op = recibir_operacion(conexion);
	printf("codigo de operacion: %d como handhaske servidor \n", code_op);
	switch (code_op) {
		case HANDSHAKE:
			void* coso_a_enviar = malloc(sizeof(int));
			int respuesta = 1;
			memcpy(coso_a_enviar, &respuesta, sizeof(int));
			send(conexion, coso_a_enviar, sizeof(int),0);
			free(coso_a_enviar);

			break;
		case -1:
			log_error(logger, "Desconexion en HANDSHAKE\n");
			exit(EXIT_FAILURE);
			break;
		default:
			log_error(logger, "ERROR EN HANDSHAKE: Operacion desconocida\n");
			exit(EXIT_FAILURE);
			break;
	}
}


void gestionar_handshake_como_cliente(int fd_conexion, char* modulo_destino, t_log* logger){ 
	enviar_handshake(fd_conexion);
	int respuesta_handshake = recibir_operacion(fd_conexion);
	if(respuesta_handshake != 1){
		log_error(logger, "Error en handshake con %s", modulo_destino);
		exit(EXIT_FAILURE);
}
}

void enviar_handshake(int conexion){
	void* coso_a_enviar = malloc(sizeof(int));
	int saludar = HANDSHAKE;
	memcpy(coso_a_enviar, &saludar, sizeof(int));
	send(conexion, coso_a_enviar, sizeof(int),0);
	free(coso_a_enviar);
}

void saludar_cliente(void *void_args, t_log* logger){
	int* conexion = (int*) void_args;

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
			log_error(logger, "Desconexion en HANDSHAKE");
			break;
		default:
			log_error(logger, "ERROR EN HANDSHAKE: Operacion desconocida");
			break;
	}
}