#include "utils_connections.h"

// Funciones de Listas

bool contains_string(t_list* lista, char* elemento){
	bool funcion_de_ayuda(void* elemento_posible){
            return son_el_mismo_string(elemento_posible, elemento);
      	}
      return list_any_satisfy(lista, funcion_de_ayuda);
}

bool son_el_mismo_string(char* elemento_posible, char* elemento){
      return (strcmp(elemento_posible, elemento)==0);
}

bool contains_algo(t_list* lista, void* elemento){
	bool funcion_de_ayuda(void* elemento_posible){
            return son_lo_mismo(elemento_posible, elemento);
      	}

      return list_any_satisfy(lista, funcion_de_ayuda);
}

bool son_lo_mismo(void* elemento_posible, void* elemento){
      return (elemento_posible == elemento);
}


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
	//gestionar error de send aca. Para cuando un cliente se desconecta repentinamente.
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
			enviar_handshake(conexion);
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

void enviar_mensaje(void* mensaje, int socket_cliente)
{
	int tamanio = sizeof(mensaje);
	void* a_enviar = malloc(tamanio);
	memcpy(a_enviar, &mensaje, tamanio);
	send(socket_cliente, a_enviar, tamanio, 0);
	free(a_enviar);
}

void enviar_mensaje_string(char* mensaje, int socket_cliente)
{
	int tamanio = strlen(mensaje)+1;
	void* a_enviar = malloc(tamanio);
	memcpy(a_enviar, &mensaje, tamanio);
	send(socket_cliente, a_enviar, tamanio, 0);
	free(a_enviar);
}

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void agregar_algo_a_paquete(t_paquete* paquete, void* valor)
{
	size_t tamanio = sizeof(valor);
	paquete->buffer->size += tamanio;
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size); //Agranda el tamanio del stream
	memcpy(paquete->buffer->stream + paquete->buffer->offset, &valor, tamanio);
	paquete->buffer->offset += tamanio; 
}

void agregar_string_a_paquete(t_paquete* paquete, char* valor) 
{
	int tamanio = strlen(valor) + 1;  // +1 por el /0
	agregar_algo_a_paquete(paquete, &tamanio);

	paquete->buffer->size += tamanio;
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size);
	memcpy(paquete->buffer->stream + paquete->buffer->offset, &valor, tamanio);
	paquete->buffer->offset += tamanio; 
}

void agregar_registro_a_paquete(t_paquete* un_paquete, t_cpu* registros_CPU){
agregar_algo_a_paquete(un_paquete,registros_CPU->PC);
agregar_algo_a_paquete(un_paquete,registros_CPU->AX);
agregar_algo_a_paquete(un_paquete,registros_CPU->BX);
agregar_algo_a_paquete(un_paquete,registros_CPU->CX);
agregar_algo_a_paquete(un_paquete,registros_CPU->DX);
agregar_algo_a_paquete(un_paquete,registros_CPU->EAX);
agregar_algo_a_paquete(un_paquete,registros_CPU->EBX);
agregar_algo_a_paquete(un_paquete,registros_CPU->ECX);
agregar_algo_a_paquete(un_paquete,registros_CPU->EDX);
agregar_algo_a_paquete(un_paquete,registros_CPU->SI);
agregar_algo_a_paquete(un_paquete,registros_CPU->DI);
}

/////////////////////// PCB /////////////////////////

void enviar_pcb_a(t_pcb* un_pcb, int socket){
	t_paquete* un_paquete = crear_paquete(PCB); //Ejecutar, ver si tiene ese nombre;
	agregar_algo_a_paquete(un_paquete, un_pcb->pid);
    //agregar_algo_a_paquete(un_paquete, &un_pcb->program_counter);elPC debe ir en el registro de CPU ????
    agregar_algo_a_paquete(un_paquete,un_pcb->QUANTUM);
    agregar_registro_a_paquete(un_paquete, &un_pcb->registros_CPU);
	enviar_paquete(un_paquete, socket);
	eliminar_paquete(un_paquete);
}

