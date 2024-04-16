#include "../includes/inicializar_entradasalida.h"

void inicializar_entradasalida(){
    inicializar_logs();
    inicializar_configs();
    imprimir_configs();
}

void inicializar_logs (){
	//Creo que esto estaba desde antes, lo metiero los profes
   entradasalida_logger = log_create("entradasalida.log","ENTRADASALIDA_LOG",1,LOG_LEVEL_INFO);
if (entradasalida_logger == NULL) {
	perror("Algo raro pasa con el log. No se pudo crear o encontrar el archivo.");
	exit(EXIT_FAILURE);
	}

log_info(entradasalida_logger, "Se inicializo el kernel logger"); //Sacar eventualmente


entradasalida_log_debug = log_create("entradasalida_log_debug.log","ENTRADASALIDA_LOG_DEBUG",1,LOG_LEVEL_TRACE);
if (entradasalida_log_debug == NULL) {
	perror("Algo raro pasa con el log. No se pudo crear o encontrar el archivo.");
	exit(EXIT_FAILURE);
	}

log_info(entradasalida_log_debug, "Se inicializo el kernel debug logger"); //Sacar eventualmente
 }

void inicializar_configs(){
entradasalida_config = config_create("/home/utnso/tp-2024-1c-AlwaysTuesday/entradasalida/entradasalida.config");
if (entradasalida_config == NULL) {
	perror("Error al intentar cargar el config.");
	exit(EXIT_FAILURE);
	}

	TIPO_INTERFAZ = config_get_string_value(entradasalida_config,"TIPO_INTERFAZ");
	TIEMPO_UNIDAD_TRABAJO = config_get_int_value(entradasalida_config,"TIEMPO_UNIDAD_TRABAJO");
	IP_KERNEL = config_get_string_value(entradasalida_config,"IP_KERNEL");
	PUERTO_KERNEL= config_get_int_value(entradasalida_config,"PUERTO_KERNEL");
	IP_MEMORIA = config_get_string_value(entradasalida_config,"IP_MEMORIA");
	PUERTO_MEMORIA = config_get_int_value(entradasalida_config,"PUERTO_MEMORIA");
	PATH_BASE_DIALFS = config_get_string_value(entradasalida_config,"PATH_BASE_DIALFS");
	BLOCK_SIZE = config_get_int_value(entradasalida_config,"BLOCK_SIZE");
	BLOCK_COUNT = config_get_int_value(entradasalida_config,"BLOCK_COUNT");

}
void imprimir_configs(){
	log_info(entradasalida_log_debug, "Se inicializan las configs..."); //Sacar eventualmente

}
	//Inicializar Estructuras EntradaSalida. OJO PUERTO_ESCUCHA, 
	//fd_entradasalida = iniciar_servidor(PUERTO_ESCUCHA, entradasalida_logger,"EntradaSalida iniciado!");

	//Conectarme como cliente a Memoria
	//fd_memoria = crear_conexion(IP_MEMORIA);
//Conectarme como cliente a Kernel
	fd_kernel = crear_conexion(IP_KERNEL,PUERTO_KERNEL);