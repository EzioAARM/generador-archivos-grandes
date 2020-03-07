#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

FILE *archivoGrande;
pthread_mutex_t lock;
pthread_t threads[10];
double lineasThread;
double startLine = 892;
double startReading = 0;

/*
    Se podria crear un array de 90 posiciones y que haya una posicion para el ascii del caracter y que sea int, asi se ahorra la comparacion
*/

struct args {
    double bytesStart;
};

// Funcion del thread
void *readingThread(void *input) {
    // Obtiene el argumento del struct
    double bytePos = ((struct args*)input)->bytesStart;
    printf("%f", bytePos);
    // Crea un array de 100 posiciones
    char letras[100];
    // lee del archivo 100 posiciones cada conteo
    for (int i = bytePos; i < lineasThread + bytePos; i += 100) {
        // bloquea el archivo para que solo un thread lo lea a la vez
        pthread_mutex_lock(&lock);
            fseek(archivoGrande, bytePos, SEEK_SET);
            fread(&letras, 1, 100, archivoGrande);
        pthread_mutex_unlock(&lock);
        // imprime, aqui debe ir la cuenta
        printf("%s\n", letras);
    }
}

int main() {
    printf("Ingrese la ubicacion de su archivo\n");
    char ubicacion[1000];
    scanf("%s", ubicacion);
    // Abre el archivo en modo read only
    archivoGrande = fopen(ubicacion, "r");
    // Encuentra el peso del archivo para hacer los calculos de las divisiones
    fseek(archivoGrande, 0L, SEEK_END);
    // Obtiene el peso del archivo
    double fileSize = ftell(archivoGrande);
    if ((fileSize / 1e+9) < 1) {
        // peso en mb
        printf("El archivo pesa: %lf mb\n", fileSize / 1e+6);
    } else {
        // peso en gb
        printf("El archivo pesa: %lf gb\n", fileSize / 1e+9);
    }
    // En base al peso calcula la cantidad de lineas que tiene
    lineasThread = ceil((fileSize - startLine) / 400);
    //printf("%f", lineasThread);
    int i = 0;
    // El struct envia el parametro para saber cuantas lineas leera cada thread
    struct args *myStruc;
    double start = startLine;
    startReading = lineasThread;
    while (i < 10) {
        // Crea el struct con la cantidad de lineas
        myStruc = (struct args *)malloc(sizeof(struct args));
        myStruc->bytesStart = start;
        // Crea el thread en el arreglo de threads y envia el struct
        pthread_create(&threads[i], NULL, readingThread, myStruc);
        start = start + lineasThread;
        i++;
    }
    printf("\n\n");
    // Cierra el archivo *esto hay que cambiarlo, debe esperar a que terminen los threads para cerrarlo
    fclose(archivoGrande);
    return 0;
}