#include "../includes/memoria_procesos.h"




t_list* leer_archivo_pseudocodigo(char* nombre_Archivo){
    
    char* PATH = string_new();
    //Busco el archivo y lo abro
    string_append(&PATH, PATH_INSTRUCCIONES);
	string_append(&PATH, "/");
	string_append(&PATH, nombre_Archivo);
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

char* recibir_direccion_pseudocodigo(){
    op_code code_op = recibir_operacion(fd_kernel);
    t_paquete* paquete = recibir_paquete(fd_kernel);
    void* stream = paquete->buffer->stream;
    char* direccion_pseudocodigo;
    if(code_op == PSEUDOCODIGO)
    {
        leer_string_del_stream(stream, direccion_pseudocodigo);
        printf("Direc pseudocodigo: %s",direccion_pseudocodigo);
        free(stream);
    }
    else
    {   
        log_error(memoria_logger, "No se recibio un pseudocodigo.");
        exit(EXIT_FAILURE);
    }
    return direccion_pseudocodigo;
};
