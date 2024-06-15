#include "../includes/inicializar_memoria.h"

void inicializar_memoria(char* path){
    _iniciar_log();  //memoria_logger = _iniciar_log();
    _iniciar_config(path);
    _imprimir_config();
    _inicializar_listas();
    _inicializar_estructuras();
}

void _iniciar_log(){  // t_log* _iniciar_log()
    memoria_logger = log_create("cliente.log", "CL_LOG", 1, LOG_LEVEL_INFO);
    if (memoria_logger == NULL){
        perror("Algo raro paso con el log. No se pudo crear o encontrar el archivo.");
        exit(EXIT_FAILURE);
    }
    
    memoria_log_debug = log_create("cliente.log", "CL_LOG", 1, LOG_LEVEL_TRACE);
    if (memoria_log_debug == NULL){
        perror("Algo raro paso con el log. No se pudo crear o encontrar el archivo.");
        exit(EXIT_FAILURE);
    }
}


void _iniciar_config(char* path){
    memoria_config = config_create(path);
    if (memoria_config == NULL){
        perror("Error al intentar cargar el config.");
        exit(EXIT_FAILURE);
    }
    
    PUERTO_ESCUCHA = config_get_string_value(memoria_config, "PUERTO_ESCUCHA");
    TAM_MEMORIA = config_get_int_value(memoria_config, "TAM_MEMORIA");
    TAM_PAGINA = config_get_int_value(memoria_config, "TAM_PAGINA");
    PATH_INSTRUCCIONES = config_get_string_value(memoria_config,"PATH_INSTRUCCIONES");
    RETARDO_RESPUESTA = config_get_int_value(memoria_config, "RETARDO_RESPUESTA");
}

void _imprimir_config(){
    log_info(memoria_log_debug, "MEMORIA PUERTO_ESCUCHA: %s", PUERTO_ESCUCHA);
    log_info(memoria_log_debug, "MEMORIA TAM_MEMORIA: %d", TAM_MEMORIA);
    log_info(memoria_log_debug, "MEMORIA TAM_PAGINA: %d", TAM_PAGINA);
    log_info(memoria_log_debug, "MEMORIA PATH_INSTRUCCIONES: %s", PATH_INSTRUCCIONES);
    log_info(memoria_log_debug, "MEMORIA RETARDO_RESPUESTA: %d", RETARDO_RESPUESTA);
}

void _inicializar_listas(){
    procesos_memoria = list_create();
}

void _inicializar_estructuras(){
    size_t tamanio_memoria = TAM_MEMORIA;
    espacio_memoria = malloc(tamanio_memoria);
    int cantidad_de_marcos = TAM_MEMORIA/TAM_PAGINA;

    bitmap_memoria_usuario = malloc (cantidad_de_marcos/8);

    tabla_de_marcos = bitarray_create_with_mode(bitmap_memoria_usuario, cantidad_de_marcos/8, LSB_FIRST); 
    //LSB_FIRST Completa los bits en un byte priorizando el bit menos significativo 00000001

    size_de_tabla_marcos = bitarray_get_max_bit(tabla_de_marcos);
    
    for(int i = 0; i<size_de_tabla_marcos;i++){ //SE INICIALIZAN ALGUNOS BITS EN 1 POR ESO LOS LIMPIO
            liberar_marco(i);
    }

    ocupar_marco(0);
    ocupar_marco(1);
    ocupar_marco(2);
    ocupar_marco(3);
    ocupar_marco(4);
    ocupar_marco(6);
    ocupar_marco(7);

    /*TEST!*/
    for(int ii = 0; ii<size_de_tabla_marcos;ii++){
            log_info(memoria_log_debug, "valor del %d bit: %d",ii, bitarray_test_bit(tabla_de_marcos, ii));
    }
    /**/
    log_info(memoria_log_debug, "Cantidad de marcos: %d", cantidad_de_marcos);
    log_info(memoria_log_debug, "Cantidad de marcos/8: %d", cantidad_de_marcos/8);
    log_info(memoria_log_debug, "long de bitarray: %ld", size_de_tabla_marcos);
}