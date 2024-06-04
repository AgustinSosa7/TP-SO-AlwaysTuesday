#include "../includes/consola.h"

void iniciar_consola(){
	t_list* lista_instrucciones= list_create();
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
	int size = string_array_size(array_leido);
	char* tamanio_en_string = string_itoa(size);
	t_list_iterator* lista = list_iterator_create(lista_instrucciones);
	while(list_iterator_has_next(lista))
	{
        t_instruccion* instruccion = malloc(sizeof(t_instruccion));
		instruccion = list_iterator_next(lista);
		if(strcmp(array_leido[0],instruccion->nombre)!=0){
			log_error(kernel_logger,"No se pudo reconocer el comando");
            exit(EXIT_FAILURE);
		} else if(strcmp(tamanio_en_string, array_leido[1])!=0){
			log_error(kernel_logger,"No coincide la cantidad de parametros");
            exit(EXIT_FAILURE);
		} 
	}
	return true;
}


void atender_instruccion_validada(char* leido){
	op_code_instruccion op_code_encontrado = encontrar_op_code(leido);

	switch (op_code_encontrado)
	{
	case EJECUTAR_SCRIPT:
		
		break;
	case INICIAR_PROCESO:
		t_pcb* nuevo_pcb = crearPcb();
		pthread_mutex_lock(&mutex_new);
		 queue_push(cola_new, nuevo_pcb);
		pthread_mutex_unlock(&mutex_new);	
		log_info(kernel_logger,"Se crea el proceso < %d > en NEW",nuevo_pcb->pid);
		sem_post(&sem_new_a_ready);
		//enviar algo a memoria

		break;
	case FINALIZAR_PROCESO:
		break;
	case DETENER_PLANIFICACION:
		break;
	case INICIAR_PLANIFICACION:
		break;
	case MULTIPROGRAMACION:
		break;
	case PROCESO_ESTADO:
		break;
	default:
		break;
	}

}

op_code_instruccion encontrar_op_code(char* leido){ 
	op_code_instruccion op_code_instr;
    char** array_leido = string_split(leido," ");
	t_list_iterator* lista = list_iterator_create(lista_instrucciones);
	while(list_iterator_has_next(lista)){
        t_instruccion* instruccion = malloc(sizeof(t_instruccion));
		instruccion = list_iterator_next(lista);
		if(strcmp(array_leido[0],instruccion->nombre)==0){
            op_code_instr = instruccion->op_code_instruccion;
        }
    }
	return op_code_instr;
}
