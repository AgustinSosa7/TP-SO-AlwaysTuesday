#include <../includes/inicializar_kernel.h>

void inicializar_kernel(char* path){
    inicializar_logs();
    inicializar_configs(path);
	inicializar_listas();
	inicializar_semaforos();
	inicializar_pid();
	inicializar_recursos();
	iniciar_tiempo_VRR = false;
}

void inicializar_logs(void){

kernel_logger = log_create("kernel.log","KERNEL_LOG",1,LOG_LEVEL_INFO);

if (kernel_logger == NULL) {
	perror("Algo raro pasa con el log. No se pudo crear o encontrar el archivo.");
	exit(EXIT_FAILURE);
    }



kernel_log_debug = log_create("kernel_debug.log","KERNEL_DEBUG_LOG",1,LOG_LEVEL_INFO);
if (kernel_log_debug == NULL) {
	perror("Algo raro pasa con el log. No se pudo crear o encontrar el archivo.");
	exit(EXIT_FAILURE);
   }

}    
    
void inicializar_configs(char* path) {
    char* PATH = string_new();
    
    string_append(&PATH, "/home/utnso/tp-2024-1c-AlwaysTuesday/kernel/configs/");
	string_append(&PATH, path);
	string_append(&PATH, ".config");

	kernel_config = config_create(PATH);

	free(PATH);

	if (kernel_config == NULL) {
		perror("Error al intentar cargar el config.");
		exit(EXIT_FAILURE);
    } 

PUERTO_ESCUCHA = config_get_string_value(kernel_config,"PUERTO_ESCUCHA");
IP_MEMORIA = config_get_string_value(kernel_config,"IP_MEMORIA");
IP_KERNEL = config_get_string_value(kernel_config,"IP_KERNEL");
PUERTO_MEMORIA = config_get_string_value(kernel_config,"PUERTO_MEMORIA");
IP_CPU = config_get_string_value(kernel_config,"IP_CPU");
PUERTO_CPU_DISPATCH= config_get_string_value(kernel_config,"PUERTO_CPU_DISPATCH");
PUERTO_CPU_INTERRUPT = config_get_string_value(kernel_config,"PUERTO_CPU_INTERRUPT");
ALGORITMO_PLANIFICACION = config_get_string_value(kernel_config,"ALGORITMO_PLANIFICACION");
QUANTUM = config_get_int_value(kernel_config,"QUANTUM");
RECURSOS = config_get_array_value(kernel_config, "RECURSOS"); // REVISAR POR FAVOR EL TIPO DE DATO QUE SE MANEJA (kernel.h)
INSTANCIAS_RECURSOS = config_get_array_value(kernel_config, "INSTANCIAS_RECURSOS"); // REVISAR POR FAVOR EL TIPO DE DATO QUE SE MANEJA (kernel.h)
GRADO_MULTIPROGRAMACION = config_get_int_value(kernel_config,"GRADO_MULTIPROGRAMACION");

log_info(kernel_log_debug, "Se inicializan las configs..."); //Sacar eventualmente

log_info(kernel_log_debug, "PUERTO_ESCUCHA: %s", PUERTO_ESCUCHA); //Sacar eventualmente
log_info(kernel_log_debug, "IP_MEMORIA: %s", IP_MEMORIA); //Sacar eventualmente
log_info(kernel_log_debug, "PUERTO_MEMORIA: %s", PUERTO_MEMORIA); //Sacar eventualmente
log_info(kernel_log_debug, "IP_CPU: %s", IP_CPU); //Sacar eventualmente
log_info(kernel_log_debug, "PUERTO_CPU_DISPATCH: %s", PUERTO_CPU_DISPATCH); //Sacar eventualmente
log_info(kernel_log_debug, "PUERTO_CPU_INTERRUPT: %s", PUERTO_CPU_INTERRUPT); //Sacar eventualmente
//log_info(kernel_log_debug, "ALGORITMO_PLANIFICACION: %s", ALGORITMO_PLANIFICACION); //Sacar eventualmente
//log_info(kernel_log_debug, "QUANTUM: %d", QUANTUM); //Sacar eventualmente
//log_info(kernel_log_debug, "Falta RECURSOS:" %d); //Sacar eventualmente
//log_info(kernel_log_debug, "Falta INSTANCIAS_RECURSOS: %d", INSTANCIAS_RECURSOS); //Sacar eventualmente
//log_info(kernel_log_debug, "GRADO_MULTIPROGRAMACION: %d", GRADO_MULTIPROGRAMACION); //Sacar eventualmente

log_info(kernel_log_debug, "Se inicializaron las configs"); //Sacar eventualmente

}

