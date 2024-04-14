#include <stdlib.h>
#include <stdio.h>
#include <../includes/kernel.h>

#include <../src/utils/hello.h>
#include <../src/utils/utils_connections.h>


int main() {
// INICIAR KERNEL


void inicializar_kernel();

//Conectarse con Memoria
fd_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA); 
log_info(kernel_logger, "Conexion con MEMORIA exitosa.");

//Conectarse con Filesystem
//fd_entradasalida = crear_conexion(IP_, PUERTO_ESCUCHA); 
//log_info(kernel_logger, "Conexion con FILESISTEM exitosa.");

//Conectarse con CPU
fd_cpu_dispatch = crear_conexion(IP_CPU, PUERTO_CPU_DISPATCH); 
log_info(kernel_logger, "Conexion con CPU DISPATCH exitosa.");
fd_cpu_interrupt = crear_conexion(IP_CPU, PUERTO_CPU_INTERRUPT; 
log_info(kernel_logger, "Conexion con CPU INTERRUPT exitosa.");


printf("Puerto escucha %s\n",PUERTO_ESCUCHA);


fd_kernel = iniciar_servidor(PUERTO_ESCUCHA, kernel_logger, "KERNEL INICIADO!");


decir_hola("carlos");

return EXIT_SUCCESS;
}
