#include "../includes/gestion_paginas.h"

void ocupar_marco(int index){
    pthread_mutex_lock(&mutex_tabla_marcos);
    bitarray_set_bit(tabla_de_marcos, index);
    pthread_mutex_unlock(&mutex_tabla_marcos);
    
}

void liberar_marco(int index){
    pthread_mutex_lock(&mutex_tabla_marcos);
    bitarray_clean_bit(tabla_de_marcos, index);
    pthread_mutex_unlock(&mutex_tabla_marcos);
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

    
    pthread_mutex_lock(&mutex_tabla_marcos);
    int marco_encontrado = -1;
    //log_info(memoria_log_debug, "Busco marco libre...");
    for(int i = 0; i<size_de_tabla_marcos;i++){ 
        if(!bitarray_test_bit(tabla_de_marcos, i))
        {
            marco_encontrado = i;
            i = size_de_tabla_marcos + 1;
        } else 
        {
        }
    }
    log_info(memoria_log_debug, "Encontrado el marco %d... bit: %d",marco_encontrado, (bitarray_test_bit(tabla_de_marcos, marco_encontrado)));
    pthread_mutex_unlock(&mutex_tabla_marcos);

    return marco_encontrado;
}

void cambiar_tamanio_proceso(t_proceso* proceso,int tamanio_nuevo){
    if(proceso->long_tabla_pags == 0){
        
        int* aux = proceso->tabla_de_paginas;
        proceso->tabla_de_paginas = (int *)malloc(tamanio_nuevo * sizeof(int));   
        free(aux);

        if (proceso->tabla_de_paginas == NULL) {
        log_error(memoria_log_debug, "Error al asignar memoria\n");
        }
    }
    else
    {
        proceso->tabla_de_paginas = (int *)realloc(proceso->tabla_de_paginas, tamanio_nuevo * sizeof(int));    

        if (proceso->tabla_de_paginas == NULL) {
        log_error(memoria_log_debug, "Error al asignar memoria\n");
        }
    }
}

void cambiar_variable_long_tabla_pags(t_proceso* proceso,int tamanio_nuevo){
    proceso->long_tabla_pags = tamanio_nuevo;
}

void asignar_marcos_memoria(t_proceso* proceso,int tamanio_nuevo){
    int marco;
    for(int i = proceso->long_tabla_pags;i < tamanio_nuevo;i++){
        marco = buscar_marco_libre();
        ocupar_marco(marco);
        proceso->tabla_de_paginas[i] = marco;
    }
    cambiar_variable_long_tabla_pags(proceso,tamanio_nuevo);
}

void liberar_marcos_memoria(t_proceso* proceso,int tamanio_nuevo){
    int numero_marco;
    for(int i = (tamanio_nuevo); i < proceso->long_tabla_pags; i++){
        numero_marco = traer_numero_marco(proceso,i);
        //log_info(memoria_log_debug, "El marco: %d de la pagina: %d va a ser liberado", numero_marco,i);
        liberar_marco(numero_marco);
    }
    cambiar_variable_long_tabla_pags(proceso,tamanio_nuevo);
}

int calcular_paginas_necesarias(int tamanio_nuevo){
    int paginas_necesarias = tamanio_nuevo/TAM_PAGINA;
    // Verifica si hay residuo
    if (tamanio_nuevo % TAM_PAGINA != 0) {
        paginas_necesarias += 1;
    }
    return paginas_necesarias;
}

int calcular_marcos_libres(){

    pthread_mutex_lock(&mutex_tabla_marcos);
    int marcos_libres = 0;
    for(int i = 0;i<size_de_tabla_marcos;i++){
        if(!bitarray_test_bit(tabla_de_marcos, i)){
            marcos_libres++;
        }
    }
    pthread_mutex_unlock(&mutex_tabla_marcos);

    log_info(memoria_log_debug, "Marcos libres calculados: %d\n",marcos_libres);
    return marcos_libres;
}

int ajustar_tamanio_proceso(t_proceso* proceso,int tamanio_nuevo){
    //Al llegar una solicitud de ajuste de tamaño de proceso (resize) se deberá cambiar el tamaño del proceso de acuerdo al nuevo tamaño.

    if (tamanio_nuevo > proceso->long_tabla_pags){
        
        //Log obligatorio. (Creación / destrucción de Tabla de Páginas:)
        log_info(memoria_logger, "PID: %d - Tamaño Actual: %d - Tamaño a Ampliar: %d",proceso->pid,proceso->long_tabla_pags,tamanio_nuevo); 

        if((tamanio_nuevo-(proceso->long_tabla_pags)) > calcular_marcos_libres())//Lo que yo quiero pedir es mayor a lo libre
        { 
            return (-1);//OUT_OF_MEMORY
        }
        else
        {

            pthread_mutex_lock(&mutex_tabla_paginas);
            cambiar_tamanio_proceso(proceso,tamanio_nuevo);
            asignar_marcos_memoria(proceso,tamanio_nuevo); 
            pthread_mutex_unlock(&mutex_tabla_paginas);

        return 0;
        }
    }
    else
    {
        //Log obligatorio. (Creación / destrucción de Tabla de Páginas:)
        log_info(memoria_logger, "PID: %d - Tamaño Actual: %d - Tamaño a Reducir: %d",proceso->pid,proceso->long_tabla_pags,tamanio_nuevo); 

        pthread_mutex_lock(&mutex_tabla_paginas);
        liberar_marcos_memoria(proceso,tamanio_nuevo);
        cambiar_tamanio_proceso(proceso,tamanio_nuevo);
        pthread_mutex_unlock(&mutex_tabla_paginas);

        return 0;
    }
   
}