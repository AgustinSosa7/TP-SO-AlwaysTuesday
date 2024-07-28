#include "../includes/planificador_largo_plazo.h"



void planif_largo_plazo(){
   while(1){
      printf("Entro al plani largo plazo.\n");

      printf("Pase funcion detener plani.\n");
      printf("WAIT del plani LARGO plazo.\n");
      sem_wait(&sem_new_a_ready);
      detener_plani_largo_plazo();
 	   sem_wait(&sem_grado_multiprogram);
      printf("Pase los semaforos del plani LARGO plazo.\n");
      if(!list_is_empty(struct_new->lista)){
      cambiar_de_estado_y_de_lista(NEW, READY);
      printf("Cambio de lista.\n");
      printf("SIGNAL del plani CORTO plazo.\n");
      sem_post(&sem_planificador_corto_plazo);
         }
      }
   }