void inicializar_listas(void){
	inicializar_structs_listas();
	inicializar_listas_instrucciones();
}

void inicializar_listas_instrucciones(){
	INSTRUCCIONES_GENERICA = list_create();
	INSTRUCCIONES_STDIN = list_create();
	INSTRUCCIONES_STDOUT = list_create();
	INSTRUCCIONES_FS = list_create();
	list_add(INSTRUCCIONES_GENERICA, "IO_GEN_SLEEP");
	list_add(INSTRUCCIONES_STDIN, "IO_STDIN_READ");
	list_add(INSTRUCCIONES_STDOUT, "IO_STDOUT_WRITE");
	list_add(INSTRUCCIONES_FS, "IO_FS_CREATE");
	list_add(INSTRUCCIONES_FS, "IO_FS_DELETE");
	list_add(INSTRUCCIONES_FS, "IO_FS_TRUNCATE");
	list_add(INSTRUCCIONES_FS, "IO_FS_WRITE");
	list_add(INSTRUCCIONES_FS, "IO_FS_READ");
}

void inicializar_semaforos(){
	printf("arrancando sems...\n");
	
	sem_init(&sem_grado_multiprogram,0,GRADO_MULTIPROGRAMACION);
	printf("listo las GRADO_MULTIPROGRAMACION\n");
	sem_init(&sem_new_a_ready,0,0);
	sem_init(&sem_planificador_corto_plazo,0,0);
	printf("listo las sem_planificador_corto_plazo\n");
	sem_init(&sem_detener_planificacion,0,0);
	
	pthread_mutex_init(&mutex_pid, NULL);
	printf("listo las mutex_pid\n");
	pthread_mutex_init(&mutex_io, NULL);
	pthread_mutex_init(&mutex_new, NULL);
	pthread_mutex_init(&mutex_ready, NULL);
	pthread_mutex_init(&mutex_exec, NULL);
	pthread_mutex_init(&mutex_ready_plus, NULL);
	pthread_mutex_init(&mutex_exit, NULL);
	pthread_mutex_init(&mutex_detener_planificacion,NULL);
	pthread_mutex_init(&mutex_VRR,NULL);
	printf("listo las mutex_exit\n");
}

void inicializar_pid(){
	pid_global = 0;
	} 

void inicializar_recursos(){
	printf("Inicializando la lista de recursos.\n");
	lista_recursos = list_create();
	int i = 0;

	while(RECURSOS[i]!= NULL){
		t_recursos* recurso = malloc(sizeof(t_recursos));
		pthread_mutex_t mutex;
		pthread_mutex_init(&mutex, NULL);
		recurso->mutex_recurso = mutex;
		recurso->nombre_recurso = RECURSOS[i];
		recurso->instancias = atoi(INSTANCIAS_RECURSOS[i]);
		recurso->lista_procesos_bloqueados = list_create();
		recurso->lista_procesos_asignados = list_create();
		list_add(lista_recursos,recurso);
		i++;
	}
	imprimir_lista_recursos(lista_recursos);
}

void imprimir_recursos(t_recursos* recurso){
	printf("nombre de recurso: %s\n",recurso->nombre_recurso);
	printf("instancias de recurso: %d\n",recurso->instancias );
}
void imprimir_lista_recursos(t_list* lista_a_mostrar){
	printf("************LISTA <RECURSOS>************\n");
	t_list_iterator* lista = list_iterator_create(lista_a_mostrar);
	t_recursos* recurso;
	while(list_iterator_has_next(lista)){
		recurso = list_iterator_next(lista);
		printf("recurso: %s \n", recurso->nombre_recurso);
	}
}

void inicializar_structs_listas(){
	struct_new = malloc(sizeof(t_listas_estados));
	struct_new->estado = NEW;
	struct_new->mutex = mutex_new;
	struct_new->lista = list_create();
	struct_ready = malloc(sizeof(t_listas_estados));
	struct_ready->estado = READY;
	struct_ready->mutex = mutex_ready;
	struct_ready->lista = list_create();
	struct_ready_plus = malloc(sizeof(t_listas_estados));
	struct_ready_plus->estado = READYPLUS;
	struct_ready_plus->mutex = mutex_ready_plus;
	struct_ready_plus->lista = list_create();
	struct_exec = malloc(sizeof(t_listas_estados));
	struct_exec->estado = EXEC;
	struct_exec->mutex = mutex_exec;
	struct_exec->lista= list_create();
	struct_exit = malloc(sizeof(t_listas_estados));
	struct_exit->estado = EXIT;
	struct_exit->mutex = mutex_exit;
	struct_exit->lista = list_create();
	printf("inicializadas las listas de estados de procesos:\n");
}