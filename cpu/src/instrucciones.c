#include "../includes/instrucciones.h"
/*
void comparacion_de_strings(){
    if(strcmp(instruccion_a_ejecutar[0], "SET") == 0){//[SET][AX][22]
		log_info(cpu_log_obligatorio, "PID: <%d> - Ejecutando: <%s> - <%s> - <%s>", contexto->proceso_pid, instruccion_a_ejecutar[0], instruccion_a_ejecutar[1], instruccion_a_ejecutar[2]);
		contexto->proceso_ip = contexto->proceso_ip + 1;
		uint32_t* registro_referido = detectar_registro(instruccion_a_ejecutar[1]);
		*registro_referido = atoi(instruccion_a_ejecutar[2]);

    }else if(strcmp(instruccion_a_ejecutar[0], "SUM") == 0){//[SUM][destino:AX][origen:BX]
		log_info(cpu_log_obligatorio, "PID: <%d> - Ejecutando: <%s> - <%s> - <%s>", contexto->proceso_pid, instruccion_a_ejecutar[0], instruccion_a_ejecutar[1], instruccion_a_ejecutar[2]);
		contexto->proceso_ip = contexto->proceso_ip + 1;
		uint32_t* registro_referido_destino = detectar_registro(instruccion_a_ejecutar[1]);
		uint32_t* registro_referido_origen = detectar_registro(instruccion_a_ejecutar[2]);
		*registro_referido_destino += *registro_referido_origen;

	}else if(strcmp(instruccion_a_ejecutar[0], "SUB") == 0){//[SUB][destino:AX][origen:BX]
		log_info(cpu_log_obligatorio, "PID: <%d> - Ejecutando: <%s> - <%s> - <%s>", contexto->proceso_pid, instruccion_a_ejecutar[0], instruccion_a_ejecutar[1], instruccion_a_ejecutar[2]);
		contexto->proceso_ip = contexto->proceso_ip + 1;
		uint32_t* registro_referido_destino = detectar_registro(instruccion_a_ejecutar[1]);
		uint32_t* registro_referido_origen = detectar_registro(instruccion_a_ejecutar[2]);
		*registro_referido_destino -= *registro_referido_origen;

	}else if(strcmp(instruccion_a_ejecutar[0], "JNZ") == 0){// [JNZ][Registro][Instruccion]
		log_info(cpu_log_obligatorio, "PID: <%d> - Ejecutando: <%s> - <%s> - <%s>", contexto->proceso_pid, instruccion_a_ejecutar[0], instruccion_a_ejecutar[1], instruccion_a_ejecutar[2]);
		uint32_t* registro_referido = detectar_registro(instruccion_a_ejecutar[1]);
		if(*registro_referido != 0) {
			contexto->proceso_ip = atoi(instruccion_a_ejecutar[2]);
		}else{
			contexto->proceso_ip ++;
		}

	}else if(strcmp(instruccion_a_ejecutar[0], "IO_GEN_SLEEP") == 0){// [IO_GEN_SLEEP][INTERFAZ][Tiempo]
		log_info(cpu_log_obligatorio, "PID: <%d> - Ejecutando: <%s> - <%s>", contexto->proceso_pid, instruccion_a_ejecutar[0], instruccion_a_ejecutar[2]);
		contexto->proceso_ip = contexto->proceso_ip + 1; //esto no se para que se haria

		//Enviar al KERNEL: [PEDIDO_IO][SLEEP][INTERFAZ][Tiempo]
		t_paquete* paquete = crear_paquete(PEDIDO_IO);
		agregar_string_al_paquete(paquete, instruccion_a_ejecutar[0])
		agregar_string_al_paquete(paquete, instruccion_a_ejecutar[1])
		agregar_algo_al_paquete(paquete, instruccion_a_ejecutar[2], sizeof(int));

		hay_que_desalojar = true; //tampoco se para que se haria

	}else if(strcmp(instruccion_a_ejecutar[0], "IO_STDOUT_READ") == 0){// [IO_STDOUT_READ][INTERFAZ][RegistroDireccion][RegistroTamanio]
		log_info(cpu_log_obligatorio, "PID: <%d> - Ejecutando: <%s> - <%s> - <%s>", contexto->proceso_pid, instruccion_a_ejecutar[0], instruccion_a_ejecutar[2], instruccion_a_ejecutar[3]);
		contexto->proceso_ip = contexto->proceso_ip + 1; //esto no se para que se haria

		//Enviar al KERNEL: [PEDIDO_IO][IO_STDOUT_READ][INTERFAZ][RegistroDireccion][RegistroTamanio]
		t_paquete* paquete = crear_paquete(PEDIDO_IO);
		agregar_string_al_paquete(paquete, instruccion_a_ejecutar[0])
		agregar_string_al_paquete(paquete, instruccion_a_ejecutar[1]);
		agregar_string_al_paquete(paquete, instruccion_a_ejecutar[2]);
		agregar_string_al_paquete(paquete, instruccion_a_ejecutar[3]);		

		hay_que_desalojar = true; //tampoco se para que se haria

	}else if(strcmp(instruccion_a_ejecutar[0], "IO_STDOUT_WRITE") == 0){// [IO_STDOUT_WRITE][INTERFAZ][RegistroDireccion][RegistroTamanio]
		log_info(cpu_log_obligatorio, "PID: <%d> - Ejecutando: <%s> - <%s> - <%s>", contexto->proceso_pid, instruccion_a_ejecutar[0], instruccion_a_ejecutar[2], instruccion_a_ejecutar[3]);
		contexto->proceso_ip = contexto->proceso_ip + 1; //esto no se para que se haria

		//Enviar al KERNEL: [PEDIDO_IO][IO_STDOUT_WRITE][INTERFAZ][RegistroDireccion][RegistroTamanio]
		t_paquete* paquete = crear_paquete(PEDIDO_IO);
		agregar_string_al_paquete(paquete, instruccion_a_ejecutar[0])
		agregar_string_al_paquete(paquete, instruccion_a_ejecutar[1]);
		agregar_string_al_paquete(paquete, instruccion_a_ejecutar[2]);
		agregar_string_al_paquete(paquete, instruccion_a_ejecutar[3]);

		hay_que_desalojar = true; //tampoco se para que se haria

	}

}*/