#include <stdlib.h>
#include <stdio.h>
#include <../includes/kernel.h>




int main() {
// INICIAR KERNEL


inicializar_kernel();


//Conectarse con Memoria
/*
fd_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA); 
log_info(kernel_logger, "Conexion con MEMORIA exitosa.");

//Conectarse con Filesystem
//fd_entradasalida = crear_conexion(IP_, PUERTO_ESCUCHA); 
//log_info(kernel_logger, "Conexion con FILESISTEM exitosa.");

//Conectarse con CPU
fd_cpu_dispatch = crear_conexion(IP_CPU, PUERTO_CPU_DISPATCH); 
log_info(kernel_logger, "Conexion con CPU DISPATCH exitosa.");
fd_cpu_interrupt = crear_conexion(IP_CPU, PUERTO_CPU_INTERRUPT); 
log_info(kernel_logger, "Conexion con CPU INTERRUPT exitosa.");

*/

//INICIAR EL SERVIDOR PARA ENTRADA Y SALIDA.
fd_kernel = iniciar_servidor(PUERTO_ESCUCHA, kernel_logger, "KERNEL INICIADO!");

log_info(kernel_logger,"Esperando una entrada y salida...");
fd_entradasalida = esperar_cliente(fd_kernel, kernel_logger,"ENTRADASALIDA");

//Atender los mensajes de Entrada y salida

return EXIT_SUCCESS;
}