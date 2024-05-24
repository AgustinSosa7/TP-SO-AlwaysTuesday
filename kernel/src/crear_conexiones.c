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

    log_info(kernel_logger, "Esperando a Entradasalida...");
    pthread_t hilo_generador_de_io;
    pthread_create(&hilo_generador_de_io, NULL, (void*)gestionador_entrada_salida, NULL);
    pthread_detach(hilo_generador_de_io);



    }

  //crear hilo cada vez que te llegue una io

void gestionador_entrada_salida(){
    while(1){
      pthread_t hilo_io;
      pthread_create(&hilo_io,NULL, (void*)gestionar_entrada_salida, NULL);
      pthread_detach(hilo_io);
    }
}


void gestionar_entrada_salida(){
    fd_entradasalida = esperar_cliente(fd_kernel, kernel_logger, "ENTRADASALIDA");
    gestionar_handshake_como_server(fd_entradasalida, kernel_logger, "ENTRADA SALIDA");
    
   }

//lista_io = list_create(); ver donde crearla

void inicializar_lista_io(){
  list_add(lista_io,);

}