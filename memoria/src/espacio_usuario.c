#include "../includes/espacio_usuario.h"

void* leer_espacio_usuario(int direccion_fisica,int cuanto_leer){
    void* espacio_leido = malloc(cuanto_leer);

    memcpy(espacio_leido, espacio_memoria + direccion_fisica , cuanto_leer);

    return espacio_leido;
}

bool escribir_espacio_usuario(int direccion_fisica,int cuanto_escribir,void* a_escribir){

    memcpy(espacio_memoria + direccion_fisica, &a_escribir, cuanto_escribir);

    // Compara los bloques de memoria
    if (memcmp(a_escribir, (espacio_memoria + direccion_fisica), cuanto_escribir) == 0){
        return true;
    } else 
    {
        return false;
    }
}