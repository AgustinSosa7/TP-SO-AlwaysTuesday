#include "../includes/ciclo_instruccion.h"


void ciclo_instruccion(){                         
    
    while(1){
	log_warning(cpu_logger, "Esperando una nueva instruccion... \n");
    sem_wait(&sem_ciclo_de_instruccion);
	log_info(cpu_logger, "Tome el semaforo del ciclo de instruccion.\n");

	dejar_de_ejecutar = false;
	pthread_mutex_lock(&mutex_ocurrio_interrupcion);
	ocurrio_interrupcion = false;
	pthread_mutex_unlock(&mutex_ocurrio_interrupcion);
    
	//FETCH 
	log_info(cpu_logger, "PID: <%d> - FETCH - PC: <%d>", pcb_global->pid, pcb_global->registros_cpu->PC);
	pedir_instruccion_pseudocodigo(pcb_global->pid, pcb_global->registros_cpu->PC);
    
	char *instruccion_con_parametros = recibir_instruccion_pseudocodigo();
	
	//DECODE & EXECUTE
	char *saveptr = instruccion_con_parametros;
	char *nombre_instruccion = strtok_r(saveptr, " ", &saveptr);
    
	if (strcmp(nombre_instruccion, "SET") == 0)
		{
			log_info(cpu_logger, "Instruccion Ejecutada: \"PID: %d - Ejecutando: %s - %s \"", pcb_global->pid, nombre_instruccion, saveptr);
			char *nombre_registro = strtok_r(saveptr, " ", &saveptr);
			int valor = atoi(strtok_r(saveptr, " ", &saveptr));
			escribir_valor_a_registro(nombre_registro, valor);
			pcb_global->registros_cpu->PC++;
		}
	else if (strcmp(nombre_instruccion, "MOV_IN") == 0)
		{
			log_info(cpu_logger, "Instruccion Ejecutada: \"PID: %d - Ejecutando: %s - %s \"", pcb_global->pid, nombre_instruccion, saveptr);
			char *registro_datos = strtok_r(saveptr, " ", &saveptr);
			char *registro_direccion = strtok_r(saveptr, " ", &saveptr);
			int direccion_logica = leer_valor_de_registro(registro_direccion);
			int direccion_fisica = mmu(direccion_logica);
			
			if (direccion_fisica != -1)
			{
				u_int32_t valor_leido_en_memoria = leer_valor_de_memoria(pcb_global->pid, direccion_fisica);
				escribir_valor_a_registro(registro_datos, valor_leido_en_memoria);
				log_info(cpu_logger, "Lectura/Escritura Memoria: \"PID: %d - Acción: LEER - Dirección Física: %d - Valor: %d\"", pcb_global->pid, direccion_fisica, valor_leido_en_memoria);
				pcb_global->registros_cpu->PC++;
			}
		}
	else if (strcmp(nombre_instruccion, "MOV_OUT") == 0)
		{
			log_info(cpu_logger, "Instruccion Ejecutada: \"PID: %d - Ejecutando: %s - %s \"", pcb_global->pid, nombre_instruccion, saveptr);
			char *registro_direccion = strtok_r(saveptr, " ", &saveptr);
			char *registro_datos = strtok_r(saveptr, " ", &saveptr);
			int direccion_logica = leer_valor_de_registro(registro_direccion);
			int direccion_fisica = mmu(direccion_logica);
			
			if (direccion_fisica != -1)
			{
				uint32_t valor_a_escribir = leer_valor_de_registro(registro_datos);
				escribir_valor_en_memoria(pcb_global->pid, direccion_fisica, valor_a_escribir);
				log_info(cpu_logger, "Lectura/Escritura Memoria: \"PID: %d - Acción: ESCRIBIR - Dirección Física: %d - Valor: %d\"", pcb_global->pid, direccion_fisica, valor_a_escribir);
				pcb_global->registros_cpu->PC++;
			}
		}	
	else if (strcmp(nombre_instruccion, "SUM") == 0)
		{
			log_info(cpu_logger, "Instruccion Ejecutada: \"PID: %d - Ejecutando: %s - %s \"", pcb_global->pid, nombre_instruccion, saveptr);
			char *nombre_registro_destino = strtok_r(saveptr, " ", &saveptr);
			char *nombre_registro_origen = strtok_r(saveptr, " ", &saveptr);
			escribir_valor_a_registro(nombre_registro_destino, leer_valor_de_registro(nombre_registro_destino) + leer_valor_de_registro(nombre_registro_origen));
			pcb_global->registros_cpu->PC++;
		}
	else if (strcmp(nombre_instruccion, "SUB") == 0)
		{
			log_info(cpu_logger, "Instruccion Ejecutada: \"PID: %d - Ejecutando: %s - %s \"", pcb_global->pid, nombre_instruccion, saveptr);
			char *nombre_registro_destino = strtok_r(saveptr, " ", &saveptr);
			char *nombre_registro_origen = strtok_r(saveptr, " ", &saveptr);
			escribir_valor_a_registro(nombre_registro_destino, leer_valor_de_registro(nombre_registro_destino) - leer_valor_de_registro(nombre_registro_origen));
			pcb_global->registros_cpu->PC++;
		}
	else if (strcmp(nombre_instruccion, "JNZ") == 0)
		{
			log_info(cpu_logger, "Instruccion Ejecutada: \"PID: %d - Ejecutando: %s - %s \"", pcb_global->pid, nombre_instruccion, saveptr);
			char *nombre_registro = strtok_r(saveptr, " ", &saveptr);
			u_int32_t nuevo_program_counter = atoi(strtok_r(saveptr, " ", &saveptr));
			if (leer_valor_de_registro(nombre_registro) != 0)
			{
				escribir_valor_a_registro("PC", nuevo_program_counter);
			}
			else
			{
				pcb_global->registros_cpu->PC++;
			}
		}
	else if (strcmp(nombre_instruccion, "RESIZE") == 0)
		{
			log_info(cpu_logger, "Instruccion Ejecutada: \"PID: %d - Ejecutando: %s - %s \"", pcb_global->pid, nombre_instruccion, saveptr);
			int tamanio = atoi(strtok_r(saveptr, " ", &saveptr));
			int nuevoTamanioDelProceso = pedir_ajustar_tamanio_del_proceso(pcb_global->pid, tamanio);
			bool outOfMemory = nuevoTamanioDelProceso == -1;
			log_info(cpu_logger, "RESPUESTA DE REZISE: PID: %d - Nuevo tamaño intentado: %d", pcb_global->pid, nuevoTamanioDelProceso);
			if(outOfMemory){
				log_info(cpu_logger, "Out of Memory: PID: %d - Nuevo tamaño intentado: %d", pcb_global->pid, tamanio);
				dejar_de_ejecutar = true;
				devolver_contexto_por_out_of_memory();
			}
			pcb_global->registros_cpu->PC++;
		}
	else if (strcmp(nombre_instruccion, "WAIT") == 0)
		{
			log_info(cpu_logger, "Instruccion Ejecutada: \"PID: %d - Ejecutando: %s - %s \"", pcb_global->pid, nombre_instruccion, saveptr);
			char *nombre_recurso = strtok_r(saveptr, " ", &saveptr);
			pcb_global->registros_cpu->PC++;
			dejar_de_ejecutar = true;
			devolver_contexto_por_wait(nombre_recurso);
		}
	else if (strcmp(nombre_instruccion, "SIGNAL") == 0)
		{
			log_info(cpu_logger, "Instruccion Ejecutada: \"PID: %d - Ejecutando: %s - %s \"", pcb_global->pid, nombre_instruccion, saveptr);
			char *nombre_recurso = strtok_r(saveptr, " ", &saveptr);
			pcb_global->registros_cpu->PC++;
			dejar_de_ejecutar = true;
			devolver_contexto_por_signal(nombre_recurso);
		}
	else if (strcmp(nombre_instruccion, "IO_GEN_SLEEP") == 0)
		{
			log_info(cpu_logger, "Instruccion Ejecutada: \"PID: %d - Ejecutando: %s - %s \"", pcb_global->pid, nombre_instruccion, saveptr);
			char *nombre_interfaz = strtok_r(saveptr, " ", &saveptr);
			int tiempo_sleep = atoi(strtok_r(saveptr, " ", &saveptr));
			pcb_global->registros_cpu->PC++;
			dejar_de_ejecutar = true;
			devolver_contexto_por_sleep(nombre_instruccion, nombre_interfaz, tiempo_sleep);
		}
	else if (strcmp(nombre_instruccion, "IO_STDIN_READ") == 0)
		{
			log_info(cpu_logger, "Instruccion Ejecutada: \"PID: %d - Ejecutando: %s - %s \"", pcb_global->pid, nombre_instruccion, saveptr);
			char *nombre_interfaz = strtok_r(saveptr, " ", &saveptr);
			char *registro_direccion = strtok_r(saveptr, " ", &saveptr);
			char *registro_tamanio = strtok_r(saveptr, " ", &saveptr);

			int direccion_logica = leer_valor_de_registro(registro_direccion);
			int direccion_fisica = mmu(direccion_logica);
			int tamanio = leer_valor_de_registro(registro_tamanio);

			if (direccion_fisica != -1) // Si la traduccion de la direccion logica arroja un Page Fault, se devuelve el contexto por page fault y no se ejecuta el resto!.
			{
				pcb_global->registros_cpu->PC++;
				dejar_de_ejecutar = true;
				devolver_contexto_por_stdin_read(nombre_instruccion, nombre_interfaz, direccion_fisica, tamanio);
			}
			
		}
	else if (strcmp(nombre_instruccion, "IO_STDOUT_WRITE") == 0)
		{
			log_info(cpu_logger, "Instruccion Ejecutada: \"PID: %d - Ejecutando: %s - %s \"", pcb_global->pid, nombre_instruccion, saveptr);
			char *nombre_interfaz = strtok_r(saveptr, " ", &saveptr);
			char *registro_direccion = strtok_r(saveptr, " ", &saveptr);
			char *registro_tamanio = strtok_r(saveptr, " ", &saveptr);

			int direccion_logica = leer_valor_de_registro(registro_direccion);
			int direccion_fisica = mmu(direccion_logica);
			int tamanio = leer_valor_de_registro(registro_tamanio);

			if (direccion_fisica != -1) // Si la traduccion de la direccion logica arroja un Page Fault, se devuelve el contexto por page fault y no se ejecuta el resto!.
			{
				pcb_global->registros_cpu->PC++;
				dejar_de_ejecutar = true;
				devolver_contexto_por_stdout_write(nombre_instruccion, nombre_interfaz, direccion_fisica, tamanio);
			}
			
		}
	else if (strcmp(nombre_instruccion, "IO_FS_CREATE") == 0)
		{
			log_info(cpu_logger, "Instruccion Ejecutada: \"PID: %d - Ejecutando: %s - %s \"", pcb_global->pid, nombre_instruccion, saveptr);
			char *nombre_interfaz = strtok_r(saveptr, " ", &saveptr);
			char *nombre_archivo = strtok_r(saveptr, " ", &saveptr);
			pcb_global->registros_cpu->PC++;
			dejar_de_ejecutar = true;
			devolver_contexto_por_fs_create(nombre_instruccion, nombre_interfaz, nombre_archivo);
		}
	else if (strcmp(nombre_instruccion, "IO_FS_DELETE") == 0)
		{
			log_info(cpu_logger, "Instruccion Ejecutada: \"PID: %d - Ejecutando: %s - %s \"", pcb_global->pid, nombre_instruccion, saveptr);
			char *nombre_interfaz = strtok_r(saveptr, " ", &saveptr);
			char *nombre_archivo = strtok_r(saveptr, " ", &saveptr);
			pcb_global->registros_cpu->PC++;
			dejar_de_ejecutar = true;
			devolver_contexto_por_fs_delete(nombre_instruccion, nombre_interfaz, nombre_archivo);
		}
	else if (strcmp(nombre_instruccion, "EXIT") == 0)
		{
			log_info(cpu_logger, "Instruccion Ejecutada: \"PID: %d - Ejecutando: %s \"", pcb_global->pid, nombre_instruccion);
			dejar_de_ejecutar = true;
			devolver_contexto_por_correcta_finalizacion();
		}

	free(instruccion_con_parametros);

    //CHECK INTERRUPT
    pthread_mutex_lock(&mutex_ocurrio_interrupcion);
	if (ocurrio_interrupcion && !dejar_de_ejecutar)
	{
		pthread_mutex_unlock(&mutex_ocurrio_interrupcion);
		log_info(cpu_logger, "DEVUELVO INTERRUPT");
		dejar_de_ejecutar = true;
		devolver_contexto_por_ser_interrumpido();
	}
	else
	{
		pthread_mutex_unlock(&mutex_ocurrio_interrupcion);
	}

	if (!dejar_de_ejecutar)
	{
		// Reinicio el ciclo de instruccion, ejecutando la próxima.
		sem_post(&sem_ciclo_de_instruccion);
	}
	else
	{	
		// Queda bloqueado esperando que vuelvan a ejecutar un proceso en la CPU.
		log_info(cpu_logger, "FRENO");
	}

	}
}

