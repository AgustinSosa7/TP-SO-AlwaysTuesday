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

    string_append(&PATH, "/home/utnso/tp-2024-1c-AlwaysTuesday/entradasalida/configs/");
	string_append(&PATH, path);
	string_append(&PATH, ".config");

	entradasalida_config = config_create(PATH);

if (entradasalida_config == NULL) {
	perror("Error al intentar cargar el config.");
	exit(EXIT_FAILURE);
	}

	TIPO_INTERFAZ = config_get_string_value(entradasalida_config,"TIPO_INTERFAZ");
	recibir_configs(TIPO_INTERFAZ);

	free(PATH);
}


void recibir_configs(char* TIPO_INTERFAZ){

	 if (strcmp(TIPO_INTERFAZ, "GENERICA") == 0) {
            TIEMPO_UNIDAD_TRABAJO = config_get_int_value(entradasalida_config,"TIEMPO_UNIDAD_TRABAJO");
			IP_KERNEL = config_get_string_value(entradasalida_config,"IP_KERNEL");
			PUERTO_KERNEL = config_get_string_value(entradasalida_config,"PUERTO_KERNEL");

    } else if (strcmp(TIPO_INTERFAZ, "STDIN") == 0) {
			IP_KERNEL = config_get_string_value(entradasalida_config,"IP_KERNEL");
			PUERTO_KERNEL= config_get_string_value(entradasalida_config,"PUERTO_KERNEL");
			IP_MEMORIA = config_get_string_value(entradasalida_config,"IP_MEMORIA");
			PUERTO_MEMORIA = config_get_string_value(entradasalida_config,"PUERTO_MEMORIA");   

    } else if (strcmp(TIPO_INTERFAZ, "STDOUT") == 0) {
			IP_KERNEL = config_get_string_value(entradasalida_config,"IP_KERNEL");
			PUERTO_KERNEL= config_get_string_value(entradasalida_config,"PUERTO_KERNEL");
			IP_MEMORIA = config_get_string_value(entradasalida_config,"IP_MEMORIA");
			PUERTO_MEMORIA = config_get_string_value(entradasalida_config,"PUERTO_MEMORIA");

	} else if (strcmp(TIPO_INTERFAZ, "FS") == 0) {      // VER PORQUE ENTRADA SALIDA_config tiene esos datos
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

			//finalizar_filesystem();
	
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
	//lista_struct_fcbs = list_create();
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
	log_info(entradasalida_log_debug,"Busacando FileSystem existente");
	crear_paths();
	inicializar_archivos(); // Inicia archivo Bloques y Bitmap
	log_info(entradasalida_log_debug, "FileSystem inicializado");
	pthread_mutex_init(&mutex_bitmap, NULL);
	pthread_mutex_init(&mutex_bloques, NULL);

}

void crear_paths(){
	log_info(entradasalida_log_debug, "Creando paths");
	PATH_BLOQUES=crear_path("/Bloques.dat");
	//string_append(&PATH_BLOQUES,PATH_BASE_DIALFS);
	//string_append(&PATH_BLOQUES,"/Bloques.dat"); 
	
	PATH_BITMAP = crear_path("/Bitmap.dat");
	//string_append(&PATH_BITMAP,PATH_BASE_DIALFS);
	//string_append(&PATH_BITMAP,"/Bitmap.dat"); 
	log_info(entradasalida_log_debug, "Paths creados correctamente");
}

char* crear_path(char* nombre_archivo){ 	// Nombra el directorio y lo guarda en el PATH
	char* path = string_new();
	string_append(&path,PATH_BASE_DIALFS);
	string_append(&path,nombre_archivo); 
	return path;
}

void inicializar_archivos(){ 

	fd_archivoBloques = open(PATH_BLOQUES, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);  // ???

	int tamanio_archivo_bloques = BLOCK_COUNT * BLOCK_SIZE;
	int tamanio_bitmap = ceil(BLOCK_COUNT/8);   
	
	ftruncate(fd_archivoBloques, tamanio_archivo_bloques); // hace que un archivo tenga ese tamanio
	
	bloquesEnMemoria = mmap(NULL, tamanio_archivo_bloques, PROT_READ | PROT_WRITE, MAP_SHARED, fd_archivoBloques, 0); // Verificar 2 parametro
	if (bloquesEnMemoria == MAP_FAILED) {
		log_error(entradasalida_logger, "Error al mapear los Bloques.dat");
		exit(1);
	}
	
	char* bitmap_inicializado = fopen (PATH_BITMAP,"r");  
	if (bitmap_inicializado!=NULL)
	{	
		log_info(entradasalida_log_debug, "El Archivo ya existía");
		fclose(bitmap_inicializado);					 
	}
	
	fd_archivoBitmap = open(PATH_BITMAP, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	ftruncate(fd_archivoBitmap, tamanio_bitmap);
	pre_bitmap = mmap(NULL, tamanio_bitmap, PROT_READ | PROT_WRITE, MAP_SHARED, fd_archivoBitmap, 0); 
	if (pre_bitmap == MAP_FAILED) {
		log_error(entradasalida_logger, "Error al mapear los Bitmap.dat");
		exit(1);
	}

	bitmap = malloc(tamanio_bitmap);

	// USO bitmapSWAP para el tema del bitmap
	bitmap = bitarray_create_with_mode(pre_bitmap, tamanio_bitmap, LSB_FIRST); 
	msync(bitmap->bitarray,tamanio_bitmap,MS_SYNC);

	if (bitmap_inicializado==NULL)
	{
		for (int i = 0 ; i < BLOCK_COUNT; i++)
	{	
		bitarray_clean_bit(bitmap,i);
	}
	}
	int rta;
	for (int i = 0 ; i < 32; i++)
	{	
		rta = bitarray_test_bit(bitmap,i) ? 1 : 0;
		//log_info(entradasalida_log_debug,"bitmap posicion %d : %d",i,rta);	
	}
	//string_array_destroy(PATH_BLOQUES);
	//string_array_destroy(PATH_BITMAP);
	//LSB_FIRST Completa los bits en un byte priorizando el bit menos significativo 00000001
}

