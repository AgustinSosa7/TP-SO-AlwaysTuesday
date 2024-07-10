#include "../includes/espacio_usuario.h"

void* leer_espacio_usuario(int direccion_fisica,int cuanto_leer){
    void* espacio_leido = malloc(cuanto_leer);
    pthread_mutex_lock(&mutex_espacio_memoria);
    memcpy(espacio_leido, espacio_memoria + direccion_fisica , cuanto_leer);
    pthread_mutex_unlock(&mutex_espacio_memoria);
    return espacio_leido;
}


void escribir_espacio_usuario(int direccion_fisica,int cuanto_escribir,void* a_escribir){
    pthread_mutex_lock(&mutex_espacio_memoria);
    memcpy(espacio_memoria + direccion_fisica, a_escribir, cuanto_escribir);
    pthread_mutex_unlock(&mutex_espacio_memoria);
}