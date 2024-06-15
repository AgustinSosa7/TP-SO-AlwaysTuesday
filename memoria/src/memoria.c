
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


// Esperar conexion de KERNEL
log_info(memoria_logger, "Esperando a KERNEL...");
fd_kernel = esperar_cliente(fd_memoria, memoria_logger,"KERNEL"); 
gestionar_handshake_como_server(fd_kernel, memoria_logger, "KERNEL");


// Esperar conexion de ENTRADASALIDA
   
pthread_t hilo_generador_de_io;
pthread_create(&hilo_generador_de_io, NULL, (void*)gestionar_entrada_salida, NULL);
pthread_detach(hilo_generador_de_io);

/////////////////////// Lectura del Pseudocodigo/////////////////////////////////////

list_add(procesos_memoria, crear_proceso_nuevo());
t_proceso* proceso1 = buscar_proceso_en_memoria(3);
printf("longitud: %d\n",(proceso1->long_tabla_pags));

int numero_marco = traer_numero_marco(proceso1,1);
printf("numero_marco: %d\n",numero_marco);

printf("marco libre encontrado: %d\n",buscar_marco_libre());

/*
t_proceso* proceso1 = buscar_proceso_en_memoria(3);

longitud_tabla_paginas()
*/


while(1){
t_pedido* pedido = recibir_instruccion_a_enviar();
printf("Proceso pedido: %d\n",pedido->pid);
//recv_cpu lo que tengo que pasarte. pid PC
t_proceso* proceso1 = buscar_proceso_en_memoria(pedido->pid);
printf("ENCONTRADO %d\n",proceso1->pid); //BORRAR
enviar_instruccion_pesudocodigo(proceso1->instrucciones,pedido->pc);//PROGRAM_COUNTER_PEDIDO);
free(pedido);
sleep(3);
}

// Finalizar MEMORIA

return EXIT_SUCCESS;
}



void gestionar_entrada_salida(){
  while(1){
    log_info(memoria_logger, "Esperando a EntradaSalida...");
    fd_entradasalida = esperar_cliente(fd_memoria, memoria_logger, "ENTRADA SALIDA");
    gestionar_handshake_como_server(fd_entradasalida, memoria_logger, "ENTRADA SALIDA");
    pthread_t hilo_atender_io;
    pthread_create(&hilo_atender_io, NULL, (void*)atender_entradasalida, NULL);
    pthread_detach(hilo_atender_io);
  }
}