// Formato del Paquete: OP_CODE, BUFFER [PCB, INSTRUCCION, PARAMETROS]
/*
OP_CODE's que envía CPU a Kernel:
DESALOJO_QUANTUM
PEDIDO_IO
PROCESO_EXIT
*/

void devolver_contexto_por_ser_interrumpido()
{
	t_paquete* paquete = crear_paquete(DESALOJO_QUANTUM);
	agregar_pcb_a_paquete(pcb_global, paquete);
	agregar_int_a_paquete(paquete, motivo_interrupcion);
	enviar_paquete(paquete, fd_kernel_dispatch);
    eliminar_paquete(paquete);
}

void devolver_contexto_por_page_fault(int numero_de_pagina)
{
	t_paquete* paquete = crear_paquete(DEVOLVER_PROCESO_POR_PAGEFAULT);
	agregar_pcb_a_paquete(pcb_global, paquete);
	agregar_int_a_paquete(paquete, numero_de_pagina);
	enviar_paquete(paquete, fd_kernel_dispatch);
    eliminar_paquete(paquete);
}

void devolver_contexto_por_out_of_memory() // Si Kernel lo necesitara, podría pasarle además del PCB, el tamaño al cual se intentó agrandar el tamaño del proceso en Memoria.
{
	t_paquete* paquete = crear_paquete(DEVOLVER_PROCESO_POR_OUT_OF_MEMORY);
	agregar_pcb_a_paquete(pcb_global, paquete);
	enviar_paquete(paquete, fd_kernel_dispatch);
    eliminar_paquete(paquete);
}

