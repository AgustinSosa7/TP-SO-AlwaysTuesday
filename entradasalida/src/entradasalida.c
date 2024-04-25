#include "../includes/entradasalida.h"

int main() {

inicializar_entradasalida();

//Conectarme como cliente a Memoria
gestionar_conexion_como_cliente(IP_MEMORIA, PUERTO_MEMORIA, entradasalida_logger, "MEMORIA");

//Conectarme como cliente a Kernel
gestionar_conexion_como_cliente(IP_KERNEL, PUERTO_KERNEL, entradasalida_logger, "KERNEL");

return EXIT_SUCCESS;
}

//Interfaz Genérica

//void procesar_peticion(char *peticion, parametros) {
//
//    switch (peticion) {
//      case IO_GEN_SLEEP: { int tiempo_espera = TIEMPO_UNIDAD_TRABAJO * parametros.UnidadesDeTrabajo;
//                           sleep(tiempo_espera);
//                           break;}
//      case IO_STDIN_READ :
//            // Lógica para manejar operaciones para STDIN
//            break;
//      case IO_STDOUT_WRITE :
//            // Lógica para manejar operaciones para STDOUT
//            break;
//      case IO_FS_CREATE :
//            // Lógica para manejar operaciones para DialFS
//            break;
//      case IO_FS_DELETE :
//            // Lógica para manejar operaciones para DialFS
//            break;
//      case IO_FS_TRUNCATE :
//            // Lógica para manejar operaciones para DialFS
//            break;   
//      case IO_FS_WRITE :
//            // Lógica para manejar operaciones para DialFS
//            break;
//      case IO_FS_READ :
//            // Lógica para manejar operaciones para DialFS
//            break;                                                         
//      default:
//            //log_error(logger,"Interfaz %s: Tipo de interfaz no soportado. Proceso enviado a EXIT.\n", NOMBRE_INTERFAZ);
//            exit(EXIT_FAILURE);
//    }
//}
 
 

	




