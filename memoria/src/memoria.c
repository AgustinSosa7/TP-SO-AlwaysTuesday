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

/////////////////////// Lectura del Pseudocodigo/////////////////////////////////////

list_add(procesos_memoria, crear_proceso_nuevo());

//recv_cpu lo que tengo que pasarte. pid PC
t_proceso* proceso1 = buscar_proceso_en_memoria(3);

printf("ENCONTRADO %d\n",proceso1->pid); //BORRAR

/*
printf("Llene la lista de longitud %d\n",list_size(proceso1->instrucciones));   
for(int i=0;i<7;i++){
    printf("%s\n",list_get(proceso1->instrucciones, i ));
    }*/

enviar_instruccion_pesudocodigo(proceso1->instrucciones,2);//PROGRAM_COUNTER_PEDIDO);

// Esperar conexion de ENTRADASALIDA
/*log_info(memoria_logger, "Esperando a EntradaSalida...");
fd_entradasalida = esperar_cliente(fd_memoria, memoria_logger, "ENTRADA SALIDA");
gestionar_handshake_como_server(fd_entradasalida, memoria_logger, "ENTRADA SALIDA");*/

// Finalizar MEMORIA

return EXIT_SUCCESS;
}



