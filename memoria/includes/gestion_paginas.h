#ifndef GESTION_PAGINAS_H_
#define GESTION_PAGINAS_H_

#include "m_gestor.h"

void ocupar_marco(int);
void liberar_marco(int);
int traer_numero_marco(t_proceso*,int);
int buscar_marco_libre();
void cambiar_tamanio_proceso(t_proceso*,int);
void asignar_marcos_memoria(t_proceso*);
void liberar_marcos_memoria();
void ajustar_tamanio_proceso(t_proceso*,int);


#endif