#ifndef ENTRADASALIDA_KERNEL_H_
#define ENTRADASALIDA_KERNEL_H

#include "es_gestor.h"
#include "entradasalida_memoria.h"



void identificarse_con_kernel(int socket, char* nombre_io);

void atender_entradasalida_kernel();

t_peticion* recibir_peticion(t_paquete* paquete);
void asignar_parametros_segun_tipo(t_peticion* peticion, t_buffer* buffer);
void procesar_peticion(t_peticion* peticion);
char* iniciar_la_consola(int registroTamanio);
bool validar_tamanio_leido(char* leido, int registroTamanio);

void finalizar_peticion(t_peticion* peticion);
void eliminar_peticion(t_peticion* peticion);
void eliminar_parametros_segun_instruccion(char* instruccion, t_peticion_param* parametros);

//FS
bool crear_config(char* nombre_archivo);
bool delete_archivo(char* nombre_archivo);
bool truncar_archivo(char* nombre_archivo,int tamanio_nuevo, int pid);
bool escribir_archivo(char* nombre_archivo,int registro_archivo,char* escrito,int tamanio_text);
char* leer_archivo(char* nombre_archivo,int registro_archivo,int tamanio);
int obtener_bloques_libres(int tamanio);
bool puede_crecer(int final_bloques_nuevo,int final_bloques_viejo);
void setear_bitmap(int comienzo, int final,bool asigna);
bool hay_espacios(int bloques_faltantes);
int compactacion(int bloque_inicial,int final_bloques_viejo, int final_bloques_nuevo,int pid);
int mover_archivos();
int tamanio_bloque_escrito(int tamanio_archivo);
int copiar_archivo(int primer_bloque_libre, int primer_bloque_ocupado, int ultimo_bloque_ocupado);
char* generar_path_config(char* nombre_archivo);
void mostrar_estado_archivo(t_config* config_archivo);
void modificar_config (int primer_bloque_ocupado, int ultimo_bloque_ocupado,int primer_bloque_libre);
int inicializar_lista_archivos();
int ignorar(const char *nombre_archivo);
#endif 