#include "../includes/consola.h"

void iniciar_consola(){
	lista_instrucciones = list_create();
	agregar_instruccion(lista_instrucciones,EJECUTAR_SCRIPT,1,"EJECUTAR_SCRIPT");
	agregar_instruccion(lista_instrucciones,INICIAR_PROCESO,1,"INICIAR_PROCESO");
	agregar_instruccion(lista_instrucciones,FINALIZAR_PROCESO,1,"FINALIZAR_PROCESO");
	agregar_instruccion(lista_instrucciones,DETENER_PLANIFICACION,0,"DETENER_PLANIFICACION");
	agregar_instruccion(lista_instrucciones,INICIAR_PLANIFICACION,0,"INICIAR_PLANIFICACION");
	agregar_instruccion(lista_instrucciones,MULTIPROGRAMACION,1,"MULTIPROGRAMACION");
	agregar_instruccion(lista_instrucciones,PROCESO_ESTADO,0,"PROCESO_ESTADO");
};

void agregar_instruccion(t_list* lista_instrucciones, op_code_instruccion op_code, int parametros,char* op_code_string){
	t_instruccion* instruccion = malloc(sizeof(t_instruccion));
	instruccion->op_code_instruccion = op_code;
	instruccion->cant_parametros = parametros;
	instruccion->nombre = malloc((sizeof(char) *strlen(op_code_string))+1);
	memcpy(instruccion->nombre, op_code_string,(sizeof(char) *strlen(op_code_string))+1);
	list_add(lista_instrucciones,instruccion);

}


void leer_consola(){
    pthread_t hilo_consola;
    pthread_create(&hilo_consola, NULL, (void*) leer_comandos, NULL);
	pthread_join(hilo_consola,NULL);
	printf("finalizo la consola\n");
	finalizar_consola();

};

void leer_comandos(){

    char* leido;
	leido = readline("> ");
	while(strcmp(leido,"\0") != 0){
		if(validar_instruccion(leido)){
			printf("Comando válido\n");
			atender_instruccion_validada(leido);
		}

		free(leido);
		leido = readline("> ");
	}
	free(leido);
}

bool validar_instruccion(char* leido){
   	char** array_leido = string_split(leido," ");
	int size_parametros = string_array_size(array_leido) - 1;
	char* nombre_instruccion= array_leido[0];
	string_array_destroy(array_leido);
	return (validar_nombre_y_parametros(nombre_instruccion,size_parametros));


}

bool validar_nombre_y_parametros(char* nombre_instruccion,int cant_parametros) {

    bool esta_la_instruccion(void* instruccion) {
      return esta_o_noo(nombre_instruccion, cant_parametros, instruccion);
    }

    return (list_any_satisfy(lista_instrucciones, esta_la_instruccion)); 
}


bool esta_o_noo(char* nombre_instruccion, int cant_parametros, t_instruccion* instruccion){ 
		
        if(strcmp(nombre_instruccion, instruccion->nombre) == 0){  
			//printf("parametros posibles:%d \n",instruccion->cant_parametros);
			//printf("parametros recibidos:%d \n",cant_parametros);
			return (instruccion->cant_parametros == cant_parametros);

		} else return false; 
}
	


