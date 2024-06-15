#include "../includes/planificador_largo_plazo.h"



void planif_largo_plazo(){
   while(1){
      printf("Plani largo plazo.\n");
      detener_planificacion();
      printf("Paso funcion detener plani.\n");
      sem_wait(&sem_new_a_ready);
 	   sem_wait(&sem_grado_multiprogram);
      printf("Paso semaforos.\n");
      if(!list_is_empty(lista_new)){
      cambiar_de_estado_y_de_lista(NEW, READY);
      printf("Cambio de lista.\n");
      sem_post(&sem_planificador_corto_plazo);
         }
      }
   }
