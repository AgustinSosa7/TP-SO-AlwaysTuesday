#include "utils_connections.h"


// CONEXIONES DE CLIENTE 
int crear_conexion(char* ip, char* puerto, t_log* logger)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family,
								server_info->ai_socktype,
								server_info->ai_protocol);

	connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);

	if(socket_cliente == -1){
		log_error(logger, "No se pudo realizar la conexion correctamente.");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(server_info);

	return socket_cliente;
}


void gestionar_handshake_como_cliente(int fd_conexion, const char* modulo_destino, t_log* logger){ 
	enviar_handshake(fd_conexion);
	int respuesta_handshake = recibir_operacion(fd_conexion);
	if(respuesta_handshake == HANDSHAKE){
		log_warning(logger, "HANDSHAKE CON %s [EXITOSO]", modulo_destino);
		enviar_handshake(fd_conexion);
	}else{
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

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}


// CONEXIONES DE SERVIDOR

int iniciar_servidor(char* puerto, t_log* logger, char* ip)
{
	if(puerto == NULL) {
		log_error(logger,"No encuentra el puerto");
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
			log_error(logger,"fallo el bindeo");
			return -1;
		}
		freeaddrinfo(server_info);
		if(listen(server_socket, BACKLOG) == -1){
			log_error(logger,"fallo el listen");
			return -1;
		}
		return server_socket;
}

int esperar_cliente(int socket_servidor, t_log* logger, const char* msj)
{
	struct sockaddr_in dir_cliente;
    socklen_t tam_direccion = sizeof(struct sockaddr_in);

    int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

    log_warning(logger, "Cliente conectado a %s", msj);

    return socket_cliente;
}

void gestionar_handshake_como_server(int conexion, t_log* logger, const char* modulo_destino){
	int code_op = recibir_operacion(conexion);
	switch (code_op) {
		case HANDSHAKE:
			void* coso_a_enviar = malloc(sizeof(int));
			int respuesta = HANDSHAKE;
			memcpy(coso_a_enviar, &respuesta, sizeof(int));
			send(conexion, coso_a_enviar, sizeof(int),0);
			free(coso_a_enviar);
			
			int respuesta_handshake = recibir_operacion(conexion);
			if(respuesta_handshake == HANDSHAKE){
				log_warning(logger, "HANDSHAKE CON %s [EXITOSO]", modulo_destino);
			}else{
				log_error(logger, "Error en handshake con %s", modulo_destino);
				}
	

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



/////////////////////////////// SERIALIZACION ///////////////

void crear_buffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

void eliminar_buffer(t_buffer *buffer){
	free(buffer->stream);
	free(buffer);
};

t_paquete* crear_paquete(op_code code_op)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = code_op;
	crear_buffer(paquete);
	return paquete;
}

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

void enviar_paquete(t_paquete* paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + sizeof(uint32_t) + sizeof(uint8_t);
	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
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

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio);

	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio;
}

void agregar_string_a_paquete(t_paquete* paquete, void* valor)
{
	int tamaño_instruccion = string_length(valor);
    agregar_a_paquete(paquete, valor, tamaño_instruccion);
}


////////////////////// DESERIALIZACION //////////////

int recibir_operacion(int socket_cliente) 
{
	int cod_op;

	int bytesRecibidos = recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL);

	if(bytesRecibidos > 0) {
	    if(bytesRecibidos != sizeof(int)) {
	        printf("Recibidos %d bytes, esperaba %zu bytes\n", bytesRecibidos, sizeof(int));
	    }
	    return cod_op;
	} else if (bytesRecibidos == 0) {
		printf("El cliente cerró la conexión.\n");
		close(socket_cliente);
		return -1;
	} else {
		printf("Error al recibir el codigo de operacion\n");
		close(socket_cliente);
		return -2;
	}

}

t_buffer* recibir_buffer(int unSocket){ 
	t_buffer* buffer = malloc(sizeof(t_buffer));

	recv(unSocket, &(buffer->size), sizeof(uint32_t), 0);
	buffer->stream = malloc(buffer->size);
	recv(unSocket, buffer->stream, buffer->size, 0);

	 return buffer;	
}

t_paquete* recibir_paquete(int unSocket)
{ 
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer = recibir_buffer(unSocket);
	return paquete;
}

char* recibir_mensaje(int socket_cliente)
{	
	char* mensaje;
	t_paquete* paquete = recibir_paquete(socket_cliente);
	leer_string_del_paquete(paquete, &mensaje);
	
	return mensaje;
}

void leer_del_paquete(t_paquete* paquete, void* *valor, int tamanio)
{
	memcpy(*valor, paquete->buffer->stream, tamanio);

	paquete->buffer->stream += tamanio;
}

void leer_string_del_paquete(t_paquete* paquete, char* *valor)
{
	int tamanio;
	leer_del_paquete(paquete, (void**)&tamanio, sizeof(int));
	memcpy(*valor, &paquete->buffer->stream, sizeof(tamanio));

	paquete->buffer->stream += tamanio;
}




//////////////////////// Funciones del Super_Paquete

//t_paquete* crear_super_paquete(op_code code_op)   //Unica diff con el crear_paquete es el cod de op y buffer.
//{
//	t_paquete* super_paquete = malloc(sizeof(t_paquete));
//	super_paquete->codigo_operacion = code_op;
//	crear_buffer(super_paquete);
//	return  super_paquete;
//}  
//
//void cargar_int_al_super_paquete(t_paquete* paquete, int numero){  //Ver que hacer con el off set Ruka
//	if(paquete->buffer->size == 0){
//		paquete->buffer->stream = malloc(sizeof(int));
//		memcpy(paquete->buffer->stream, &numero, sizeof(int));
//	}else{
//		paquete->buffer->stream = realloc(paquete->buffer->stream,
//											paquete->buffer->size + sizeof(int));
//		/**/
//		memcpy(paquete->buffer->stream + paquete->buffer->size, &numero, sizeof(int));
//	}
//
//	paquete->buffer->size += sizeof(int);
//}
//
//void cargar_choclo_al_super_paquete(t_paquete* paquete, void* choclo, int size){
//	if(paquete->buffer->size == 0){
//		paquete->buffer->stream = malloc(sizeof(int) + size);
//		memcpy(paquete->buffer->stream, &size, sizeof(int));
//		memcpy(paquete->buffer->stream + sizeof(int), choclo, size);
//	}else{
//		paquete->buffer->stream = realloc(paquete->buffer->stream,
//												paquete->buffer->size + sizeof(int) + size);
//
//		memcpy(paquete->buffer->stream + paquete->buffer->size, &size, sizeof(int));
//		memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), choclo, size);
//	}
//
//	paquete->buffer->size += sizeof(int);
//	paquete->buffer->size += size;
//}