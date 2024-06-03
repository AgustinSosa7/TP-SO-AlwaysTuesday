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

	}else if(strcmp(instruccion_a_ejecutar[0], "SLEEP") == 0){// [SLEEP][tiempo]
		log_info(cpu_log_obligatorio, "PID: <%d> - Ejecutando: <%s> - <%s>", contexto->proceso_pid, instruccion_a_ejecutar[0], instruccion_a_ejecutar[1]);
		contexto->proceso_ip = contexto->proceso_ip + 1;

		//Enviar al KERNEL: [contexto]["SLEEP"][Tiempo]
		mochila = crear_super_paquete(100);
		cargar_string_al_super_paquete(mochila, instruccion_split[0]); // instruccion_split[0]: instruccion
		cargar_int_al_super_paquete(mochila, atoi(instruccion_split[1])); //otros perametros necesarios, en este caso el tiempo

		hay_que_desalojar = true;

	}

}*/