void devolver_contexto_por_wait(char* nombre_recurso)
{
	t_paquete* paquete = crear_paquete(WAIT);
	agregar_pcb_a_paquete(pcb_global, paquete);
	agregar_string_a_paquete(paquete, nombre_recurso);
	enviar_paquete(paquete, fd_kernel_dispatch);
    eliminar_paquete(paquete);
}

void devolver_contexto_por_signal(char* nombre_recurso)
{
	t_paquete* paquete = crear_paquete(SIGNAL);
	agregar_pcb_a_paquete(pcb_global, paquete);
	agregar_string_a_paquete(paquete, nombre_recurso);
	enviar_paquete(paquete, fd_kernel_dispatch);
    eliminar_paquete(paquete);
}

void devolver_contexto_por_sleep(char* nombre_instruccion, char* nombre_interfaz, int segundos_sleep)
{
	t_paquete* paquete = crear_paquete(PEDIDO_IO);
	agregar_pcb_a_paquete(pcb_global, paquete);
	agregar_string_a_paquete(paquete, nombre_instruccion);
	agregar_string_a_paquete(paquete, nombre_interfaz);
	agregar_int_a_paquete(paquete, segundos_sleep);
	enviar_paquete(paquete, fd_kernel_dispatch);
    eliminar_paquete(paquete);
}

