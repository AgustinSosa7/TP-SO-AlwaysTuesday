#ifndef MEMORIA_PROCESOS_H
#define MEMORIA_PROCESOS_H


#include "m_gestor.h"

t_list* leer_archivo_pseudocodigo(char*);
void enviar_instruccion_pesudocodigo(t_list* lista_proceso,int pc); 

t_proceso* buscar_proceso_en_memoria(int process_id);


#endif /*INICIALIZAR_MEMORIA_H_*/
