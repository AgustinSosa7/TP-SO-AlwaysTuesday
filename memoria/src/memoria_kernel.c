#include "../includes/memoria_kernel.h"

void atender_kernel(){
        //int i = 0;//SACAR
    while(1){
        //printf("Atender kernel. Vez: %d\n",i);
        op_code code_op_recibido = recibir_operacion(fd_kernel);
        if(code_op_recibido == CREAR_PROCESO){
            list_add(procesos_memoria, crear_proceso_nuevo());
            /* PRUBEAS
            t_proceso* proceso = malloc(sizeof(proceso));
            proceso = buscar_proceso_en_memoria(i+1);
            log_info(memoria_logger, "Cree el proceso: %d",proceso->pid);
            i = i + 1;
            free(proceso);
            // PRUEBAS */
        }
        else if(code_op_recibido == FINALIZAR_PROCESO_MEMORIA){
            //finalizar_proceso(proceso); ESto debe abuscar un proceso en memoria. Marcar libres todos los marcos del proceso, y luego de esto hacerle free.
            log_info(memoria_logger, "Se pidio finalizar un proceso");
        }
        else{
            log_error(memoria_logger, "No se recibio un pedido correcto de kernel.");
            exit(EXIT_FAILURE);
        }
    }    



    


}