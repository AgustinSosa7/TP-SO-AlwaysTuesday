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

///////////////////////////////
bool validar_parametros_incorrectos_main(int argc, int parametros_esperados, char** argv){
	if (argc < parametros_esperados) {
	    fprintf(stderr, "Uso: %s <ruta_archivo_configuracion>\n", argv[0]);
	    return true;
	}else{
		return false;
	}
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
	paquete->buffer->offset = 0;
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

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(op_code));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

void enviar_paquete(t_paquete* paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + sizeof(op_code) + sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);

	/* BORRAR ES PARA LAS PRUEBAS  

      	int tamanio_string = 13;
      	char *string = malloc((tamanio_string * sizeof(char))); // En caso de pisar algun valor, hacerle free antes
	    memcpy(string, a_enviar + sizeof(int) + sizeof(int) + sizeof(int), tamanio_string); //cambiar por offset el size of int
	    //paquete->buffer->offset += strlen(string);//tamanio_string;
        printf("Longitud de lo guardado en el string: %ld\n",strlen(string));
	    printf("offset: %d\n",paquete->buffer->offset);
	    printf("Se guardo en el string:%s\n",string);
        
    FIN DE BORRAR ES PARA LAS PRUEBAS  */

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
	//free(paquete->buffer->stream);
	//free(paquete->buffer);
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

void agregar_string_a_paquete(t_paquete* paquete, char* string) 
{
	/*int tamanio = strlen(valor) + 1;  // +1 por el /0
	agregar_algo_a_paquete(paquete, &tamanio);
	paquete->buffer->size += tamanio;
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size);
	memcpy(paquete->buffer->stream + paquete->buffer->offset, &valor, tamanio);
	paquete->buffer->offset += tamanio; */
	
	int tamanio_string = strlen(string) + 1;

	paquete->buffer->stream = realloc(paquete->buffer->stream,
															paquete->buffer->size + sizeof(int) + sizeof(char)*tamanio_string);
	
	//paquete->buffer->size + sizeof(int) + sizeof(char)*tamanio_string;
	/**/
	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio_string, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), string, sizeof(char)*tamanio_string);

	

	paquete->buffer->size += sizeof(int);
	paquete->buffer->size += sizeof(char)*tamanio_string;
	
}

void agregar_registro_a_paquete(t_paquete* paquete, t_registros_cpu* registros_cpu){
agregar_algo_a_paquete(paquete,&registros_cpu->PC);
agregar_algo_a_paquete(paquete,&registros_cpu->AX);
agregar_algo_a_paquete(paquete,&registros_cpu->BX);
agregar_algo_a_paquete(paquete,&registros_cpu->CX);
agregar_algo_a_paquete(paquete,&registros_cpu->DX);
agregar_algo_a_paquete(paquete,&registros_cpu->EAX);
agregar_algo_a_paquete(paquete,&registros_cpu->EBX);
agregar_algo_a_paquete(paquete,&registros_cpu->ECX);
agregar_algo_a_paquete(paquete,&registros_cpu->EDX);
agregar_algo_a_paquete(paquete,&registros_cpu->SI);
agregar_algo_a_paquete(paquete,&registros_cpu->DI);
}

/////////////////////// PCB /////////////////////////

void enviar_pcb_a(t_pcb* un_pcb, int socket){
	t_paquete* un_paquete = crear_paquete(PCB); //Ejecutar, ver si tiene ese nombre;
	agregar_algo_a_paquete(un_paquete, &(un_pcb->pid));
    agregar_algo_a_paquete(un_paquete,&(un_pcb->quantum));
    agregar_registro_a_paquete(un_paquete, un_pcb->registros_cpu);
	agregar_algo_a_paquete(un_paquete,&(un_pcb->estado_pcb));
	enviar_paquete(un_paquete, socket);
	eliminar_paquete(un_paquete);
}

t_pcb* recibir_pcb(t_paquete* paquete){
	void* buffer = paquete->buffer;
	t_pcb* pcb = malloc(sizeof(t_pcb));
	leer_algo_del_stream(buffer, &(pcb->pid), sizeof(int));
	leer_algo_del_stream(buffer,&(pcb->quantum),sizeof(int));
	leer_registros_del_stream(buffer, pcb->registros_cpu);
	leer_algo_del_stream(buffer,&(pcb->estado_pcb),sizeof(pcb->estado_pcb));
	return pcb; 
}

