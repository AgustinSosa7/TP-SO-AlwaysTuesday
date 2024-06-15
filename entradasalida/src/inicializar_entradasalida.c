#include "../includes/inicializar_entradasalida.h"


void inicializar_entradasalida(char** argv){
    inicializar_logs();
    inicializar_configs(argv);
    imprimir_configs();
	inicializar_listas();
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
	char* path = argv[1];
	char* PATH = string_new();
    NOMBRE_INTERFAZ = path;

    string_append(&PATH, "/home/utnso/Desktop/tp-2024-1c-AlwaysTuesday/entradasalida/configs/");
	string_append(&PATH, path);
	string_append(&PATH, ".config");

	entradasalida_config = config_create(PATH);

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

			iniciar_file_system();
	
		} else {
	        printf("Tipo de interfaz desconocida.");
			EXIT_FAILURE;    
			}
}


void imprimir_configs(){
    log_info(entradasalida_logger, "NOMBRE_INTERFAZ: %s",NOMBRE_INTERFAZ);
	log_info(entradasalida_logger, "TIPO_INTERFAZ: %s",TIPO_INTERFAZ);

}

void inicializar_listas(){
	lista_registros_propisito_general = list_create();
	lista_registros_extendidos = list_create();
	list_add(lista_registros_propisito_general, "AX");
	list_add(lista_registros_propisito_general, "BX");
	list_add(lista_registros_propisito_general, "CX");
	list_add(lista_registros_propisito_general, "DX");
	list_add(lista_registros_extendidos, "EAX");
	list_add(lista_registros_extendidos, "EBX");
	list_add(lista_registros_extendidos, "ECX");
	list_add(lista_registros_extendidos, "EDX");
}

void iniciar_file_system(){
	inicializar_archivo_de_bloques();
}

void inicializar_archivo_de_bloques(){
	fd_archivoBloques = open(PATH_BASE_DIALFS, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	int tamanio_archivo_bloques = BLOCK_COUNT * BLOCK_SIZE;
	ftruncate(fd_archivoBloques, tamanio_archivo_bloques);

	//bitmap_swap = calloc(CANT_BLOQUES_SWAP, sizeof(char));
	//bitmapSWAP = bitarray_create_with_mode(bitmap_swap, CANT_BLOQUES_SWAP/8, LSB_FIRST);
//
	//bloquesEnMemoria = mmap(NULL, CANT_BLOQUES_TOTAL*TAM_BLOQUE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_archivoBloques, 0);
	//if (bloquesEnMemoria == MAP_FAILED) {
	//	log_error(filesystem_logger, "Error al mapear los bloques SWAP");
	//	exit(1);
	//}
}