void devolver_contexto_por_stdin_read(char* nombre_instruccion, char* nombre_interfaz, int direccion_fisica, int tamanio)
{
	t_paquete* paquete = crear_paquete(PEDIDO_IO);
	agregar_pcb_a_paquete(pcb_global, paquete);
	agregar_string_a_paquete(paquete, nombre_instruccion);
	agregar_string_a_paquete(paquete, nombre_interfaz);
	agregar_int_a_paquete(paquete, direccion_fisica);
	agregar_int_a_paquete(paquete, tamanio);
	enviar_paquete(paquete, fd_kernel_dispatch);
    eliminar_paquete(paquete);
}

void devolver_contexto_por_stdout_write(char* nombre_instruccion, char* nombre_interfaz, int direccion_fisica, int tamanio)
{
	t_paquete* paquete = crear_paquete(PEDIDO_IO);
	agregar_pcb_a_paquete(pcb_global, paquete);
	agregar_string_a_paquete(paquete, nombre_instruccion);
	agregar_string_a_paquete(paquete, nombre_interfaz);
	agregar_int_a_paquete(paquete, direccion_fisica);
	agregar_int_a_paquete(paquete, tamanio);
	enviar_paquete(paquete, fd_kernel_dispatch);
    eliminar_paquete(paquete);
}

