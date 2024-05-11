#include "../includes/pcb.h"


void _enviar_pcb_a_CPU_por_dispatch(t_pcb* un_pcb){
	t_paquete* un_paquete = crear_paquete(EJECUTAR_PROCESO_KC); //Ejecutar, ver si tiene ese nombre;
	agregar_algo_a_paquete(un_paquete, un_pcb->pid);
    agregar_algo_a_paquete(un_paquete, un_pcb->program_counter);
    agregar_algo_a_paquete(un_paquete, un_pcb->QUANTUM);
    agregar_registro_a_paquete(un_paquete, un_pcb->registros_CPU);
	enviar_paquete(un_paquete, fd_cpu_dispatch);
	eliminar_paquete(un_paquete);
} 


