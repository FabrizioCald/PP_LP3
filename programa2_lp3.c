/*programa2_lp3.c*/
/*Nombre y Apellido: Fabrizio Calderoli*/
/*Materia: Lenguaje de programacion 3*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void handler(int signum) {
    printf("Señal recibida, numero: %d\n", signum);
}

int main() {

    for (int i = 1; i < 65; ++i)
    {
    	signal(i,handler);
    }
    printf("Mi PID: %d\n", getpid());
    // Mantener el programa en ejecución
    while (1) {
        // Esperar a que llegue una señal
        printf("Esperando señal...\n");
        sleep(10);
    }

    return 0;
}