void devolver_contexto_por_fs_create(char* nombre_instruccion, char* nombre_interfaz, char* nombre_archivo)
{
	t_paquete* paquete = crear_paquete(PEDIDO_IO);
	agregar_pcb_a_paquete(pcb_global, paquete);
	agregar_string_a_paquete(paquete, nombre_instruccion);
	agregar_string_a_paquete(paquete, nombre_interfaz);
	agregar_string_a_paquete(paquete, nombre_archivo);
	enviar_paquete(paquete, fd_kernel_dispatch);
    eliminar_paquete(paquete);
}

void devolver_contexto_por_fs_delete(char* nombre_instruccion, char* nombre_interfaz, char* nombre_archivo)
{
	t_paquete* paquete = crear_paquete(PEDIDO_IO);
	agregar_pcb_a_paquete(pcb_global, paquete);
	agregar_string_a_paquete(paquete, nombre_instruccion);
	agregar_string_a_paquete(paquete, nombre_interfaz);
	agregar_string_a_paquete(paquete, nombre_archivo);
	enviar_paquete(paquete, fd_kernel_dispatch);
    eliminar_paquete(paquete);
}

void devolver_contexto_por_correcta_finalizacion()
{
	t_paquete* paquete = crear_paquete(DEVOLVER_PROCESO_POR_CORRECTA_FINALIZACION);
	agregar_pcb_a_paquete(pcb_global, paquete);
	enviar_paquete(paquete, fd_kernel_dispatch);
	printf("paquete enviado al kernel por EXIT\n");
    eliminar_paquete(paquete);
}

void escribir_valor_a_registro(char* nombre_registro, u_int32_t valor)
{
	if (strcmp(nombre_registro, "AX") == 0)
	{
		pcb_global->registros_cpu->AX = (u_int8_t)valor;
		log_info(cpu_logger, "Se le asigno al registro '%s' el valor %d.", nombre_registro, valor);
	}
	else if (strcmp(nombre_registro, "BX") == 0)
	{
		pcb_global->registros_cpu->BX = (u_int8_t)valor;
		log_info(cpu_logger, "Se le asigno al registro '%s' el valor %d.", nombre_registro, valor);
	}
	else if (strcmp(nombre_registro, "CX") == 0)
	{
		pcb_global->registros_cpu->CX = (u_int8_t)valor;
		log_info(cpu_logger, "Se le asigno al registro '%s' el valor %d.", nombre_registro, valor);
	}
	else if (strcmp(nombre_registro, "DX") == 0)
	{
		pcb_global->registros_cpu->DX = (u_int8_t)valor;
		log_info(cpu_logger, "Se le asigno al registro '%s' el valor %d.", nombre_registro, valor);
	}
	else if (strcmp(nombre_registro, "EAX") == 0)
	{
		pcb_global->registros_cpu->EAX = valor;
		log_info(cpu_logger, "Se le asigno al registro '%s' el valor %d.", nombre_registro, valor);
	}
	else if (strcmp(nombre_registro, "EBX") == 0)
	{
		pcb_global->registros_cpu->EBX = valor;
		log_info(cpu_logger, "Se le asigno al registro '%s' el valor %d.", nombre_registro, valor);
	}
	else if (strcmp(nombre_registro, "ECX") == 0)
	{
		pcb_global->registros_cpu->ECX = valor;
		log_info(cpu_logger, "Se le asigno al registro '%s' el valor %d.", nombre_registro, valor);
	}
	else if (strcmp(nombre_registro, "EDX") == 0)
	{
		pcb_global->registros_cpu->EDX = valor;
		log_info(cpu_logger, "Se le asigno al registro '%s' el valor %d.", nombre_registro, valor);
	}
	else if (strcmp(nombre_registro, "PC") == 0)
	{
		pcb_global->registros_cpu->PC = valor;
		log_info(cpu_logger, "Se le asigno al registro '%s' el valor %d.", nombre_registro, valor);
	}
	else if (strcmp(nombre_registro, "SI") == 0)
	{
		pcb_global->registros_cpu->SI = valor;
		log_info(cpu_logger, "Se le asigno al registro '%s' el valor %d.", nombre_registro, valor);
	}
	else if (strcmp(nombre_registro, "DI") == 0)
	{
		pcb_global->registros_cpu->DI = valor;
		log_info(cpu_logger, "Se le asigno al registro '%s' el valor %d.", nombre_registro, valor);
	}
	else
	{
		log_error(cpu_logger, "No se asigno el valor %d al registro '%s' porque es un registro no conocido.", valor, nombre_registro);
	}
}