void atender_instruccion_validada(char* leido){
	char** array_leido = string_split(leido," ");
	char* nombre_instruccion = array_leido[0];
	bool encontrar_instruccion(void* instruccion){
		return (estaa_o_no(instruccion, nombre_instruccion));
	}
	printf("voy a buscar la instruccion:\n");
	t_instruccion* instruccion = list_find(lista_instrucciones,encontrar_instruccion);
	op_code_instruccion op_code_encontrado = instruccion->op_code_instruccion;
	printf("codigo encontrado: %d \n",op_code_encontrado);
	switch (op_code_encontrado)
	{
	case EJECUTAR_SCRIPT:
		t_list* lista_comandos = leer_archivo(array_leido[1]);
		while(!list_is_empty(lista_comandos)){
			char* comando = list_remove(lista_comandos,0);
			atender_instruccion_validada(comando);
		}
		list_destroy(lista_comandos);
		break;
	case INICIAR_PROCESO:
		printf("Entre a iniciar proceso. \n");
		t_pcb* nuevo_pcb = crearPcb();
		enviar_path_a_memoria(array_leido[1],nuevo_pcb->pid,fd_memoria);
		//verificar si memoria creo el proceso 
		sleep(2);
		pthread_mutex_lock(&(struct_new->mutex));
		list_add(struct_new->lista, nuevo_pcb);
		pthread_mutex_unlock(&(struct_new->mutex));	
		log_warning(kernel_logger,"Se crea el proceso < %d > en NEW. \n",nuevo_pcb->pid);
		sem_post(&sem_new_a_ready);
		
    
		break;
	case FINALIZAR_PROCESO: 
		int pid = atoi(array_leido[1]);
	 	t_pcb* pcb = buscar_pcb(pid);
		if(pcb != NULL){
			estado_pcb estado_anterior = pcb->estado_pcb;
			if(contains_algo(struct_exec->lista, &(pcb->pid))){
				enviar_interrupción_a_cpu(SOLICITUD_INTERRUMPIR_PROCESO, INTERRUPCION_POR_KILL);
				printf("Envie interrupcion para finalizar proceso\n");	
			} else if(pcb->estado_pcb == EXIT){
				eliminar_proceso(pcb,INTERRUPTED_BY_USER);
				} else if(pcb->estado_pcb != BLOCKED){
					t_listas_estados* lista_encontrada = buscar_lista(estado_anterior);
					list_remove_element(lista_encontrada->lista,pcb);
					pcb->estado_pcb = EXIT;
					eliminar_proceso(pcb,INTERRUPTED_BY_USER);
					log_warning(kernel_logger,"Cambio de Estado: PID: <%d> - Estado Anterior: <%s> - Estado Actual: <%s> \n",pcb->pid, enum_a_string(estado_anterior),enum_a_string(pcb->estado_pcb));

				
			} else{
				pcb->estado_pcb = EXIT;
				pthread_mutex_lock(&(struct_exit->mutex));
				list_add(struct_exit->lista,pcb);
				pthread_mutex_unlock(&(struct_exit->mutex));
				eliminar_proceso(pcb,INTERRUPTED_BY_USER);
				log_warning(kernel_logger,"Cambio de Estado: PID: <%d> - Estado Anterior: <%s> - Estado Actual: <%s> \n",pcb->pid, enum_a_string(estado_anterior),enum_a_string(pcb->estado_pcb));
			}
		} 	else{
			printf("No se encontró el pcb con PID: %d \n", pid);
			}
		break;
	case DETENER_PLANIFICACION:
			pthread_mutex_lock(&mutex_detener_planificacion);
			if(flag_detener_planificacion ){
				printf("la planificación ya se encuentra pausada \n");
			}
			flag_detener_planificacion = true;
			pthread_mutex_unlock(&mutex_detener_planificacion);
		break;
	case INICIAR_PLANIFICACION:
			pthread_mutex_lock(&mutex_detener_planificacion);
			if(flag_detener_planificacion){
			flag_detener_planificacion = false;
			pthread_mutex_unlock(&mutex_detener_planificacion);
		}
		break;
	case MULTIPROGRAMACION:
		int nuevo_valor = atoi(array_leido[1]);
		int anterior_valor = GRADO_MULTIPROGRAMACION;
		int valor;
		if(nuevo_valor >= 1){
			GRADO_MULTIPROGRAMACION = nuevo_valor;
			valor = nuevo_valor - anterior_valor;
			if(valor > 0){
				for (int i = 0; i < valor; i++)
				{
					sem_post(&sem_grado_multiprogram);
				} 	
				}else {
					valor = anterior_valor - nuevo_valor;
					for (int i = 0; i < valor; i++)
					{
					sem_wait(&sem_grado_multiprogram);
					}			
		    }
			printf("Grado de multiprogramación anterior: %d - Grado de multiprogramación actual: %d \n",anterior_valor,nuevo_valor);
		} else {
			log_error(kernel_logger,"El grado de multiprogramacion %d no es válido \n",nuevo_valor);
		}	
		break;
	case PROCESO_ESTADO:
		pthread_mutex_lock(&(struct_new->mutex));
		imprimir_lista(struct_new);
		pthread_mutex_unlock(&(struct_new->mutex));

		pthread_mutex_lock(&(struct_ready->mutex));
		imprimir_lista(struct_ready);
		pthread_mutex_unlock(&(struct_ready->mutex));

		pthread_mutex_lock(&(struct_ready_plus->mutex));
		imprimir_lista(struct_ready_plus);
		pthread_mutex_unlock(&(struct_ready_plus->mutex));

		pthread_mutex_lock(&(struct_exec->mutex));
		imprimir_lista(struct_exec);
		pthread_mutex_unlock(&(struct_exec->mutex));

		imprimir_lista_blocked_recursos();

		imprimir_lista_blocked_interfaz();

		pthread_mutex_lock(&(struct_exit->mutex));
		imprimir_lista(struct_exit);
		pthread_mutex_unlock(&(struct_exit->mutex));



		break;
	default:
		break;
	
	}
	string_array_destroy(array_leido);
}

bool estaa_o_no(t_instruccion* instruccion, char* nombre_instruccion){
	return (strcmp(instruccion->nombre,nombre_instruccion)==0);
}

