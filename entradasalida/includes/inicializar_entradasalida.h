#ifndef INICIALIZAR_ENTRADASALIDA_H_
#define INICIALIZAR_ENTRADASALIDA_H_

#include "es_gestor.h"

#include <commons/log.h>
#include <commons/config.h>

void inicializar_entradasalida(char *path);
void inicializar_logs();
void inicializar_listas_instrucciones(); 
void inicializar_configs(char *path);
void imprimir_configs();
void recibir_configs(char* TIPO_INTERFAZ);


#endif /* INICIALIZAR_ENTRADASALIDA_H_ */
