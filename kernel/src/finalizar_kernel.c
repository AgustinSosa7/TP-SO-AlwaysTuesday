#include "../includes/finalizar_kernel.h"


void finalizar_kernel(){
    eliminar_recursos();
    eliminar_listas();
}
void eliminar_listas(){
    list_destroy_and_destroy_elements(IOS_CONECTADOS,(void*)eliminar_ios_conectados);
    eliminar_lista_instrucciones_posibles();
    eliminar_pcbs_y_listas();

}
void eliminar_pcbs_y_listas(){
    list_destroy_and_destroy_elements(struct_new->lista,(void*)eliminar_pcb);
    free(struct_new);
    list_destroy_and_destroy_elements(struct_ready->lista,(void*)eliminar_pcb);
    free(struct_ready);
    list_destroy_and_destroy_elements(struct_ready_plus->lista,(void*)eliminar_pcb);
    free(struct_ready_plus);
    list_destroy_and_destroy_elements(struct_exec->lista,(void*)eliminar_pcb);
    free(struct_exec);
    list_destroy_and_destroy_elements(struct_exit->lista,(void*)eliminar_pcb);
    free(struct_exit);
    
}

void eliminar_recursos(){
    list_clean_and_destroy_elements(lista_recursos,(void*)eliminar_recurso);
}

void eliminar_recurso(t_recursos* recurso){
    free(recurso->nombre_recurso);
    list_destroy_and_destroy_elements(recurso->lista_procesos_asignados,(void*)eliminar_pcb);
    list_destroy_and_destroy_elements(recurso->lista_procesos_bloqueados,(void*)eliminar_pcb);
}

void eliminar_lista_instrucciones_posibles(){
    list_destroy(INSTRUCCIONES_GENERICA);
    list_destroy(INSTRUCCIONES_STDIN);
    list_destroy(INSTRUCCIONES_STDOUT);
    list_destroy(INSTRUCCIONES_FS);
}

void eliminar_ios_conectados(t_interfaz* interfaz){
    enviar_cola_de_procesos_blocked_io_a_exit(interfaz);
    eliminar_interfaz(interfaz);
}