#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main() {

    int D = 4;
    int N = 6;

    float *bK = (float *) malloc(D * sizeof(float));
    float *bQ = (float *) malloc(D * sizeof(float));
    float *bV = (float *) malloc(D * sizeof(float));

    // Reservar memoria para cada fila de bK, bQ, bV
    float *WK_mem = (float *) malloc(D * D * sizeof(float));
    float *WQ_mem = (float *) malloc(D * D * sizeof(float));
    float *WV_mem = (float *) malloc(D * D * sizeof(float));

    float **WK = (float **) malloc(D * sizeof(float *));
    float **WQ = (float **) malloc(D * sizeof(float *));
    float **WV = (float **) malloc(D * sizeof(float *));
    for (int i = 0; i < D; i++) {
        WK[i] = &WK_mem[i * D];
        WQ[i] = &WQ_mem[i * D];
        WV[i] = &WV_mem[i * D];
    }

    float *X_mem = (float *) malloc(N * D * sizeof(float));
    float **X = (float **) malloc(N * sizeof(float *));
    for (int i = 0; i < N; i++) {
        X[i] = &X_mem[i * D];
    }

    // Inicialización de X, WK, WQ, WV
    float X_values[6][4] = {{0, 6, 12, 18}, {1, 7, 13, 19}, {2, 8, 14, 20},
                     {3, 9, 15, 21}, {4, 10, 16, 22}, {5, 11, 17, 23}};
    float WK_values[4][4] = {{-0.2, -0.1, 0.0, 0.1}, {-0.2, -0.1, 0.0, 0.1},
                      {-0.2, -0.1, 0.0, 0.1}, {-0.2, -0.1, 0.0, 0.1}};
    float WQ_values[4][4] = {{-0.2, -0.2, -0.2, -0.2}, {-0.1, -0.1, -0.1, -0.1},
                      {0.0, 0.0, 0.0, 0.0}, {0.1, 0.1, 0.1, 0.1}};
    float WV_values[4][4] = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0},
                      {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};

    // Inicialización de bK, bQ, bV
    float bK_values[4] = {-1.0, -1.0, -1.0, -1.0};
    float bQ_values[4] = {0.1, 0.1, 0.1, 0.1};
    float bV_values[4] = {0.0, 0.0, 0.0, 0.0};

    for(int i = 0; i < D; i++) {
        bK[i] = bK_values[i];
        bQ[i] = bQ_values[i];
        bV[i] = bV_values[i];
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

    // Calcular Kn
    float Kn[N][D];
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < D; ++j) {
            Kn[i][j] = 0;
            for (size_t k = 0; k < D; ++k) {
                Kn[i][j] += X[i][k] * WK[k][j];
            }
            Kn[i][j] += bK[j];
        }
    }

    // Calcular Qn
    float Qn[N][D];
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < D; ++j) {
            Qn[i][j] = 0;
            for (size_t k = 0; k < D; ++k) {
                Qn[i][j] += X[i][k] * WQ[k][j];
            }
            Qn[i][j] += bQ[j];
        }
    }

    // Calcular Vn
    float Vn[N][D];
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < D; ++j) {
            Vn[i][j] = 0;
            for (size_t k = 0; k < D; ++k) {
                Vn[i][j] += X[i][k] * WV[k][j];
            }
            Vn[i][j] += bV[j];
        }
    }

    // Imprimir Kn
    printf("Matriz Kn:\n");
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < D; j++) {
            printf("%f ", Kn[i][j]);
        }
        printf("\n");
    }

    // Imprimir Qn
    printf("\nMatriz Qn:\n");
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < D; j++) {
            printf("%f ", Qn[i][j]);
        }
        printf("\n");
    }

    // Imprimir Vn
    printf("\nMatriz Vn:\n");
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < D; j++) {
            printf("%f ", Vn[i][j]);
        }
        printf("\n");
    }
    
    float A[N][N];
    float sumatorio = 0.0;
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

    // Imprimir A
    printf("\nMatriz A:\n");
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            printf("%f ", A[i][j]);
        }
        printf("\n");
    }

    float Anorm[N][N];
    float exponente = 0.0;
    float sumatorio_A = 0.0;
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

    // Imprimir Anorm
    printf("\nMatriz Anorm:\n");
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            printf("%f ", Anorm[i][j]);
        }
        printf("\n");
    }


    float c[N][D];
    for(size_t i=0; i<N; i++){
        for(size_t j=0; j<D; j++){
            c[i][j] = 0.0;
            for (size_t k = 0; k < N; ++k) {
                c[i][j] += Anorm[i][k] * Vn[k][j];
            }
        }
    }
    
    // Imprimir c
    printf("\nMatriz c:\n");
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < D; j++) {
            printf("%.1lf ", c[i][j]);
        }
        printf("\n");
    }

    return 0;

}

