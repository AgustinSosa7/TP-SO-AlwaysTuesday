#include "../includes/inicializar_entradasalida.h"


void inicializar_entradasalida(char** argv){
    inicializar_logs();
    inicializar_configs(argv);
    imprimir_configs();
}

void inicializar_logs (){
   entradasalida_logger = log_create("entradasalida.log","ENTRADASALIDA_LOG",1,LOG_LEVEL_INFO);
if (entradasalida_logger == NULL) {
	perror("Algo raro pasa con el log. No se pudo crear o encontrar el archivo.");
	exit(EXIT_FAILURE);
	}


entradasalida_log_debug = log_create("entradasalida_log_debug.log","ENTRADASALIDA_LOG_DEBUG",1,LOG_LEVEL_TRACE);
if (entradasalida_log_debug == NULL) {
	perror("Algo raro pasa con el log. No se pudo crear o encontrar el archivo.");
	exit(EXIT_FAILURE);
	}
}

void inicializar_configs(char** argv){
	NOMBRE_INTERFAZ = argv[1];
	char* path = argv[2];
		
	entradasalida_config = config_create(path);

if (entradasalida_config == NULL) {
	perror("Error al intentar cargar el config.");
	exit(EXIT_FAILURE);
	}

	TIPO_INTERFAZ = config_get_string_value(entradasalida_config,"TIPO_INTERFAZ");
	recibir_configs(TIPO_INTERFAZ);
}
void recibir_configs(char* TIPO_INTERFAZ){

	 if (strcmp(TIPO_INTERFAZ, "GEN") == 0) {
            TIEMPO_UNIDAD_TRABAJO = config_get_int_value(entradasalida_config,"TIEMPO_UNIDAD_TRABAJO");
			IP_KERNEL = config_get_string_value(entradasalida_config,"IP_KERNEL");
			PUERTO_KERNEL = config_get_string_value(entradasalida_config,"PUERTO_KERNEL");

    } else if (strcmp(TIPO_INTERFAZ, "STDIN") == 0) {
			IP_KERNEL = config_get_string_value(entradasalida_config,"IP_KERNEL");
			PUERTO_KERNEL= config_get_string_value(entradasalida_config,"PUERTO_KERNEL");
			IP_MEMORIA = config_get_string_value(entradasalida_config,"IP_MEMORIA");
			PUERTO_MEMORIA = config_get_string_value(entradasalida_config,"PUERTO_MEMORIA");   

    } else if (strcmp(TIPO_INTERFAZ, "STDOUT") == 0) {
			TIEMPO_UNIDAD_TRABAJO = config_get_int_value(entradasalida_config,"TIEMPO_UNIDAD_TRABAJO");
			IP_KERNEL = config_get_string_value(entradasalida_config,"IP_KERNEL");
			PUERTO_KERNEL= config_get_string_value(entradasalida_config,"PUERTO_KERNEL");
			IP_MEMORIA = config_get_string_value(entradasalida_config,"IP_MEMORIA");
			PUERTO_MEMORIA = config_get_string_value(entradasalida_config,"PUERTO_MEMORIA");

	} else if (strcmp(TIPO_INTERFAZ, "FS") == 0) {
			TIEMPO_UNIDAD_TRABAJO = config_get_int_value(entradasalida_config,"TIEMPO_UNIDAD_TRABAJO");
			IP_KERNEL = config_get_string_value(entradasalida_config,"IP_KERNEL");
			PUERTO_KERNEL= config_get_string_value(entradasalida_config,"PUERTO_KERNEL");
			IP_MEMORIA = config_get_string_value(entradasalida_config,"IP_MEMORIA");
			PUERTO_MEMORIA = config_get_string_value(entradasalida_config,"PUERTO_MEMORIA");
			PATH_BASE_DIALFS = config_get_string_value(entradasalida_config,"PATH_BASE_DIALFS");
			BLOCK_SIZE = config_get_int_value(entradasalida_config,"BLOCK_SIZE");
			BLOCK_COUNT = config_get_int_value(entradasalida_config,"BLOCK_COUNT");
			RETRASO_COMPACTACION = config_get_int_value(entradasalida_config,"RETRASO_COMPACTACION");

		} else {
	        printf("Tipo de interfaz desconocida.");
			EXIT_FAILURE;    
			}
}

void imprimir_configs(){
    log_info(entradasalida_logger, "NOMBRE_INTERFAZ: %s",NOMBRE_INTERFAZ);
	log_info(entradasalida_logger, "TIPO_INTERFAZ: %s",TIPO_INTERFAZ);

}


