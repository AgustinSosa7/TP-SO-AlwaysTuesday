#ifndef MEMORIA_PROCESOS_H
#define MEMORIA_PROCESOS_H


#include "m_gestor.h"

t_list* leer_archivo_pseudocodigo(char*);
char* recibir_direccion_pseudocodigo(); //CODIGO REPETIDO EN CPU_MEMORIA PASAR A SHARED
void enviar_instruccion_pesudocodigo(t_list* lista_proceso,int pc); 


#endif /*INICIALIZAR_MEMORIA_H_*/