#include <stdlib.h>
#include <stdio.h>
#include <../includes/kernel.h>

#include <../src/utils/hello.h>
#include <../src/utils/utils_connections.h>


int main() {
// INICIAR KERNEL

inicializar_kernel();


printf("Puerto escucha %s\n",PUERTO_ESCUCHA);


fd_kernel = iniciar_servidor(PUERTO_ESCUCHA, kernel_logger, "KERNEL INICIADO!");


decir_hola("carlos");

return EXIT_SUCCESS;
}
