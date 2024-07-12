#include "../includes/memoria_procesos.h"

t_list* leer_archivo_pseudocodigo(char* nombre_Archivo){
    
    char* PATH = string_new();
    //Busco el archivo y lo abro
    string_append(&PATH, PATH_INSTRUCCIONES);
	string_append(&PATH, nombre_Archivo);
    printf("%s\n",PATH);
    FILE *Pseudocodigo = fopen(PATH, "r");

    //Una vez leido verifico que lo haya leido bien.
    if (Pseudocodigo == NULL){
        log_error(memoria_log_debug, "Error al intentar cargar el archivo de Pseudocodigo.");
        exit(EXIT_FAILURE);
    }
    
    //Escribo en la lista y guardo las instrucciones.
    t_list* lista_de_instrucciones = list_create();
    int longitud = 0;
    char* INSTRUCCION = malloc(256 * sizeof(char));//Averiguar por que 256
    while(fgets(INSTRUCCION, 256, Pseudocodigo)){  
        longitud = strlen(INSTRUCCION);
    		if(INSTRUCCION[longitud - 1] == '\n'){
				char* nueva_instruccion = string_new();
				string_n_append(&nueva_instruccion, INSTRUCCION, longitud - 1);
				free(INSTRUCCION);
				INSTRUCCION = malloc(256 * sizeof(char));
				strcpy(INSTRUCCION,nueva_instruccion);
                list_add(lista_de_instrucciones, nueva_instruccion);
    		}
    }
    fgets(INSTRUCCION, 256, Pseudocodigo);
    list_add(lista_de_instrucciones, INSTRUCCION);//ES EL ESPACIO DE MEMORIA DE INSTRUCCION
    free(PATH);
    fclose(Pseudocodigo);
    return lista_de_instrucciones;
};

void enviar_instruccion_pesudocodigo(t_list* lista_proceso,int pc){
    t_paquete* paquete = crear_paquete(PSEUDOCODIGO);
    agregar_string_a_paquete(paquete, list_get(lista_proceso, pc));
    enviar_paquete(paquete, fd_cpu);
    eliminar_paquete(paquete);
}

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

/*
t_proceso* buscar_proceso_en_memoria(int process_id){
     printf("Busco el proceso...1\n");
    bool buscar_proceso(void* proceso){
	    return (buscar_pid(proceso,process_id));
    }
     printf("Busco el proceso...2\n");
    t_proceso* proceso_encontrado = list_find(procesos_memoria, buscar_proceso);
     printf("Busco el proceso...3\n");
    if(proceso_encontrado == NULL){
		log_error(memoria_log_debug, "El proceso %d no se encontro en memoria.", process_id);
		exit(EXIT_FAILURE);
	}
    return proceso_encontrado;
}

bool buscar_pid(t_proceso* proceso,int process_id){
    int processid = proceso->pid;
    return (processid==process_id);
}*/