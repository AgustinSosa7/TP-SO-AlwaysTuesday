#include <stdlib.h>
#include <stdio.h>
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
log_info(memoria_logger, "Esperando a EntradaSalida...");
fd_entradasalida = esperar_cliente(fd_memoria, memoria_logger, "ENTRADA SALIDA");
gestionar_handshake_como_server(fd_entradasalida, memoria_logger, "ENTRADA SALIDA");

/////////////////////// Lectura del Pseudocodigo/////////////////////////////////////

list_add(procesos_memoria, crear_proceso_nuevo());

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



