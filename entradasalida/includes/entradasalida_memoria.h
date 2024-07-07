#ifndef ENTRADASALIDA_MEMORIA_H_
#define ENTRADASALIDA_MEMORIA_H

#include "es_gestor.h"

void guardar_en_memoria(char* leido, int registroDireccion, int registroTamanio);
void partir_y_guardar_en_memoria(char* leido, t_list* lista_de_accesos);
char* pedir_a_memoria(int registroDireccion, int registroTamanio);
char*pedir_a_memoria_y_unir(t_list* lista_de_accesos);
int tamanio_total_del_leido(t_list* lista_de_accesos);

#endif 