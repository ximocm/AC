#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
        //printf("D: %d\n", D);
        N = atoi(argv[3]);
        //printf("N: %d\n", N);
    }
    else {
        D = 4;
        N = 6;
    }
    

    double *bK = (double *) malloc(D * sizeof(double));
    double *bQ = (double *) malloc(D * sizeof(double));
    double *bV = (double *) malloc(D * sizeof(double));

    // Reservar memoria para cada fila de bK, bQ, bV
    double *WK_mem = (double *) malloc(D * D * sizeof(double));
    double *WQ_mem = (double *) malloc(D * D * sizeof(double));
    double *WV_mem = (double *) malloc(D * D * sizeof(double));

    double **WK = (double **) malloc(D * sizeof(double *));
    double **WQ = (double **) malloc(D * sizeof(double *));
    double **WV = (double **) malloc(D * sizeof(double *));
    for (int i = 0; i < D; i++) {
        WK[i] = &WK_mem[i * D];
        WQ[i] = &WQ_mem[i * D];
        WV[i] = &WV_mem[i * D];
    }

    double *X_mem = (double *) malloc(N * D * sizeof(double));
    double **X = (double **) malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) {
        X[i] = &X_mem[i * D];
    }

    for(int i = 0; i < D; i++) {
        bK[i] = -1.0;
        bQ[i] = 0.1;
        bV[i] = 0.0;
    }

    if (debug){ 
        // Inicialización de X, WK, WQ, WV
        double X_values[6][4] = {{0, 6, 12, 18}, {1, 7, 13, 19}, {2, 8, 14, 20},
                        {3, 9, 15, 21}, {4, 10, 16, 22}, {5, 11, 17, 23}};
        double WK_values[4][4] = {{-0.2, -0.1, 0.0, 0.1}, {-0.2, -0.1, 0.0, 0.1},
                        {-0.2, -0.1, 0.0, 0.1}, {-0.2, -0.1, 0.0, 0.1}};
        double WQ_values[4][4] = {{-0.2, -0.2, -0.2, -0.2}, {-0.1, -0.1, -0.1, -0.1},
                        {0.0, 0.0, 0.0, 0.0}, {0.1, 0.1, 0.1, 0.1}};
        double WV_values[4][4] = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0},
                        {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};

        for (int i = 0; i < D; i++) {
            for (int j = 0; j < D; j++) {
                WK[i][j] = WK_values[i][j];
                WQ[i][j] = WQ_values[i][j];
                WV[i][j] = WV_values[i][j];
            }
        }

        for(int i = 0; i < N; i++) {
            for(int j = 0; j < D; j++) {
                X[i][j] = X_values[i][j];
            }
        }

    }
    else{
        double X_values[N][D];
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < D; j++) {
                X_values[i][j] = (float)rand()/RAND_MAX * 10.0;
            }
        }


        double WK_values[D][D];
        double WQ_values[D][D];
        double WV_values[D][D];
        for (int i = 0; i < D; i++) {
            for (int j = 0; j < D; j++) {
                WK_values[i][j] = 0.001*((rand() / (float)RAND_MAX) - 0.5);
                WQ_values[i][j] = 0.001*((rand() / (float)RAND_MAX) - 0.5);
                WV_values[i][j] = 0.001*((rand() / (float)RAND_MAX) - 0.5);
            }
        }

        for (int i = 0; i < D; i++) {
            for (int j = 0; j < D; j++) {
                WK[i][j] = WK_values[i][j];
                WQ[i][j] = WQ_values[i][j];
                WV[i][j] = WV_values[i][j];
            }
        }

        for(int i = 0; i < N; i++) {
            for(int j = 0; j < D; j++) {
                X[i][j] = X_values[i][j];
            }
        }
    }

    printf("Punto de control 1");
    start = clock();    

    // Calcular Kn
    double *Kn_mem = (double *) malloc(N * D * sizeof(double));
    double **Kn = (double **) malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) {
        Kn[i] = &Kn_mem[i * D];
    }
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < D; ++j) {
            Kn[i][j] = 0;
            for (size_t k = 0; k < D; ++k) {
                Kn[i][j] += X[i][k] * WK[k][j];
            }
            Kn[i][j] += bK[j];
        }
    }
    
    free(bK);

    // Calcular Qn
    double *Qn_mem = (double *) malloc(N * D * sizeof(double));
    double **Qn = (double **) malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) {
        Qn[i] = &Qn_mem[i * D];
    }
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < D; ++j) {
            Qn[i][j] = 0;
            for (size_t k = 0; k < D; ++k) {
                Qn[i][j] += X[i][k] * WQ[k][j];
            }
            Qn[i][j] += bQ[j];
        }
    }

    free(bQ);

    // Calcular Vn
    double *Vn_mem = (double *) malloc(N * D * sizeof(double));
    double **Vn = (double **) malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) {
        Vn[i] = &Vn_mem[i * D];
    }
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < D; ++j) {
            Vn[i][j] = 0;
            for (size_t k = 0; k < D; ++k) {
                Vn[i][j] += X[i][k] * WV[k][j];
            }
            Vn[i][j] += bV[j];
        }
    }

    free(bV);
    printf("Punto de control 2");

    double *A_mem = (double *) malloc(N * N * sizeof(double));
    double **A = (double **) malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) {
        A[i] = &A_mem[i * N];
    }
    double sumatorio = 0.0;
    for(size_t i=0; i<N; i++){
        for(size_t j=0; j<N; j++){
            sumatorio = 0.0;
            A[i][j] = 0.0;
            for (size_t d = 0; d < D; ++d) {
                sumatorio += Qn[i][d] * Kn[j][d];
            }
            A[i][j] += sumatorio/sqrt(D);
        }
    }

    free(Kn_mem);
    free(Kn);
    free(Qn_mem);
    free(Qn);
    free(X_mem);
    free(X);

    double *Anorm_mem = (double *) malloc(N * N * sizeof(double));
    double **Anorm = (double **) malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) {
        Anorm[i] = &Anorm_mem[i * N];
    }
    double exponente = 0.0;
    double sumatorio_A = 0.0;
    // Calculamos el sumatorio de A
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            sumatorio_A = 0.0;
            for (size_t d = 0; d < N; ++d) {
                sumatorio_A += exp(A[i][d]);
            }
            exponente = exp(A[i][j]); // Calculamos A[i][j]
            Anorm[i][j] = exponente/sumatorio_A; // Calculamos Anorm[i][j]
        }
    }

    printf("Punto de control 3");

    free(A_mem);
    free(A);


    double *c_mem = (double *) malloc(N * D * sizeof(double));
    double **c = (double **) malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) {
        c[i] = &c_mem[i * D];
    }
    for(size_t i=0; i<N; i++){
        for(size_t j=0; j<D; j++){
            c[i][j] = 0.0;
            for (size_t k = 0; k < N; ++k) {
                c[i][j] += Anorm[i][k] * Vn[k][j];
            }
        }
    }

    free(Vn_mem);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Tiempo de ejecución: %f\n", cpu_time_used);

    return 0;

}

