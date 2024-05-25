#include "../includes/cpu.h"


bool validador_de_header(char* header_string){
	bool respuesta = false;
	int i = 0;
	while(string[i] != NULL){
		if(strcmp(string[i], header_string) == 0) respuesta = true;
		i++;
	}
	return respuesta;
}

//  probando que existe el header de la instruccion
void correr_decode(){
	if(validador_de_header(instruccion_a_ejecutar[0])){   
		// log_info(cpu_logger, "Instruccion Validada: [%s] -> OK", instruccion_a_ejecutar[0]);
		sem_post(&sem_control_decode_execute);
	}else{
		log_error(cpu_logger, "Instruccion no encontrada: [PC: %d][Instruc_Header: %s]", contexto->proceso_ip, instruccion_a_ejecutar[0]);
		exit(EXIT_FAILURE); 
	}
}