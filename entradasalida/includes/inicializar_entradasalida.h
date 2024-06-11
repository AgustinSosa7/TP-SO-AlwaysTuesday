#ifndef INICIALIZAR_ENTRADASALIDA_H_
#define INICIALIZAR_ENTRADASALIDA_H_

#include "es_gestor.h"

#include <commons/log.h>
#include <commons/config.h>

void inicializar_entradasalida(char** argv);
void inicializar_logs();
void inicializar_configs(char** argv);
void imprimir_configs();
void recibir_configs(char* TIPO_INTERFAZ);

void iniciar_file_system();
void inicializar_archivo_de_bloques();


#endif /* INICIALIZAR_ENTRADASALIDA_H_ */
