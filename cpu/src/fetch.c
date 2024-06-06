#include "../includes/fetch.h"

char ** ciclo_instruccion_fetch(){ //
    
	
	//sacar
	printf("Cree fetch\n");
	int i = 0;
	char* pseudo;
	char** instruccion_spliteada;
	//fin sacar

	//while(i<7){

		//SEM WAIT FETCH
		log_info(cpu_log_debug, "PID: <%d> - FETCH - PC: <%d>", contexto_ejecucion->pid, contexto_ejecucion->registros_cpu->PC);
		pedir_instruccion_pseudocodigo(contexto_ejecucion->pid,contexto_ejecucion->registros_cpu->PC);
		//SEM SIGNAL DECODE
		
		//sacar
		pseudo = recibir_instruccion_pseudocodigo();
		printf("Recibi el pseudo: %s\n",pseudo);

		instruccion_spliteada = string_split(pseudo, " ");
		printf("Spliteado param0: %s\n",instruccion_spliteada[0]);
		printf("Spliteado param1: %s\n",instruccion_spliteada[1]);
		printf("Spliteado param2: %s\n",instruccion_spliteada[2]);
		
		contexto_ejecucion->registros_cpu->PC++;
		i++;

		return instruccion_spliteada;
		//sleep(3);
		//fin sacar
	//}

}