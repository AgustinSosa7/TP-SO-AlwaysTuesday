
#include "../includes/memoria.h"

int main(int argc, char** argv) {

if(validar_parametros_incorrectos_main(argc, 2, argv)){
        return EXIT_FAILURE;
    }
//Inicializar MEMORIA
inicializar_memoria(argv[1]);

//Comentado para que corra el programa :D


// Iniciar servidor de memoria
fd_memoria = iniciar_servidor(PUERTO_ESCUCHA, memoria_logger, IP_MEMORIA);  //Tercer argumento, ip del server


// Esperar conexion de CPU
log_info(memoria_logger, "Esperando a CPU...");
fd_cpu = esperar_cliente(fd_memoria, memoria_logger,"CPU");
gestionar_handshake_como_server(fd_cpu, memoria_logger, "CPU");


  // Atender los mensajes de Kernel
  pthread_t hilo_atender_cpu;
  pthread_create(&hilo_atender_cpu, NULL, (void*)atender_cpu, NULL);
  pthread_detach(hilo_atender_cpu); 


// Esperar conexion de KERNEL
log_info(memoria_logger, "Esperando a KERNEL...");
fd_kernel = esperar_cliente(fd_memoria, memoria_logger,"KERNEL"); 
gestionar_handshake_como_server(fd_kernel, memoria_logger, "KERNEL");

  // Atender los mensajes de Kernel
  pthread_t hilo_atender_kernel;
  pthread_create(&hilo_atender_kernel, NULL, (void*)atender_kernel, NULL);
  pthread_detach(hilo_atender_kernel); 



// Esperar conexion de ENTRADASALIDA
   
pthread_t hilo_generador_de_io;
pthread_create(&hilo_generador_de_io, NULL, (void*)gestionar_entrada_salida, NULL);
pthread_join(hilo_generador_de_io, NULL);


return EXIT_SUCCESS;
}



void gestionar_entrada_salida(){
  while(1){
    log_info(memoria_logger, "Esperando a EntradaSalida...");
    int fd_entradasalida = esperar_cliente(fd_memoria, memoria_logger, "ENTRADA SALIDA");
    gestionar_handshake_como_server(fd_entradasalida, memoria_logger, "ENTRADA SALIDA");
    pthread_t hilo_atender_io;
    pthread_create(&hilo_atender_io, NULL, (void*)atender_entradasalida, fd_entradasalida);
    pthread_detach(hilo_atender_io);
  }
}

// Memoria no administra bien a las IO y no puede distinguir que IO le envía cada cosa.
// Posible solucion:  
// pthread_create(&hilo_atender_io, NULL, (void*)atender_entradasalida, fd_entradasalida);

// MALO: fd_entradasalida deja de ser global


