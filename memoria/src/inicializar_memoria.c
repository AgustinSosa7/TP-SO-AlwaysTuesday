#include "../includes/inicializar_memoria.h"

void inicializar_memoria(){
    _iniciar_log();  //memoria_logger = _iniciar_log();
    _iniciar_config();
    _imprimir_config();
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


void _iniciar_config(){
    memoria_config = config_create("/home/utnso/Desktop/tp-2024-1c-AlwaysTuesday/memoria/memoria.config");
    if (memoria_config == NULL){
        perror("Error al intentar cargar el config.");
        exit(EXIT_FAILURE);
    }
    
    //IP_MEMORIA = config_get_string_value(memoria_config,"IP_MEMORIA");
    PUERTO_ESCUCHA = config_get_string_value(memoria_config, "PUERTO_ESCUCHA");
    //IP_FILESYSTEM = config_get_string_value(memoria_config,"IP_FILESYSTEM");
    //PUERTO_FILESYSTEM = config_get_string_value(memoria_config,"PUERTO_FILESYSTEM");
    TAM_MEMORIA = config_get_int_value(memoria_config, "TAM_MEMORIA");
    TAM_PAGINA = config_get_int_value(memoria_config, "TAM_PAGINA");
    PATH_INSTRUCCIONES = config_get_string_value(memoria_config,"PATH_INSTRUCCIONES");
    RETARDO_RESPUESTA = config_get_int_value(memoria_config, "RETARDO_RESPUESTA");
    //ALGORITMO_REEMPLAZO = config_get_string_value(memoria_config,"ALGORITMO_REEMPLAZO");
}

void _imprimir_config(){
    log_info(memoria_log_debug, "MEMORIA PUERTO_ESCUCHA: %s", PUERTO_ESCUCHA);
    log_info(memoria_log_debug, "MEMORIA TAM_MEMORIA: %d", TAM_MEMORIA);
    log_info(memoria_log_debug, "MEMORIA TAM_PAGINA: %d", TAM_PAGINA);
    log_info(memoria_log_debug, "MEMORIA PATH_INSTRUCCIONES: %s", PATH_INSTRUCCIONES);
    log_info(memoria_log_debug, "MEMORIA RETARDO_RESPUESTA: %d", RETARDO_RESPUESTA);

}