uint32_t leer_valor_de_registro(char* nombre_registro)
{
	if (strcmp(nombre_registro, "AX") == 0)
	{
		log_trace(cpu_logger, "Se leyo desde el registro '%s' el valor %d.", nombre_registro, pcb_global->registros_cpu->AX);
		return (uint32_t)pcb_global->registros_cpu->AX;
	}
	else if (strcmp(nombre_registro, "BX") == 0)
	{
		log_trace(cpu_logger, "Se leyo desde el registro '%s' el valor %d.", nombre_registro, pcb_global->registros_cpu->BX);
		return (uint32_t)pcb_global->registros_cpu->BX;
	}
	else if (strcmp(nombre_registro, "CX") == 0)
	{
		log_trace(cpu_logger, "Se leyo desde el registro '%s' el valor %d.", nombre_registro, pcb_global->registros_cpu->CX);
		return (uint32_t)pcb_global->registros_cpu->CX;
	}
	else if (strcmp(nombre_registro, "DX") == 0)
	{
		log_trace(cpu_logger, "Se leyo desde el registro '%s' el valor %d.", nombre_registro, pcb_global->registros_cpu->DX);
		return (uint32_t)pcb_global->registros_cpu->DX;
	}
	else if (strcmp(nombre_registro, "EAX") == 0)
	{
		log_trace(cpu_logger, "Se leyo desde el registro '%s' el valor %d.", nombre_registro, pcb_global->registros_cpu->EAX);
		return pcb_global->registros_cpu->EAX;
	}
	else if (strcmp(nombre_registro, "EBX") == 0)
	{
		log_trace(cpu_logger, "Se leyo desde el registro '%s' el valor %d.", nombre_registro, pcb_global->registros_cpu->EBX);
		return pcb_global->registros_cpu->EBX;
	}
	else if (strcmp(nombre_registro, "ECX") == 0)
	{
		log_trace(cpu_logger, "Se leyo desde el registro '%s' el valor %d.", nombre_registro, pcb_global->registros_cpu->ECX);
		return pcb_global->registros_cpu->ECX;
	}
	else if (strcmp(nombre_registro, "EDX") == 0)
	{
		log_trace(cpu_logger, "Se leyo desde el registro '%s' el valor %d.", nombre_registro, pcb_global->registros_cpu->EDX);
		return pcb_global->registros_cpu->EDX;
	}
	else if (strcmp(nombre_registro, "PC") == 0)
	{
		log_trace(cpu_logger, "Se leyo desde el registro '%s' el valor %d.", nombre_registro, pcb_global->registros_cpu->PC);
		return pcb_global->registros_cpu->PC;
	}
	else if (strcmp(nombre_registro, "SI") == 0)
	{
		log_trace(cpu_logger, "Se leyo desde el registro '%s' el valor %d.", nombre_registro, pcb_global->registros_cpu->SI);
		return pcb_global->registros_cpu->SI;
	}
	else if (strcmp(nombre_registro, "DI") == 0)
	{
		log_trace(cpu_logger, "Se leyo desde el registro '%s' el valor %d.", nombre_registro, pcb_global->registros_cpu->DI);
		return pcb_global->registros_cpu->DI;
	}

	log_error(cpu_logger, "No se leyo nada del registro '%s' porque es un registro no conocido.", nombre_registro);
	return -1;
}

