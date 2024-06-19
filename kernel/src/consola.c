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
	return (validar_nombre_y_parametros(array_leido[0],size_parametros));


}

bool validar_nombre_y_parametros(char* nombre_instruccion,int cant_parametros) {

    bool esta_la_instruccion(void* instruccion) {
      return esta_o_noo(nombre_instruccion, cant_parametros, instruccion);
    }

    return (list_any_satisfy(lista_instrucciones, esta_la_instruccion)); 
}


bool esta_o_noo(char* nombre_instruccion, int cant_parametros, t_instruccion* instruccion){ 
		
        if(strcmp(nombre_instruccion, instruccion->nombre) == 0){  
			printf("parametros posibles:%d \n",instruccion->cant_parametros);
			printf("parametros recibidos:%d \n",cant_parametros);
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
		break;
	case INICIAR_PROCESO:
		printf("Entre a iniciar proceso. \n");
		t_pcb* nuevo_pcb = crearPcb();
		pthread_mutex_lock(&mutex_new);
		list_add(lista_new, nuevo_pcb);
		pthread_mutex_unlock(&mutex_new);	
		log_warning(kernel_logger,"Se crea el proceso < %d > en NEW. \n",nuevo_pcb->pid);
		sem_post(&sem_new_a_ready);
		enviar_path_a_memoria(array_leido[1],nuevo_pcb->pid,fd_memoria);
    
		break;
	case FINALIZAR_PROCESO: 
		int pid = atoi(array_leido[1]);
	 	t_pcb* pcb = buscar_pcb(pid);
		if(pcb != NULL){
			estado_pcb estado_anterior = pcb->estado_pcb;
			t_pcb* pcb_ejecutando = list_get(lista_exec,0);
			if(pcb_ejecutando->pid == pid){
				enviar_interrupción_a_cpu(SOLICITUD_INTERRUMPIR_PROCESO, INTERRUPCION_POR_KILL);		
			} else if(pcb->estado_pcb == EXIT){
				eliminar_proceso(pcb,INTERRUPTED_BY_USER);
				} else if(pcb->estado_pcb != BLOCKED){
					list_remove_element(buscar_lista(estado_anterior),pcb);
					pcb->estado_pcb = EXIT;
					eliminar_proceso(pcb,INTERRUPTED_BY_USER);
					log_warning(kernel_logger,"Cambio de Estado: PID: <%d> - Estado Anterior: <%s> - Estado Actual: <%s> \n",pcb->pid, enum_a_string(estado_anterior),enum_a_string(pcb->estado_pcb));
					
				
			} else{
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
		if(flag_detener_planificacion){
			pthread_mutex_lock(&mutex_detener_planificacion);
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
		imprimir_lista(lista_new, NEW);
		imprimir_lista(lista_ready, READY);
		imprimir_lista(lista_ready_plus, READYPLUS);
		//imprimir_lista(lista_, BLOCKED); ver blocked 
		imprimir_lista(lista_exec, EXEC);
		imprimir_lista(lista_exit, EXIT);
		break;
	default:
		break;
	
	}
}

bool estaa_o_no(t_instruccion* instruccion, char* nombre_instruccion){
	return (strcmp(instruccion->nombre,nombre_instruccion)==0);
}

// /// EJECUTAR_SCRIPT [path]
t_list* leer_archivo(char* path){
   FILE * archivo_comandos = fopen(path, "r");
	if (archivo_comandos == NULL){
       perror("Error al intentar cargar el archivo de comandos \n");
       exit(EXIT_FAILURE);
   }
	t_list* lista_comandos = list_create();
	int longitud= 0;
	char* comando_leido = malloc(100 *sizeof(char));

	while (fgets(comando_leido,100,archivo_comandos)){
		longitud = strlen(comando_leido);
		if(comando_leido[longitud] == '\n'){
			char* nuevo_comando = string_new();
			string_n_append(&nuevo_comando,comando_leido,longitud-1);
			free(comando_leido);
			list_add(lista_comandos, nuevo_comando);
		}
	}
	return lista_comandos;
	
} 

// ///FINALIZAR_PROCESO

///////////PROCESO_ESTADO////////////


void enviar_path_a_memoria(char* path,int pid,int socket){
	t_paquete* un_paquete = crear_paquete(CREAR_PROCESO);
	agregar_int_a_paquete(un_paquete,pid);
	agregar_string_a_paquete(un_paquete,path);
	enviar_paquete(un_paquete, socket);
	eliminar_paquete(un_paquete);
}

void imprimir_lista(t_list* lista_a_mostrar,estado_pcb estado){
	printf("************LISTA <%s>************\n", enum_a_string(estado));
	t_list_iterator* lista = list_iterator_create(lista_a_mostrar);
	t_pcb* un_pcb;
	while(list_iterator_has_next(lista)){
		un_pcb = list_iterator_next(lista);
		printf("PID: %d \n", un_pcb->pid);
	}
}
