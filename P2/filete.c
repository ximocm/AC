#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h> // Incluir la directiva de OpenMP

typedef enum { false, true } bool;  // Definición de tipo booleano

int main(int argc, char *argv[]) {

    clock_t start, end;
    double cpu_time_used;
    
    srand(time(NULL));

    bool debug = false;

    printf("argc: %d\n", argc);
    printf("argv[0]: %s\n", argv[0]);
    printf("argv[1]: %s\n", argv[1]);
    
    if (argc == 2){
        if (strcmp(argv[1], "-d") == 0){
            printf("Modo debug\n");
            debug = true;
        }
        else if (strcmp(argv[1], "-e") == 0){
            printf("Uso: -e requiere argumentos.\n");
        }
        else {
            printf("Uso: %s\n", argv[0]);
            return 1;
        }
    }
    else if(argc == 4){
        if (strcmp(argv[1], "-e") == 0){
            debug = false;
        }
        else {
            printf("Uso: %s\n", argv[0]);
            return 1;
        }
    }
    else{
        printf("Uso: %s\n", argv[0]);
        return 1;
    }

    int D;
    int N;

    if (!debug) {
        D = atoi(argv[2]);
        N = atoi(argv[3]);
    }
    else {
        D = 4;
        N = 6;
    }

    // Asignación y reserva de memoria
    double *bK = (double *) malloc(D * sizeof(double));
    double *bQ = (double *) malloc(D * sizeof(double));
    double *bV = (double *) malloc(D * sizeof(double));
    double *WK_mem = (double *) malloc(D * D * sizeof(double));
    double *WQ_mem = (double *) malloc(D * D * sizeof(double));
    double *WV_mem = (double *) malloc(D * D * sizeof(double));
    double **WK = (double **) malloc(D * sizeof(double *));
    double **WQ = (double **) malloc(D * sizeof(double *));
    double **WV = (double **) malloc(D * sizeof(double *));
    double *X_mem = (double *) malloc(N * D * sizeof(double));
    double **X = (double **) malloc(N * sizeof(double *));
    double *Kn_mem = (double *) malloc(N * D * sizeof(double));
    double **Kn = (double **) malloc(N * sizeof(double *));
    double *Qn_mem = (double *) malloc(N * D * sizeof(double));
    double **Qn = (double **) malloc(N * sizeof(double *));
    double *Vn_mem = (double *) malloc(N * D * sizeof(double));
    double **Vn = (double **) malloc(N * sizeof(double *));
    double *A_mem = (double *) malloc(N * N * sizeof(double));
    double **A = (double **) malloc(N * sizeof(double *));
    double *Anorm_mem = (double *) malloc(N * N * sizeof(double));
    double **Anorm = (double **) malloc(N * sizeof(double *));
    double *c_mem = (double *) malloc(N * D * sizeof(double));
    double **c = (double **) malloc(N * sizeof(double *));
    double *WK_values_mem = (double *) malloc(D * D * sizeof(double));
    double **WK_values = (double **) malloc(D * sizeof(double *));
    double *WQ_values_mem = (double *) malloc(D * D * sizeof(double));
    double **WQ_values = (double **) malloc(D * sizeof(double *));
    double *WV_values_mem = (double *) malloc(D * D * sizeof(double));
    double **WV_values = (double **) malloc(D * sizeof(double *));

    for (int i = 0; i < D; i++) {
        WK[i] = &WK_mem[i * D];
        WQ[i] = &WQ_mem[i * D];
        WV[i] = &WV_mem[i * D];
    }

    for (int i = 0; i < N; i++) {
        X[i] = &X_mem[i * D];
        Kn[i] = &Kn_mem[i * D];
        Qn[i] = &Qn_mem[i * D];
        Vn[i] = &Vn_mem[i * D];
        A[i] = &A_mem[i * N];
        Anorm[i] = &Anorm_mem[i * N];
        c[i] = &c_mem[i * D];
    }

    for (int i = 0; i < D; i++) {
        bK[i] = -1.0;
        bQ[i] = 0.1;
        bV[i] = 0.0;
    }

    // Inicialización de matrices y vectores
    if (debug) {
        // Debug: inicialización de X, WK, WQ, WV con valores conocidos
    } else {
        // Producción: inicialización con valores aleatorios
        #pragma omp parallel for collapse(2)
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < D; j++) {
                X[i][j] = (double)rand() / RAND_MAX * 10.0;
            }
        }

        #pragma omp parallel for collapse(2)
        for (int i = 0; i < D; i++) {
            for (int j = 0; j < D; j++) {
                WK[i][j] = 0.001 * ((rand() / (double)RAND_MAX) - 0.5);
                WQ[i][j] = 0.001 * ((rand() / (double)RAND_MAX) - 0.5);
                WV[i][j] = 0.001 * ((rand() / (double)RAND_MAX) - 0.5);
            }
        }
    }

    start = clock();

    // Calcular Kn, Qn, Vn, A, Anorm, c utilizando OpenMP para paralelizar los bucles

    #pragma omp parallel for collapse(2)
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < D; ++j) {
            Kn[i][j] = 0;
            for (size_t k = 0; k < D; ++k) {
                Kn[i][j] += X[i][k] * WK[k][j];
            }
            Kn[i][j] += bK[j];
        }
    }

    // Repite este patrón para Qn, Vn, A, Anorm, c...

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Tiempo de ejecución: %f\n", cpu_time_used);

    // Liberar memoria
    free(bK);
    free(bQ);
    free(bV);
    free(WK_mem);
    free(WQ_mem);
    free(WV_mem);
    free(WK);
    free(WQ);
    free(WV);
    free(X_mem);
    free(X);
    free(Kn_mem);
    free(Kn);
    free(Qn_mem);
    free(Qn);
    free(Vn_mem);
    free(Vn);
    free(A_mem);
    free(A);
    free(Anorm_mem);
    free(Anorm);
    free(c_mem);
    free(c);
    free(WK_values_mem);
    free(WQ_values_mem);
    free(WV_values_mem);
    free(WK_values);
    free(WQ_values);
    free(WV_values);

    return 0;
}