void imprimir_pcb(t_pcb* pcb, t_log* un_logger){
log_info(un_logger,"PCB Pid %d", pcb->pid);
log_info(un_logger,"PCB Quantum %d",pcb->quantum);	
log_info(un_logger,"PCB PC: %d ", pcb->registros_cpu->PC);
log_info(un_logger,"PCB AX: %d ", pcb->registros_cpu->AX);
log_info(un_logger,"PCB BX: %d ", pcb->registros_cpu->BX);
log_info(un_logger,"PCB CX: %d ", pcb->registros_cpu->CX);
log_info(un_logger,"PCB DX: %d ", pcb->registros_cpu->DX);
log_info(un_logger,"PCB EAX: %d ", pcb->registros_cpu->EAX);
log_info(un_logger,"PCB EBX: %d ", pcb->registros_cpu->EBX);
log_info(un_logger,"PCB ECX: %d ", pcb->registros_cpu->ECX);
log_info(un_logger,"PCB EDX: %d ", pcb->registros_cpu->EDX);
log_info(un_logger,"PCB SI: %d ", pcb->registros_cpu->SI);
log_info(un_logger,"PCB DI: %d ", pcb->registros_cpu->DI);
//log_info(un_logger,"PCB Estado: %s",pcb->estado_pcb);
}

char* enum_a_string(estado_pcb estado){
    switch (estado)
    {
      case NEW:
        return "NEW";
        break;
        case READY:
        return "READY";
        break;
      case EXEC:
        return "EXEC";
        break;
      case EXIT:
        return "EXIT";
        break;
      case BLOCKED:
        return "BLOCKED";
        break;
	  case READYPLUS:
        return "READYPLUS";
        break;
	}
}
    
////////////////////// DESERIALIZACION //////////////

op_code recibir_operacion(int socket_cliente) 
{
	op_code cod_op;

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

t_buffer* recibir_buffer(int unSocket)
{ 
	t_buffer* buffer = malloc(sizeof(t_buffer));
	recv(unSocket, &(buffer->size), sizeof(int), MSG_WAITALL);
	buffer->stream = malloc(buffer->size);
	recv(unSocket, buffer->stream, buffer->size, MSG_WAITALL);
	
	return buffer;	
}

//INT

t_paquete* recibir_paquete(int unSocket)
{ 
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer = recibir_buffer(unSocket);
	paquete->buffer->offset = 0;
	return paquete;
}

void* recibir_mensaje(int socket_cliente)
{	
	int tamanio_mensaje;
	recv(socket_cliente, &tamanio_mensaje, sizeof(int), MSG_WAITALL);
	void* mensaje = malloc(tamanio_mensaje);
	recv(socket_cliente, mensaje, tamanio_mensaje, MSG_WAITALL);
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

void leer_algo_del_stream(t_buffer* buffer, void* valor, int tamanio)
{
	memcpy(&valor, buffer->stream + buffer->offset, tamanio);
	buffer->offset += tamanio;
}

char* leer_string_del_stream(t_buffer* buffer) 
{
	int tamanio_string;
	leer_algo_del_stream(buffer, &tamanio_string,sizeof(tamanio_string));

	char *string = malloc(tamanio_string); // En caso de pisar algun valor, hacerle free antes
	leer_algo_del_stream(buffer, string, tamanio_string);

	return string;
}

void leer_registros_del_stream(void* stream, t_registros_cpu* registros_CPU){
	leer_algo_del_stream(stream,&registros_CPU->PC,sizeof(registros_CPU->PC));
	leer_algo_del_stream(stream,&registros_CPU->AX,sizeof(registros_CPU->AX));
	leer_algo_del_stream(stream,&registros_CPU->BX,sizeof(registros_CPU->BX));
	leer_algo_del_stream(stream,&registros_CPU->CX,sizeof(registros_CPU->CX));
	leer_algo_del_stream(stream,&registros_CPU->DX,sizeof(registros_CPU->DX));
	leer_algo_del_stream(stream,&registros_CPU->EAX,sizeof(registros_CPU->EAX));
	leer_algo_del_stream(stream,&registros_CPU->EBX,sizeof(registros_CPU->EBX));
	leer_algo_del_stream(stream,&registros_CPU->ECX,sizeof(registros_CPU->ECX));
	leer_algo_del_stream(stream,&registros_CPU->EDX,sizeof(registros_CPU->EDX));
	leer_algo_del_stream(stream,&registros_CPU->SI,sizeof(registros_CPU->SI));
	leer_algo_del_stream(stream,&registros_CPU->DI,sizeof(registros_CPU->DI));
}

