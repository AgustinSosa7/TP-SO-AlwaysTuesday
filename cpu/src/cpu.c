#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>
#include <cpu.h>

int main(/*int argc, char* argv[]*/) {
   // decir_hola("CPU");
 //   return 0;
//}


cpu_logger = log_create("cpu.log","CPU_LOG",1,LOG_LEVEL_INFO);
if (cpu_logger == NULL) {
	perror("Algo raro pasa con el log. No se pudo crear o encontrar el archivo.");
	exit(EXIT_FAILURE);


cpu_log_debug = log_create("cpu.log","CPU_LOG",1,LOG_LEVEL_TRACE);
if (cpu_log_debug == NULL) {
	perror("Algo raro pasa con el log. No se pudo crear o encontrar el archivo.");
	exit(EXIT_FAILURE);

cpu_config = config_create("home/utnso/tp-2024-1c-AlwaysTuesday/cpu/cpu.config");
if (cpu_config == NULL) {
	perror("Error al intentar cargar el config.");
	exit(EXIT_FAILURE);

IP_MEMORIA = config_get_string_value(cpu_config,"IP_MEMORIA");
PUERTO_MEMORIA = config_get_int_value(cpu_config,"PUERTO_MEMORIA");
PUERTO_ESCUCHA_DISPATCH = config_get_int_value(cpu_config,"PUERTO_ESCUCHA_DISPATCH");
PUERTO_ESCUCHA_INTERRUPT = config_get_int_value(cpu_config,"PUERTO_ESCUCHA_INTERRUPT");
CANTIDAD_ENTRADAS = config_get_int_value(cpu_config,"CANTIDAD_ENTRADAS");
ALGORITMO_TLB = config_get_string_value(cpu_config,"ALGORITMO_TLB");    

return EXIT_SUCCESS;
}