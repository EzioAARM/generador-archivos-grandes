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

struct args {
    double bytesStart;
};

void *readingThread(void *input) {
    double bytePos = ((struct args*)input)->bytesStart;
    printf("%f", bytePos);
    char letras[100];
    for (int i = bytePos; i < lineasThread + bytePos; i += 100) {
        pthread_mutex_lock(&lock);
            fseek(archivoGrande, bytePos, SEEK_SET);
            fread(&letras, 1, 100, archivoGrande);
        pthread_mutex_unlock(&lock);
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
    double fileSize = ftell(archivoGrande);
    if ((fileSize / 1e+9) < 1) {
        printf("El archivo pesa: %lf mb\n", fileSize / 1e+6);
    } else {
        printf("El archivo pesa: %lf gb\n", fileSize / 1e+9);
    }
    lineasThread = ceil((fileSize - startLine) / 400);
    //printf("%f", lineasThread);
    int i = 0;
    struct args *myStruc;
    double start = startLine;
    startReading = lineasThread;
    while (i < 10) {
        myStruc = (struct args *)malloc(sizeof(struct args));
        myStruc->bytesStart = start;
        pthread_create(&threads[i], NULL, readingThread, myStruc);
        start = start + lineasThread;
        i++;
    }


    /*fseek(archivoGrande, 2, SEEK_SET);
    int letra;
    fread(&letra, 1, 1, archivoGrande);
    fileSize = fileSize - startLine;
    printf("%c", letra);*/

    printf("\n\n");
    fclose(archivoGrande);
    return 0;
}