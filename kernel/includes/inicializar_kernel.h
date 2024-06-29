#ifndef INICIALIZAR_KERNEL_H_
#define INICIALIZAR_KERNEL_H_

#include "k_gestor.h"
//#include "crear_conexiones.h"

void inicializar_logs(void);
void inicializar_configs(char* path);
void inicializar_listas(void);
void inicializar_structs_listas();
void inicializar_listas_instrucciones();
t_list* asignar_instrucciones_posibles(char* tipo);
void inicializar_kernel(char* path);
void inicializar_semaforos();
void inicializar_pid();
void inicializar_recursos();
void imprimir_recursos(t_recursos *recurso);
void imprimir_lista_recursos(t_list* lista_a_mostrar);

#endif 