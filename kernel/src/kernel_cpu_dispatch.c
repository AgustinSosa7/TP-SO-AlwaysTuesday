#include "../includes/kernel_cpu_dispatch.h"


void atender_kernel_cpu_dispatch(){
    bool control_key = 1;
    while (control_key) {
        int cod_op = recibir_operacion(fd_cpu_dispatch);
    //  t_buffer* unBuffer;
	//	log_info(kernel_logger, "Se recibio algo de CPU_Dispatch");
	//	unBuffer = recibir_paquete(fd_cpu_dispatcher);
        switch (cod_op)
        {
        case ATENDER_INSTRUCCION_INTERFAZ_KERNEL:
            
            break;
        case -1:
          //  log_error(logger, "Desconexion de CPU - DISPATCH");      
            control_key = 0;
        default:
           // log_warning(logger, "Operacion desconocida de CPU - DISPATCH");
            break;
        }
    }
}

/*void _enviar_pcb_a_CPU_por_dispatch(t_pcb* una_pcb){  //Faltaría crear el PCB
	t_paquete* un_paquete = crear_super_paquete(EJECUTAR_PROCESO_KC); //Ejecutar, ver si tiene ese nombre;
	cargar_int_al_super_paquete(un_paquete, una_pcb->ticket);
	cargar_int_al_super_paquete(un_paquete, una_pcb->program_counter);
	cargar_choclo_al_super_paquete(un_paquete, &(una_pcb->registros_CPU->AX), sizeof(uint32_t));
	cargar_choclo_al_super_paquete(un_paquete, &(una_pcb->registros_CPU->BX), sizeof(uint32_t));
	cargar_choclo_al_super_paquete(un_paquete, &(una_pcb->registros_CPU->CX), sizeof(uint32_t));
	cargar_choclo_al_super_paquete(un_paquete, &(una_pcb->registros_CPU->DX), sizeof(uint32_t));
	cargar_int_al_super_paquete(un_paquete, una_pcb->pid); //Verificar los prox 3 nombres

	enviar_paquete(un_paquete, fd_cpu_dispatch);
	eliminar_paquete(un_paquete);
}*/  //Resumen: Ver el nombre de las funciones y ver si son lo mismo :D