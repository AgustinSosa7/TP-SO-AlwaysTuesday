#ifndef MEMORIA_PROCESOS_H
#define MEMORIA_PROCESOS_H


#include "m_gestor.h"

t_list* leer_archivo_pseudocodigo(char*);
t_proceso* crear_proceso_nuevo();
void enviar_instruccion_pesudocodigo(t_list* lista_proceso,int pc); 
void recibir_instruccion_a_enviar(int pid,int pc);
t_proceso* buscar_proceso_en_memoria(int process_id);


#endif /*INICIALIZAR_MEMORIA_H_*/