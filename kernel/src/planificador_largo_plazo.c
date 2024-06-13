#include "../includes/planificador_largo_plazo.h"



void planif_largo_plazo(){
   while(1){
      printf("plani largo plazo");
      detener_planificacion();
      printf("paso funcion detener plani");
      sem_wait(&sem_new_a_ready);
 	   sem_wait(&sem_grado_multiprogram);
      printf("paso semaforos");
      if(!list_is_empty(lista_new)){
      cambiar_de_estado_y_de_lista(NEW, READY);
      printf("cambio de lista");
      sem_post(&sem_planificador_corto_plazo);
         }
      }
   }
