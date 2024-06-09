#include "../includes/crear_conexiones.h"

    // Conectarse con Memoria
    void conexion_kernel_memoria(){
    log_info(kernel_logger, "Creando conexion con MEMORIA...");
    fd_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA, kernel_logger);
    gestionar_handshake_como_cliente(fd_memoria, "MEMORIA", kernel_logger);

  }
    // Conectarse con CPU
void conexion_kernel_cpu_dispatch(){
    pthread_t hilo_kernel_cpu_dispatch;
    pthread_create(&hilo_kernel_cpu_dispatch, NULL, (void*) crear_conexion_kernel_cpu_dispatch, NULL);
    pthread_detach(hilo_kernel_cpu_dispatch);
 }

void crear_conexion_kernel_cpu_dispatch(){
    log_info(kernel_logger, "Creando conexion con CPU DISPATCH...");
    fd_cpu_dispatch = crear_conexion(IP_CPU, PUERTO_CPU_DISPATCH, kernel_logger);
    gestionar_handshake_como_cliente(fd_cpu_dispatch, "CPU-INTERRUPT", kernel_logger);
}

    // INTERRUPT
void conexion_kernel_cpu_interrupt(){
    pthread_t hilo_kernel_cpu_interrupt;
    pthread_create(&hilo_kernel_cpu_interrupt, NULL, (void*) crear_conexion_kernel_cpu_interrupt, NULL);
    pthread_join(hilo_kernel_cpu_interrupt, NULL); // Esta bie que sea join? no hay algun problema con nada? 
}

void crear_conexion_kernel_cpu_interrupt(){
    log_info(kernel_logger, "Creando conexion con CPU INTERRUPT...");
    fd_cpu_interrupt = crear_conexion(IP_CPU, PUERTO_CPU_INTERRUPT, kernel_logger);
    gestionar_handshake_como_cliente(fd_cpu_interrupt, "CPU-INTERRUPT", kernel_logger);
}


// INICIAR EL SERVIDOR PARA ENTRADA Y SALIDA. 
void conexion_kernel_entradaSalida(){
    fd_kernel = iniciar_servidor(PUERTO_ESCUCHA, kernel_logger, IP_KERNEL);

    IOS_CONECTADOS = list_create();

    pthread_t hilo_generador_de_io;
    pthread_create(&hilo_generador_de_io, NULL, (void*)gestionar_entrada_salida, NULL);
    pthread_detach(hilo_generador_de_io);
    }

void gestionar_entrada_salida(){
  while(1){
    log_info(kernel_logger, "Esperando a Entradasalida...");
    fd_entradasalida = esperar_cliente(fd_kernel, kernel_logger, "ENTRADASALIDA");
    gestionar_handshake_como_server(fd_entradasalida, kernel_logger, "ENTRADA SALIDA");

    t_interfaz* interfaz = identificar_io(fd_entradasalida);
    agregar_io(interfaz);
    log_info(kernel_logger, "Se conecto la interfaz: %s de tipo %s", interfaz->nombre, interfaz->tipo);

    pthread_t hilo_generador_procesos_io;
    pthread_create(&hilo_generador_procesos_io, NULL, (void*) gestionar_procesos_io, interfaz);
    pthread_detach(hilo_generador_procesos_io);
  }
}


t_interfaz* identificar_io(int socket){
  t_interfaz* interfaz = malloc(sizeof(t_interfaz));
  
  interfaz->nombre = recibir_mensaje_string(socket); 
  interfaz->tipo = recibir_mensaje_string(socket);

  interfaz->instrucciones_posibles = asignar_instrucciones_posibles(interfaz->tipo);
  interfaz->fd_interfaz = socket;
  interfaz->esta_conectada = true;
  interfaz->cola_procesos_blocked = queue_create();

  sem_init(&(interfaz->semaforo_cola_procesos_blocked), 0, 0); 
  pthread_mutex_t mutex_cola_blocked;
  pthread_mutex_init(&mutex_cola_blocked, NULL);
  interfaz->mutex_cola_blocked = mutex_cola_blocked;

  return interfaz;
}


t_list* asignar_instrucciones_posibles(char* tipo){
    t_list* instrucciones_posibles;
    instrucciones_posibles = list_create();
    if (strcmp(tipo, "GEN") == 0) {
      instrucciones_posibles = INSTRUCCIONES_GEN;      
    } else if (strcmp(tipo, "STDIN") == 0) {
	    instrucciones_posibles = INSTRUCCIONES_GEN;
    } else if (strcmp(tipo, "STDOUT") == 0) {
	    instrucciones_posibles = INSTRUCCIONES_GEN;
    } else if (strcmp(tipo, "FS") == 0) {
      instrucciones_posibles = INSTRUCCIONES_GEN;
		}
  return instrucciones_posibles;
}

void agregar_io(t_interfaz* interfaz){
  pthread_mutex_lock(&mutex_io);
  list_add(IOS_CONECTADOS, interfaz);
  pthread_mutex_unlock(&mutex_io);
}

void gestionar_procesos_io(t_interfaz* interfaz){
  while(1){
    sem_wait(&(interfaz->semaforo_cola_procesos_blocked));

    pthread_mutex_lock(&(interfaz->mutex_cola_blocked));
    t_proceso_blocked* proceso_a_ejecutar = queue_pop(interfaz->cola_procesos_blocked);
    pthread_mutex_unlock(&(interfaz->mutex_cola_blocked));

    enviar_peticion_a_interfaz(proceso_a_ejecutar, interfaz);
    recibir_fin_peticion(interfaz);

    desbloquear_proceso(interfaz);
  }
}
