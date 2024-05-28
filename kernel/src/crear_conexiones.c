#include "../includes/crear_conexiones.h"
        // Conectarse con Memoria
    void conexion_kernel_memoria(){
    log_info(kernel_logger, "Creando conexion con MEMORIA...");
    fd_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA, kernel_logger);
    gestionar_handshake_como_cliente(fd_memoria, "MEMORIA", kernel_logger);

  }
    // Conectarse con CPU
    void conexion_kernel_cpu_dispatch(){
    log_info(kernel_logger, "Creando conexion con CPU DISPATCH...");
    //  DISPATCH
    fd_cpu_dispatch = crear_conexion(IP_CPU, PUERTO_CPU_DISPATCH, kernel_logger);
    gestionar_handshake_como_cliente(fd_cpu_dispatch, "CPU", kernel_logger);
 }
    // INTERRUPT
    void conexion_kernel_cpu_interrupt(){
    log_info(kernel_logger, "Creando conexion con CPU INTERRUPT...");
    fd_cpu_interrupt = crear_conexion(IP_CPU, PUERTO_CPU_INTERRUPT, kernel_logger);
    gestionar_handshake_como_cliente(fd_cpu_interrupt, "CPU", kernel_logger);
}
    // INICIAR EL SERVIDOR PARA ENTRADA Y SALIDA.
    void conexion_kernel_entradaSalida(){
    fd_kernel = iniciar_servidor(PUERTO_ESCUCHA, kernel_logger, IP_KERNEL);

    IOS_CONECTADOS = list_create();

    log_info(kernel_logger, "Esperando a Entradasalida...");
    pthread_t hilo_generador_de_io;
    pthread_create(&hilo_generador_de_io, NULL, (void*)gestionar_entrada_salida, NULL);
    pthread_detach(hilo_generador_de_io);
    }

  

void gestionar_entrada_salida(){
  while(1){
    fd_entradasalida = esperar_cliente(fd_kernel, kernel_logger, "ENTRADASALIDA");
    gestionar_handshake_como_server(fd_entradasalida, kernel_logger, "ENTRADA SALIDA");
    t_interfaz* interfaz = identificar_io(fd_entradasalida);
    agregar_io(interfaz);
  }
}


t_interfaz* identificar_io(int socket){
  t_interfaz* interfaz = malloc(sizeof(t_interfaz));
  char* nombre_io = recibir_mensaje_string(socket);
  interfaz->nombre = malloc(strlen(nombre_io));
  interfaz->fd_interfaz = socket;
  interfaz->cola_procesos_blocked = queue_create();
  sem_init(interfaz->semaforo_cola_procesos_blocked, 0);
  return interfaz;
}

void agregar_io(t_interfaz* interfaz){
  pthread_mutex_lock(mutex_io);
  list_add(IOS_CONECTADOS, interfaz);
  pthread_mutex_unlock(mutex_io);

}

