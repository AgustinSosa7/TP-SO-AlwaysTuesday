#include "../includes/consola.h"

bool validar_instruccion(char* leido);
char* atender_instruccion_validada(char* leido);
void leer_comandos();
void iniciar_consola(){

};

void leer_consola(){
    pthread_t hilo_consola;
    pthread_create(&hilo_consola, NULL, (void*) leer_comandos, NULL);
	pthread_detach(hilo_consola);

};


void leer_comandos(){

    char* leido;
	leido = readline("> ");
	while(strcmp(leido,"\0") != 0){
		if(validar_instruccion(leido)){
			printf("Comando válido\n");
			atender_instruccion_validada(leido);
		}

		free(leido);
		leido = readline("> ");
	}
	free(leido);
}

bool validar_instruccion(char* leido){
    return true;
}
char* atender_instruccion_validada(char* leido){
    char* nom= "hola";
    return nom;
}