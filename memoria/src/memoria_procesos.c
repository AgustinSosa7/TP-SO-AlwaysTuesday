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


char* recibir_direccion_pseudocodigo(){ //CODIGO REPETIDO EN CPU_MEMORIA
    op_code code_op = recibir_operacion(fd_kernel);
    t_paquete* paquete = recibir_paquete(fd_kernel);
    t_buffer* buffer = paquete->buffer;
    log_info(memoria_logger, "Recibi un paquete de size: %d",paquete->buffer->size);
    log_info(memoria_logger, "Recibi un paquete con el op_code: %d",code_op);
    if(code_op == PSEUDOCODIGO)
    {
        log_info(memoria_logger, "leer_string_del_stream");
        char* direccion_pseudocodigo = leer_string_del_stream(buffer);//REVISAR POR QUE NO FUNCIONA
        log_info(memoria_logger, "free");
        free(paquete);
        return direccion_pseudocodigo;
    }
    else
    {   
        log_error(memoria_logger, "No se recibio un pseudocodigo.");
        free(paquete);
        exit(EXIT_FAILURE);
    }
};

void enviar_instruccion_pesudocodigo(t_list* lista_proceso,int pc){
    t_paquete* paquete = crear_paquete(PSEUDOCODIGO);
    agregar_string_a_paquete(paquete, list_get(lista_proceso, pc));

    /* BORRAR ES PARA LAS PRUEBAS  */

        int tamanio_string = strlen(list_get(lista_proceso, pc));
        char *string = malloc(tamanio_string * sizeof(char)); // En caso de pisar algun valor, hacerle free antes
	    printf("offset:%d\n",paquete->buffer->offset);
	    printf("tam del string (malloc): %ld\n",strlen(string));
	    memcpy(string, paquete->buffer->stream + sizeof(int), tamanio_string); //cambiar por offset el size of int
	    paquete->buffer->offset += strlen(string);//tamanio_string;
        printf("Longitud de lo guardado en el string: %ld\n",strlen(string));
	    printf("offset: %d\n",paquete->buffer->offset);
	    printf("Se guardo en el string:%s\n",string);

    /* FIN DE BORRAR ES PARA LAS PRUEBAS  */

    enviar_paquete(paquete, fd_cpu);
    eliminar_paquete(paquete);
}