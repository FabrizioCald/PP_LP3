/*programa1_lp3.c*/
/*Nombre y Apellido: Fabrizio Calderoli*/
/*Materia: Lenguaje de programacion 3*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main() {
    FILE *archivo;
    char linea[100];
    int procesoId, senal, segundosDelay;

    archivo = fopen("archivo_lp3.txt", "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }

    while (fgets(linea, sizeof(linea), archivo)) {
        sscanf(linea, "%d %d %d", &procesoId, &senal, &segundosDelay);
        sleep(segundosDelay);
        kill(procesoId, senal);
        printf("Señal numero: %d Enviada al proceso: %d Luego de: %d segundos\n", senal, procesoId, segundosDelay);
    }

    fclose(archivo);
    return 0;
}