// /// EJECUTAR_SCRIPT [path]
t_list* leer_archivo(char* archivo){
	char* PATH = string_new();
	string_append(&PATH,"/home/utnso/c-comenta-pruebas");
	string_append(&PATH, archivo);
    FILE * archivo_comandos = fopen(PATH, "r");
	if (archivo_comandos == NULL){
       perror("Error al intentar cargar el archivo de comandos \n");
       exit(EXIT_FAILURE);
    }
	t_list* lista_comandos = list_create();
	int longitud= 0;
	char* comando_leido = malloc(100 *sizeof(char));

	while (fgets(comando_leido,100,archivo_comandos)){
		longitud = strlen(comando_leido);
		char* nuevo_comando = string_new();
		if(comando_leido[longitud -1] == '\n'){
			string_n_append(&nuevo_comando,comando_leido,longitud-1);
			list_add(lista_comandos, nuevo_comando);
		}else{
			string_n_append(&nuevo_comando,comando_leido,longitud);
			list_add(lista_comandos, nuevo_comando);
		}
		free(comando_leido);
		free(nuevo_comando);
		comando_leido = malloc(100 *sizeof(char));
	}
	free(comando_leido);
	free(PATH);
	fclose(archivo_comandos);
	return lista_comandos;
	
} 

// ///FINALIZAR_PROCESO



// INICIAR_PROCESO
void enviar_path_a_memoria(char* path,int pid,int socket){
	t_paquete* un_paquete = crear_paquete(CREAR_PROCESO);
	agregar_int_a_paquete(un_paquete,pid);
	agregar_string_a_paquete(un_paquete,path);
	enviar_paquete(un_paquete, socket);
	eliminar_paquete(un_paquete);
}
///////////PROCESO_ESTADO////////////
void imprimir_lista(t_listas_estados* lista_a_mostrar){
	printf("************LISTA <%s>************\n", enum_a_string(lista_a_mostrar->estado));
	t_list_iterator* lista = list_iterator_create(lista_a_mostrar->lista);
	t_pcb* un_pcb;
	while(list_iterator_has_next(lista)){
		un_pcb = list_iterator_next(lista);
		printf("%18s PID: %d \n"," ", un_pcb->pid);
	}
	list_iterator_destroy(lista);
}

void imprimir_lista_blocked_recursos(){
	printf("************LISTA <BLOCKED> RECURSOS************\n");
	t_list_iterator* lista_general = list_iterator_create(lista_recursos);
	t_recursos* recurso;
	t_pcb* un_pcb;
	while(list_iterator_has_next(lista_general)){
		recurso = list_iterator_next(lista_general);
		t_list_iterator* lista_blocked_recurso = list_iterator_create(recurso->lista_procesos_bloqueados);
		while(list_iterator_has_next(lista_blocked_recurso)){
			un_pcb = list_iterator_next(lista_blocked_recurso);
			printf("%18s PID: %d \n"," ", un_pcb->pid);
		}
		list_iterator_destroy(lista_blocked_recurso);			
	}
	list_iterator_destroy(lista_general);
}
void imprimir_lista_blocked_interfaz(){
	printf("************LISTA <BLOCKED> IO************\n");
	pthread_mutex_lock(&mutex_io);
	t_list_iterator* lista_general = list_iterator_create(IOS_CONECTADOS);
	t_interfaz* interfaz;
	t_proceso_blocked* proceso_blocked;
	t_queue* auxiliar = queue_create();
	while(list_iterator_has_next(lista_general)){
		interfaz = list_iterator_next(lista_general);
		pthread_mutex_lock(&(interfaz->mutex_cola_blocked));
		while(!queue_is_empty(interfaz->cola_procesos_blocked)){
			proceso_blocked = queue_pop(interfaz->cola_procesos_blocked);
			printf("%18s PID: %d \n"," ", proceso_blocked->un_pcb->pid);
			queue_push(auxiliar,proceso_blocked);
		}
		int size = queue_size(auxiliar);
		for (int i = 0; i < size; i++){
			proceso_blocked = queue_pop(auxiliar);
			queue_push(interfaz->cola_procesos_blocked,proceso_blocked);
		}
		pthread_mutex_unlock(&(interfaz->mutex_cola_blocked));
	}
	pthread_mutex_unlock(&mutex_io);
	list_iterator_destroy(lista_general);
	queue_destroy(auxiliar);
}		

void finalizar_consola(){
	list_destroy_and_destroy_elements(lista_instrucciones,(void*)eliminar_intrucciones);
	
}

void eliminar_intrucciones(t_instruccion* instruccion){
	free(instruccion->nombre);
	free(instruccion);
}