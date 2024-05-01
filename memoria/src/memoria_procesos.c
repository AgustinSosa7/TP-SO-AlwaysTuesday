#include "../includes/memoria_procesos.h"




void leer_archivo_pseudocodigo(char* nombre_Archivo,t_list* lista_de_instrucciones){

    char* PATH = string_new();
    char INSTRUCCION[100];

    string_append(&PATH, PATH_INSTRUCCIONES);
	string_append(&PATH, "/");
	string_append(&PATH, nombre_Archivo);

    FILE *Pseudocodigo = fopen(PATH, "r");

    if (Pseudocodigo == NULL){
        perror("Error al intentar cargar el archivo de Pseudocodigo.");
        exit(EXIT_FAILURE);
    }
    
    char* hola;

    while(!(string_equals_ignore_case(INSTRUCCION, "EXIT"))){ 
        fgets(INSTRUCCION, sizeof(INSTRUCCION) ,Pseudocodigo);
        printf("%s",INSTRUCCION);
        list_add(lista_de_instrucciones, INSTRUCCION);

        hola = list_get(lista_de_instrucciones,0);
        printf("%s",hola);
        
    }

};