int mmu(int direccion_logica)
{
	int numero_de_pagina = floor(direccion_logica / tamanio_pagina);
	int numero_de_marco = pedir_numero_de_marco_a_memoria(numero_de_pagina);
	bool page_fault = numero_de_marco == -1;

	if (page_fault)
	{
		log_info(cpu_logger, "Page Fault: Page Fault PID: %d - Pagina: %d", pcb_global->pid, numero_de_pagina);
		dejar_de_ejecutar = true;
		devolver_contexto_por_page_fault(numero_de_pagina);
		return -1;
	}

	log_info(cpu_logger, "PID: <%d> - OBTENER MARCO - Página: <%d> - Marco: <%d>", pcb_global->pid, numero_de_pagina, numero_de_marco);
	int desplazamiento = direccion_logica - numero_de_pagina * tamanio_pagina;
	int direccion_fisica = numero_de_marco * tamanio_pagina + desplazamiento;

	return direccion_fisica;
}
	
	
	
	
	
	
	
	/////////////////////////////////////////////     FETCH    ////////////////////////////////////////

/*
char ** ciclo_instruccion_fetch(){ //
	//sacar
	printf("Cree fetch\n");
	char* pseudo;
	char** instruccion_spliteada;
	//fin sacar
		printf("pid del pcb_global: %d\n", pcb_global->pid);

		pedir_instruccion_pseudocodigo(pcb_global->pid,pcb_global->registros_cpu->PC);
		
		//sacar
		pseudo = recibir_instruccion_pseudocodigo(); 
		printf("Recibi el pseudo: %s\n",pseudo);

		instruccion_spliteada = string_split(pseudo, " ");
		printf("Spliteado param0fetch: %s\n",instruccion_spliteada[0]);
		printf("Spliteado param1fetch: %s\n",instruccion_spliteada[1]);
		printf("Spliteado param2fetch: %s\n",instruccion_spliteada[2]);
		
		pcb_global->registros_cpu->PC++;

		return instruccion_spliteada;
		//fin sacar
	}
*/
    /////////////////////////////////////////////     DECODE    ////////////////////////////////////////

/*
void decodificacion_instruccion(char *instruccion){
	if(codigo_inexistente(instruccion)){ 
		log_error(cpu_logger, "Hubo un error leyendo %s",instruccion);
		exit(EXIT_FAILURE); 
	}
	//El tp dice que acá debería fijarse si necesita que entra la MMU o no
	//requiere de una traducción de dirección lógica a dirección física. ¿QUÉ SERÍA ESTO?
}

// Esta funcion devuelve True si la cabecera de la lista está mal
bool codigo_inexistente(char* instruccion){
	bool respuesta = true;
	int i = 0; 
	while(opcode_cpu[i] != NULL){
		if(strcmp(opcode_cpu[i], instruccion) == 0) {
			respuesta = false;
			printf("La instruccion %s existe ",instruccion);
			break; // para que no siga buscando 
		}
	i++;
	}
	return respuesta;
}*/






    /////////////////////////////////////////////     EXECUTE    ////////////////////////////////////////

