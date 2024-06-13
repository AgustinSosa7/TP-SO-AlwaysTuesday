#include "../includes/memoria_procesos.h"




t_list* leer_archivo_pseudocodigo(char* nombre_Archivo){
    
    char* PATH = string_new();
    //Busco el archivo y lo abro
    string_append(&PATH, PATH_INSTRUCCIONES);
	string_append(&PATH, "/");
	string_append(&PATH, nombre_Archivo);
    printf("%s\n",PATH);
    FILE *Pseudocodigo = fopen(PATH, "r");

    //Una vez leido verifico que lo haya leido bien.
    if (Pseudocodigo == NULL){
        perror("Error al intentar cargar el archivo de Pseudocodigo.");
        exit(EXIT_FAILURE);
    }
    
    //Escribo en la lista y guardo las instrucciones.
    t_list* lista_de_instrucciones = list_create();
    int longitud = 0;
    char* INSTRUCCION = malloc(256 * sizeof(char));;//Averiguar por que 256
    
    while(fgets(INSTRUCCION, 256, Pseudocodigo)){  
        longitud = strlen(INSTRUCCION);
    		if(INSTRUCCION[longitud - 1] == '\n'){
				char* nueva_instruccion = string_new();
				string_n_append(&nueva_instruccion, INSTRUCCION, longitud - 1);
				free(INSTRUCCION);
				INSTRUCCION = malloc(256 * sizeof(int));
				strcpy(INSTRUCCION,nueva_instruccion);
                list_add(lista_de_instrucciones, nueva_instruccion);
    		}
    }
    fgets(INSTRUCCION, 256, Pseudocodigo);
    list_add(lista_de_instrucciones, INSTRUCCION);
    return lista_de_instrucciones;
};


t_proceso* crear_proceso_nuevo(){
    op_code code_op = recibir_operacion(fd_kernel);
    t_paquete* paquete = recibir_paquete(fd_kernel);
    t_buffer* buffer = paquete->buffer;
    t_proceso* procesoNuevo = malloc(sizeof(t_proceso));
    if(code_op == CREAR_PROCESO)
    {
        int pid = leer_int_del_buffer(buffer); //cambiar a leer algo del buffer
        char* direccion_pseudocodigo = leer_string_del_stream(buffer);
        procesoNuevo->pid = pid;
        procesoNuevo->direccion_pseudocodigo = direccion_pseudocodigo;
        procesoNuevo->instrucciones = list_create();
        procesoNuevo->instrucciones = leer_archivo_pseudocodigo(direccion_pseudocodigo);
        free(paquete);
        //list_add(procesos_memoria, procesoNuevo);
        //log_info(memoria_logger, "Se agrego un proceso a memoria %d",procesoNuevo->pid);
        return procesoNuevo;
    }
    else
    {   
        log_error(memoria_logger, "No se recibio un proceso a crear correctamente.");
        free(paquete);
        exit(EXIT_FAILURE);
    }
};

void enviar_instruccion_pesudocodigo(t_list* lista_proceso,int pc){
    t_paquete* paquete = crear_paquete(PSEUDOCODIGO);
    agregar_string_a_paquete(paquete, list_get(lista_proceso, pc));
    enviar_paquete(paquete, fd_cpu);
    eliminar_paquete(paquete);
}

t_pedido* recibir_instruccion_a_enviar(){
    op_code code_op = recibir_operacion(fd_cpu);
    t_paquete* paquete = recibir_paquete(fd_cpu);
    t_buffer* buffer = paquete->buffer;
    t_pedido* pedido = malloc(sizeof(t_pedido*));
    pedido->pid = leer_int_del_buffer(buffer);
    pedido->pc = leer_int_del_buffer(buffer);
    printf("PID: %d\n",pedido->pid);
    printf("PROGRAM_COUNTER: %d\n",pedido->pc);

    //enviar_instruccion_pesudocodigo((buscar_proceso_en_memoria(PROCESSID))->instrucciones,PROGRAM_COUNTER);
    free(buffer);
    free(paquete);
    return pedido;
};

t_proceso* buscar_proceso_en_memoria(int process_id){
    bool buscar_proceso(t_proceso* proceso){
	    return (proceso->pid == process_id);
    }
    t_proceso *proceso_encontrado = list_find(procesos_memoria, (void*)buscar_proceso);
    if(proceso_encontrado == NULL){
		log_error(memoria_log_debug, "El proceso %d no se encontro en memoria.", process_id);
		exit(EXIT_FAILURE);
	}
    return proceso_encontrado;
}
