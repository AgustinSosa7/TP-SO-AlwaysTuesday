#include "../includes/gestion_paginas.h"

void ocupar_marco(int index){
    bitarray_set_bit(tabla_de_marcos, index);
}

void liberar_marco(int index){
    bitarray_clean_bit(tabla_de_marcos, index);
}

int traer_numero_marco(t_proceso* proceso,int pagina_consultada){
    if((proceso->long_tabla_pags) >= pagina_consultada){
        int numero_marco = proceso->tabla_de_paginas[pagina_consultada];
        return numero_marco;
    }
    else
    {
        log_error(memoria_logger,"Pagina no encontrada. No existe esa pagina en memoria."); //BORRAR
        return -1;
    }
}

int buscar_marco_libre(){
    int marco_encontrado = -1;
    for(int i = 0; i<size_de_tabla_marcos;i++){ 
        log_info(memoria_log_debug, "Buscando en el marco %d... bit: %d",i, (bitarray_test_bit(tabla_de_marcos, i)));
        if(!bitarray_test_bit(tabla_de_marcos, i))
        {
            marco_encontrado = i;
            i = size_de_tabla_marcos + 1;
        } else {
        }
    }
    return marco_encontrado;
}

void cambiar_tamanio_proceso(t_proceso* proceso,int tamanio_nuevo){
    proceso->long_tabla_pags = tamanio_nuevo;
    proceso->tabla_de_paginas = (int *)realloc(proceso->tabla_de_paginas, proceso->long_tabla_pags * sizeof(int));    
}

void asignar_marcos_memoria(t_proceso* proceso){
    int marco;
    for(int i = 0;i < proceso->long_tabla_pags;i++){
        marco = buscar_marco_libre();
        if(marco != -1){
        ocupar_marco(marco);
        proceso->tabla_de_paginas = marco;
        }
        else
        {
            printf("OUT OF MEMORY"); //MANDAR A CPU OUT OF MEMORY.
        }
    }
}

void liberar_marcos_memoria(){

}

int calcular_paginas_necesarias(int tamanio_nuevo){
    int paginas_necesarias = tamanio_nuevo/TAM_PAGINA;
    // Verifica si hay residuo
    if (tamanio_nuevo % TAM_PAGINA != 0) {
        paginas_necesarias += 1;
    }
    return paginas_necesarias;
}

void ajustar_tamanio_proceso(t_proceso* proceso,int tamanio_nuevo){
    //Al llegar una solicitud de ajuste de tamaño de proceso (resize) se deberá cambiar el tamaño del proceso de acuerdo al nuevo tamaño.

    if (tamanio_nuevo > proceso->long_tabla_pags){
        cambiar_tamanio_proceso(proceso,tamanio_nuevo);
        
        //ocupar los marcos a partir del tamanio nuevo y asignarlos en el array.
        asignar_marcos_memoria(proceso);

        //Se deberá ampliar el tamaño del proceso al final del mismo, pudiendo solicitarse múltiples páginas.
        //Es posible que en un punto no se puedan solicitar más marcos ya que la memoria se encuentra llena, 
        //por lo que en ese caso se deberá contestar con un error de Out Of Memory.
    }
    else
    {
        //liberar los marcos a partir del tamanio nuevo.
        //recortar el array
        //Se reducirá el mismo desde el final, liberando, en caso de ser necesario, las páginas que ya no sean utilizadas (desde la última hacia la primera).
        
        cambiar_tamanio_proceso(proceso,tamanio_nuevo);
    }
}