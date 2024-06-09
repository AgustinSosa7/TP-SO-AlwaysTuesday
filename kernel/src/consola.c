#include "../includes/consola.h"

void iniciar_consola(){
	lista_instrucciones = list_create();
	agregar_instruccion(lista_instrucciones,EJECUTAR_SCRIPT,1,"EJECUTAR_SCRIPT");
	agregar_instruccion(lista_instrucciones,INICIAR_PROCESO,0,"INICIAR_PROCESO");
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
		printf("instrucciones posibles:%s \n",instruccion->nombre);
        if(strcmp(nombre_instruccion, instruccion->nombre) == 0){  
			printf("param posibles:%d \n",instruccion->cant_parametros);
			printf("param recibidos:%d \n",cant_parametros);
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
		printf("hola");
		break;
	case INICIAR_PROCESO:
		printf("entre a iniciar proceso");
		t_pcb* nuevo_pcb = crearPcb();
		pthread_mutex_lock(&mutex_new);
		list_add(lista_new, nuevo_pcb);
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

bool estaa_o_no(t_instruccion* instruccion, char* nombre_instruccion){
	return (strcmp(instruccion->nombre,nombre_instruccion)==0);
}



// void proceso_estado(){
// 	t_list* procesos_estado = list_create();
// }
