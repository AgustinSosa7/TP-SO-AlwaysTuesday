#ifndef INICIALIZAR_ENTRADASALIDA_H_
#define INICIALIZAR_ENTRADASALIDA_H_

#include "es_gestor.h"

t_list* INSTRUCCIONES_GEN;
t_list* INSTRUCCIONES_STDIN;
t_list* INSTRUCCIONES_STDOUT;
t_list* INSTRUCCIONES_FS;

void inicializar_entradasalida();
void inicializar_logs();
void inicializar_listas_instrucciones();
void inicializar_configs();
void imprimir_configs();
void recibir_configs(char* TIPO_INTERFAZ);


#endif /* INICIALIZAR_ENTRADASALIDA_H_ */
