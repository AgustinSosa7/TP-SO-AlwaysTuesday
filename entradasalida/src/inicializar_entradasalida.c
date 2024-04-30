#include "../includes/inicializar_entradasalida.h"

t_list* INSTRUCCIONES_GEN;
t_list* INSTRUCCIONES_STDIN;
t_list* INSTRUCCIONES_STDOUT;
t_list* INSTRUCCIONES_FS;

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


entradasalida_log_debug = log_create("entradasalida_log_debug.log","ENTRADASALIDA_LOG_DEBUG",1,LOG_LEVEL_TRACE);
if (entradasalida_log_debug == NULL) {
	perror("Algo raro pasa con el log. No se pudo crear o encontrar el archivo.");
	exit(EXIT_FAILURE);
	}
}

void inicializar_configs(){
entradasalida_config = config_create("/home/utnso/Desktop/tp-2024-1c-AlwaysTuesday/entradasalida/entradasalida.config");

if (entradasalida_config == NULL) {
	perror("Error al intentar cargar el config.");
	exit(EXIT_FAILURE);
	}

	NOMBRE_INTERFAZ = config_get_string_value(entradasalida_config,"NOMBRE_INTERFAZ");
	TIPO_INTERFAZ = config_get_string_value(entradasalida_config,"TIPO_INTERFAZ");
	recibir_configs(TIPO_INTERFAZ);
}
void recibir_configs(char* TIPO_INTERFAZ){

	 if (strcmp(TIPO_INTERFAZ, "GEN") == 0) {
            TIEMPO_UNIDAD_TRABAJO = config_get_int_value(entradasalida_config,"TIEMPO_UNIDAD_TRABAJO");
			IP_KERNEL = config_get_string_value(entradasalida_config,"IP_KERNEL");
			PUERTO_KERNEL = config_get_string_value(entradasalida_config,"PUERTO_KERNEL");
			INSTRUCCIONES_POSIBLES = INSTRUCCIONES_GEN;

    } else if (strcmp(TIPO_INTERFAZ, "STDIN") == 0) {
			IP_KERNEL = config_get_string_value(entradasalida_config,"IP_KERNEL");
			PUERTO_KERNEL= config_get_string_value(entradasalida_config,"PUERTO_KERNEL");
			IP_MEMORIA = config_get_string_value(entradasalida_config,"IP_MEMORIA");
			PUERTO_MEMORIA = config_get_string_value(entradasalida_config,"PUERTO_MEMORIA");   
			INSTRUCCIONES_POSIBLES = INSTRUCCIONES_STDIN; 

    } else if (strcmp(TIPO_INTERFAZ, "STDOUT") == 0) {
			TIEMPO_UNIDAD_TRABAJO = config_get_int_value(entradasalida_config,"TIEMPO_UNIDAD_TRABAJO");
			IP_KERNEL = config_get_string_value(entradasalida_config,"IP_KERNEL");
			PUERTO_KERNEL= config_get_string_value(entradasalida_config,"PUERTO_KERNEL");
			IP_MEMORIA = config_get_string_value(entradasalida_config,"IP_MEMORIA");
			PUERTO_MEMORIA = config_get_string_value(entradasalida_config,"PUERTO_MEMORIA");
			INSTRUCCIONES_POSIBLES = INSTRUCCIONES_STDOUT;   

	} else if (strcmp(TIPO_INTERFAZ, "FS") == 0) {
			TIEMPO_UNIDAD_TRABAJO = config_get_int_value(entradasalida_config,"TIEMPO_UNIDAD_TRABAJO");
			IP_KERNEL = config_get_string_value(entradasalida_config,"IP_KERNEL");
			PUERTO_KERNEL= config_get_string_value(entradasalida_config,"PUERTO_KERNEL");
			IP_MEMORIA = config_get_string_value(entradasalida_config,"IP_MEMORIA");
			PUERTO_MEMORIA = config_get_string_value(entradasalida_config,"PUERTO_MEMORIA");
			PATH_BASE_DIALFS = config_get_string_value(entradasalida_config,"PATH_BASE_DIALFS");
			BLOCK_SIZE = config_get_int_value(entradasalida_config,"BLOCK_SIZE");
			BLOCK_COUNT = config_get_int_value(entradasalida_config,"BLOCK_COUNT");
		//	RETRASO_COMPACTACION = config_get_int_value(entradasalida_config,"RETRASO_COMPACTACION");
			INSTRUCCIONES_POSIBLES = INSTRUCCIONES_FS;
	
		} else {
	        printf("Tipo de interfaz desconocida.");
			EXIT_FAILURE;    
			}
}

void imprimir_configs(){
    log_info(entradasalida_logger, "NOMBRE_INTERFAZ: %s",NOMBRE_INTERFAZ);
	log_info(entradasalida_logger, "TIPO_INTERFAZ: %s",TIPO_INTERFAZ);

}

void inicializar_listas_instrucciones(){
	list_add(INSTRUCCIONES_GEN, "IO_GEN_SLEEP");
	list_add(INSTRUCCIONES_STDIN, "IO_STDIN_READ");
	list_add(INSTRUCCIONES_STDOUT, "IO_STDOUT_WRITE");
	list_add(INSTRUCCIONES_FS, "IO_FS_CREATE");
	list_add(INSTRUCCIONES_FS, "IO_FS_DELETE");
	list_add(INSTRUCCIONES_FS, "IO_FS_TRUNCATE");
	list_add(INSTRUCCIONES_FS, "IO_FS_WRITE");
	list_add(INSTRUCCIONES_FS, "IO_FS_READ");
}
