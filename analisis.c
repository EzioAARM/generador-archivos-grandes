#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

FILE *archivoGrande;
pthread_mutex_t lock;
pthread_mutex_t lockVar;
int cantThreads = 3;
pthread_t threads[3];
bool terminados[3];
double bytesToRead;
double startLine = 891;
double startReading = 0;
int conteo[100];

struct args {
    double bytesStart;
    double byteEnd;
    int pos;
};

void *readingThread(void *input) {
    double bytePos = ((struct args*)input)->bytesStart;
    double bytePosEnd = ((struct args*)input)->byteEnd;
    int threadPos = ((struct args*)input)->pos;
    char letras[400];
    char c;
    int conteoInterno[100];
    int lastRead = 400;
    for (int i = bytePos; i <= bytePosEnd; i += 400) {
        lastRead = ((i + 400) > bytePosEnd) ? (bytePosEnd - i) : 400;
        pthread_mutex_lock(&lock);
            fseek(archivoGrande, i, SEEK_SET);
            fread(&letras, 1, lastRead, archivoGrande);
        pthread_mutex_unlock(&lock);
        // imprime, aqui debe ir la cuenta
        for (int i = 0; i <= lastRead; i++) {
            conteoInterno[(int) ((char) letras[i])]++;
        }
        //if (lastRead != 400) printf("%i", lastRead);
        //printf("thread %i: %i\n", ((struct args*)input)->pos, i);
    }
    pthread_mutex_lock(&lockVar);
        for (int i = 0; i <= 100; i++) {
            conteo[i] += conteoInterno[i];
        }
        terminados[threadPos] = true;
        bool finished = true;
        for (int i = 0; i < cantThreads; i++) {
            //printf("%d\n", terminados[i]);
            if (!terminados[i]) finished = false;
        }
        if (finished) {
            fclose(archivoGrande);
            printf("Terminaron todos los threads\n");
        }
    pthread_mutex_unlock(&lockVar);
    //printf("\nEl thread %i termino \n", threadPos);
}

int main() {
    for (int i = 0; i < cantThreads; i++) {
        terminados[i] = false;
    }
    printf("Ingrese la ubicacion de su archivo\n");
    char ubicacion[1000];
    scanf("%s", ubicacion);
    system("clear");
    archivoGrande = fopen(ubicacion, "r");
    fseek(archivoGrande, 0L, SEEK_END);
    // Obtiene el peso del archivo
    double fileSize = ftell(archivoGrande);
    if ((fileSize / 1e+9) < 1) {
        printf("El archivo pesa: %lf mb (%lf bytes)\n", fileSize / 1e+6, fileSize);
    } else {
        printf("El archivo pesa: %lf gb (%lf bytes)\n", fileSize / 1e+9, fileSize);
    }
    bytesToRead = fileSize;
    //printf("bytes: %f\n", bytesToRead);
    int i = 0;
    struct args *myStruc;
    double start = startLine;
    startReading = bytesToRead;
    double bytesThread = ceil(bytesToRead / cantThreads);
    double end = start + bytesThread;
    //printf("%lf\n", bytesThread);
    while (i < cantThreads) {
        myStruc = (struct args *)malloc(sizeof(struct args));
        myStruc->bytesStart = start;
        myStruc->byteEnd = end;
        myStruc->pos = i;
        pthread_create(&threads[i], NULL, readingThread, myStruc);
        //printf("El thread %i leera %lf bytes hasta %lf\n", i, start, end);
        start = end + 1;
        end = start + bytesThread;
        if (end >= bytesToRead) {
            end = bytesToRead;
        }
        i++;
    }
    bool terminoThreads = false;
    char cosa[100] = "";
    while (!terminoThreads) {
        system("clear");
        terminoThreads = true;
        for (int i = 0; i < cantThreads; i++) {
            if (!terminados[i]) terminoThreads = false;
        }
        printf("%s\n", terminoThreads ? "Ya termine!" : "Aun no termino :c");
        if (!terminoThreads) usleep(30e6);
    }
    for (int i = 65; i <= 90; i++) {
        printf("%c -> %i\n", (char) i, conteo[i]);
    }
    return 0;
}