/*
void ejecucion_proceso(char** instruccion){
switch (identificador_instruccion(instruccion[0]))
{
    case IO_GEN_SLEEP: // [IO_GEN_SLEEP][INTERFAZ][Tiempo]
		log_info(cpu_logger, "PID: <%d> - Ejecutando: <%s> - <%s>", pcb_global->pid, instruccion[0], instruccion[2]);
		pcb_global->registros_cpu->PC = pcb_global->registros_cpu->PC + 1;

		//Enviar al KERNEL: [PEDIDO_IO][SLEEP][INTERFAZ][Tiempo]
		t_paquete* paquete = crear_paquete(PEDIDO_IO);
		agregar_string_a_paquete(paquete, instruccion[0]);
		agregar_string_a_paquete(paquete, instruccion[1]);
		agregar_int_a_paquete(paquete, atoi(instruccion[2]));

		//hay_que_desalojar = true; //No se para que se haria
    
    break;

    case IO_STDIN_READ:// [IO_STDOUT_READ][INTERFAZ][RegistroDireccion][RegistroTamanio]
		log_info(cpu_logger, "PID: <%d> - Ejecutando: <%s> - <%s> - <%s>", pcb_global->pid, instruccion[0], instruccion[2], instruccion[3]);
		pcb_global->registros_cpu->PC = pcb_global->registros_cpu->PC + 1;

		//Enviar al KERNEL: [PEDIDO_IO][IO_STDOUT_READ][INTERFAZ][RegistroDireccion][RegistroTamanio]
		t_paquete* paquete_ = crear_paquete(PEDIDO_IO);
		agregar_string_a_paquete(paquete_, instruccion[0]);
		agregar_string_a_paquete(paquete_, instruccion[1]);
		agregar_string_a_paquete(paquete_, instruccion[2]);
		agregar_string_a_paquete(paquete_, instruccion[3]);	

		//hay_que_desalojar = true; //No se para que se haria

    break;

    case IO_STDOUT_WRITE: // [IO_STDOUT_WRITE][INTERFAZ][RegistroDireccion][RegistroTamanio]
		log_info(cpu_logger, "PID: <%d> - Ejecutando: <%s> - <%s> - <%s>", pcb_global->pid, instruccion[0], instruccion[2], instruccion[3]);
		pcb_global->registros_cpu->PC = pcb_global->registros_cpu->PC + 1;

		//Enviar al KERNEL: [PEDIDO_IO][IO_STDOUT_WRITE][INTERFAZ][RegistroDireccion][RegistroTamanio]
		t_paquete* paquete__ = crear_paquete(PEDIDO_IO);
		agregar_string_a_paquete(paquete__, instruccion[0]);
		agregar_string_a_paquete(paquete__, instruccion[1]);
		agregar_string_a_paquete(paquete__, instruccion[2]);
		agregar_string_a_paquete(paquete__, instruccion[3]);	

		// hay_que_desalojar = true; //No se para que se haria

	default:
    break;
}
}

cod_instruccion identificador_instruccion(char* codigo){
   	cod_instruccion instruccion_a_ejecutar = -1;
    if(strcmp(codigo, "SET") == 0){//[SET][AX][22]
    instruccion_a_ejecutar = SET;
    
    }else if(strcmp(codigo, "SUM") == 0){//[SUM][destino:AX][origen:BX]
    instruccion_a_ejecutar = SUM;

	}else if(strcmp(codigo, "SUB") == 0){//[SUB][destino:AX][origen:BX]
    instruccion_a_ejecutar = SUB;

	}else if(strcmp(codigo, "JNZ") == 0){// [JNZ][Registro][Instruccion]
    instruccion_a_ejecutar = JNZ;

    }else if(strcmp(codigo, "MOV_IN") == 0){
    instruccion_a_ejecutar = MOV_IN;

    }else if(strcmp(codigo, "MOV_OUT") == 0){
    instruccion_a_ejecutar = MOV_OUT;

    }else if(strcmp(codigo, "RESIZE") == 0){    
    instruccion_a_ejecutar = RESIZE;

    }else if(strcmp(codigo, "COPY_STRING") == 0){
    instruccion_a_ejecutar = COPY_STRING;

    }else if(strcmp(codigo, "WAIT") == 0){
    instruccion_a_ejecutar = WAIT;

    }else if(strcmp(codigo, "SIGNAL") == 0){
    instruccion_a_ejecutar = SIGNAL;

    }else if(strcmp(codigo, "IO_GEN_SLEEP") == 0){ // [SLEEP][tiempo]
    instruccion_a_ejecutar = IO_GEN_SLEEP;

    }else if(strcmp(codigo, "IO_STDIN_READ") == 0){
    instruccion_a_ejecutar = IO_STDIN_READ;

    }else if(strcmp(codigo, "IO_STDOUT_WRITE") == 0){
    instruccion_a_ejecutar = IO_STDOUT_WRITE;

    }else if(strcmp(codigo, "IO_FS_CREATE") == 0){
    instruccion_a_ejecutar = IO_FS_CREATE;

    }else if(strcmp(codigo, "IO_FS_DELETE") == 0){
    instruccion_a_ejecutar = IO_FS_DELETE;

    }else if(strcmp(codigo, "IO_FS_TRUNCATE") == 0){
    instruccion_a_ejecutar = IO_FS_TRUNCATE;

    }else if(strcmp(codigo, "IO_FS_WRITE") == 0){
    instruccion_a_ejecutar = IO_FS_WRITE;

    }else if(strcmp(codigo, "IO_FS_READ") == 0){
    instruccion_a_ejecutar = IO_FS_READ;

    }else if(strcmp(codigo, "EXIT") == 0){         
    instruccion_a_ejecutar = EXIT;

}  
    return instruccion_a_ejecutar;
} 
*/