t_pcb* recibir_pcb(t_paquete* paquete, t_log*logger){
	void* stream = paquete->buffer->stream;
	t_pcb* pcb = malloc(sizeof(t_pcb));
	leer_algo_del_stream(stream, pcb->pid);
	log_info(logger, pcb->pid);
	//leer_algo_del_paquete(stream, &un_pcb->program_counter); elPC debe ir en el registro de CPU ????
	leer_algo_del_stream(stream,&pcb->QUANTUM);
	leer_registro_del_stream(stream, &(pcb->registros_CPU));
	free(stream);
	return pcb; //Devuelvo PCB
}

void imprimir_pcb(t_pcb* pcb, t_log* un_logger){
log_info(un_logger,"PCB Pid %d", pcb->pid);
log_info(un_logger,"PCB Quantum %d",pcb->QUANTUM);	
log_info(un_logger,"PCB PC: %d ", pcb->registros_CPU.PC);
log_info(un_logger,"PCB AX: %d ", pcb->registros_CPU.AX);
log_info(un_logger,"PCB BX: %d ", pcb->registros_CPU.BX);
log_info(un_logger,"PCB CX: %d ", pcb->registros_CPU.CX);
log_info(un_logger,"PCB DX: %d ", pcb->registros_CPU.DX);
log_info(un_logger,"PCB EAX: %d ", pcb->registros_CPU.EAX);
log_info(un_logger,"PCB EBX: %d ", pcb->registros_CPU.EBX);
log_info(un_logger,"PCB ECX: %d ", pcb->registros_CPU.ECX);
log_info(un_logger,"PCB EDX: %d ", pcb->registros_CPU.EDX);
log_info(un_logger,"PCB SI: %d ", pcb->registros_CPU.SI);
log_info(un_logger,"PCB DI: %d ", pcb->registros_CPU.DI);
}

////////////////////// DESERIALIZACION //////////////

int recibir_operacion(int socket_cliente) 
{
	int cod_op;

	int bytesRecibidos = recv(socket_cliente, &cod_op, sizeof(op_code), MSG_WAITALL);

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

void* recibir_mensaje(int socket_cliente)
{	
	void* mensaje = malloc(sizeof(void));
	t_paquete* paquete = recibir_paquete(socket_cliente);
	leer_string_del_stream(paquete->buffer->stream, mensaje);
	
	return mensaje;
}

char* recibir_mensaje_string(int socket_cliente)
{	
	int tamanio;
	recv(socket_cliente, &tamanio, sizeof(int), MSG_WAITALL);
	char* mensaje = malloc(tamanio);
	recv(socket_cliente, mensaje, tamanio, MSG_WAITALL);
	
	return mensaje;
}

void leer_algo_del_stream(void* stream, void* valor)
{
	size_t tamanio = sizeof(valor);
	memcpy(valor, stream, tamanio);
	stream += tamanio;
}

void leer_string_del_stream(void* stream, char* valor) 
{
	int tamanio;
	leer_algo_del_stream(stream, &tamanio); 
	valor = malloc(tamanio); // En caso de pisar algun valor, hacerle free antes
	leer_algo_del_stream(stream, &valor);
}

void leer_registro_del_stream(void* stream, t_cpu* registros_CPU){
leer_algo_del_stream(stream,&registros_CPU->PC);
leer_algo_del_stream(stream,&registros_CPU->AX);
leer_algo_del_stream(stream,&registros_CPU->BX);
leer_algo_del_stream(stream,&registros_CPU->CX);
leer_algo_del_stream(stream,&registros_CPU->DI);
leer_algo_del_stream(stream,&registros_CPU->DX);
leer_algo_del_stream(stream,&registros_CPU->EAX);
leer_algo_del_stream(stream,&registros_CPU->EBX);
leer_algo_del_stream(stream,&registros_CPU->ECX);
leer_algo_del_stream(stream,&registros_CPU->EDX);
leer_algo_del_stream(stream,&registros_CPU->SI);
leer_algo_del_stream(stream,&registros_CPU->DI);
}

//pcb->pid

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