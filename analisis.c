#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

FILE *archivoGrande;
pthread_mutex_t lock;
pthread_mutex_t lockVar;
int cantThreads = 4;
pthread_t threads[4];
bool terminados[4];
double bytesToRead;
double startLine = 892;
double startReading = 0;
double conteo[255];

struct args {
    double bytesStart;
    double byteEnd;
    int pos;
};

void *readingThread(void *input) {
    /*Obtiene los argumentos del Struct*/
    double bytePos = ((struct args*)input)->bytesStart;
    double bytePosEnd = ((struct args*)input)->byteEnd;
    int threadPos = ((struct args*)input)->pos;
    char letras[400];
    char c;
    double conteoInterno[255];
    int lastRead = 400;
    int otroCont = 0;
    /*Recorre el archivo dentro de los limites establecidos*/
    for (double i = bytePos; i <= bytePosEnd; i += 400) {
        /*Calcula cuantos caracteres tiene que leer por iteración*/
        lastRead = ((i + 400) > bytePosEnd) ? (bytePosEnd - i) : 400;
        /*Bloquea el archivo para que solo el pueda acceder a el en el instante*/
        pthread_mutex_lock(&lock);
            /*Busca en el archivo, fseeko es para buscar en archivos grandes*/
            fseeko(archivoGrande, i, SEEK_SET);
            fread(&letras, 1, lastRead, archivoGrande);
        pthread_mutex_unlock(&lock);
        /*Recorre los caracteres leidos y suma en la posición correspondiente del array*/
        for (int i = 0; i <= lastRead; i++) {
            conteoInterno[(int) ((char) letras[i])]++;
        }
        switch (otroCont)
        {
        case 250000:
            system("clear");
            pthread_mutex_lock(&lockVar);
                /*Suma al contador global*/
                for (int i = 65; i <= 90; i++) {
                    conteo[i] += conteoInterno[i];
                    printf("%c -> %lf\n", (char) i, conteo[i]);
                    conteoInterno[i] = 0;
                }
            pthread_mutex_unlock(&lockVar);
            break;
        }
        otroCont++;
    }
    /*Asigna al contador global y verifica si todos los threads terminaron*/
    pthread_mutex_lock(&lockVar);
        /*Suma al contador global*/
        for (int i = 65; i <= 90; i++) {
            conteo[i] += conteoInterno[i];
        }
        /*Asigna terminado a una posición correspondiente al thread*/
        terminados[threadPos] = true;
        bool finished = true;
        /*Verifica si todos los threads terminaron*/
        for (int i = 0; i < cantThreads; i++) {
            if (!terminados[i]) finished = false;
        }
        /*Si terminaron cierra el archivo, si no ignora eso*/
        if (finished) {
            fclose(archivoGrande);
        }
        /*Limpia la consola y muestra la cuenta de las letras*/
        system("clear");
        for (int i = 65; i <= 90; i++) {
            printf("%c -> %lf\n", (char) i, conteo[i]);
        }
    pthread_mutex_unlock(&lockVar);
}

int main() {
    /*Asignaciones iniciales*/
    for (int i = 0; i < cantThreads; i++) {
        terminados[i] = false;
    }

    terminados[cantThreads] = true;
    /*Lee la ubicación del archivo y luego limpia la consola*/
    printf("Ingrese la ubicacion de su archivo\n");
    char ubicacion[1000];
    scanf("%s", ubicacion);
    system("clear");
    /*Abre el archivo*/
    archivoGrande = fopen(ubicacion, "r");
    /*Busca la ultima posición del archivo*/
    fseeko(archivoGrande, 0LL, SEEK_END);
    /*Obtiene el peso del archivo en bytes*/
    double fileSize = ftello(archivoGrande);
    /*Muestra el tamaño del archivo*/
    if ((fileSize / 1e+9) < 1) {
        printf("El archivo pesa: %lf mb (%lf bytes)\n", fileSize / 1e+6, fileSize);
    } else {
        printf("El archivo pesa: %lf gb (%lf bytes)\n", fileSize / 1e+9, fileSize);
    }
    char buff[100];
    time_t now = time(0);
    struct tm inicialDate = *localtime(&now);
    /*Obtiene la cantidad de caracteres antes del primer salto de linea, estos corresponden al encabezado*/
    char letrasHead[1];
    for (int j = 0; j < startLine; j++) {
        fseeko(archivoGrande, j, SEEK_SET);
        fread(&letrasHead, 1, 1, archivoGrande);
        int ascii = (int)((char)letrasHead[0]);
        switch (ascii)
        {
            case 10:
                startLine = j;
                j = fileSize + 10;
                break;
            default: break;
        }
    }
    startLine = startLine - 1;
    /*Calcula cuantos bytes leera cada cada thread en base a la variable cantThreads*/
    bytesToRead = fileSize;
    int i = 0;
    struct args *myStruc;
    double start = startLine;
    startReading = bytesToRead;
    /*Para que no hayan decimales ceil aproxima al entero arriba*/
    double bytesThread = ceil(bytesToRead / (cantThreads));
    double end = start + bytesThread;

    /*Crea los threads*/
    while (i < cantThreads) {
        /*Crea el struct con los datos correspondientes a cada thread*/
        myStruc = (struct args *)malloc(sizeof(struct args));
        myStruc->bytesStart = start;
        myStruc->byteEnd = end;
        myStruc->pos = i;
        /*Crea el thread*/
        pthread_create(&threads[i], NULL, readingThread, myStruc);
        /*Pone el nuevo inicio y el nuevo fin para el siguiente thread*/
        start = end + 1;
        end = start + bytesThread;
        /*Si sobrepasa el tamaño del archivo asigna el tamaño maximo del archivo*/
        if (end >= bytesToRead) {
            end = bytesToRead;
        }
        i++;
    }
    /*Espera a que los threads terminen*/
    for (int j = 0; j < cantThreads; j++) {
        pthread_join(threads[j], NULL);
    }
    /*Obtiene la ubicación del proyecto actual*/
    char cwd[1000];
    getcwd(cwd, sizeof(cwd));
    strcat(cwd, "/output.csv");
    archivoGrande = fopen(cwd, "w");
    char numStr[32];
    char cadenaEscribir[100] = "";
    char letra;
    for (int i = 65; i <= 90; i++) {
        letra = (char) i;
        fputc(letra, archivoGrande);
        fputc((char) 44, archivoGrande);
        fprintf(archivoGrande, "%f", conteo[i]);
        fputc((char) 10, archivoGrande);
    }
    fclose(archivoGrande);
    char buff2[100];
    time_t now2 = time(0);
    struct tm finalDate = *localtime(&now2);
    printf("Su archivo de salida se genero en: %s\n", cwd);
    int minutos = finalDate.tm_min - inicialDate.tm_min;
    int segundos = finalDate.tm_sec - inicialDate.tm_sec;
    printf("La ejecucion duro %i minutos y %i segundos", minutos, segundos);
    return 0;
}
