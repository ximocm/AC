#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main() {

    int D = 4;
    int N = 6;

    // Reservar memoria para bK, bQ, bV como matrices de punteros dobles
    double *bK = (float *) malloc(N * sizeof(double *));
    double *bQ = (float *) malloc(N * sizeof(double *));
    double *bV = (float *) malloc(N * sizeof(double *));
    
    // Verificar si la memoria se ha asignado correctamente
    if (bK == NULL || bQ == NULL || bV == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria\n");
        return 1;
    }

    // Reservar memoria para cada fila de bK, bQ, bV
    for (int i = 0; i < N; i++) {
        bK[i] = malloc(D * sizeof(double));
        bQ[i] = malloc(D * sizeof(double));
        bV[i] = malloc(D * sizeof(double));
        
        // Verificar si la memoria se ha asignado correctamente
        if (bK[i] == NULL || bQ[i] == NULL || bV[i] == NULL) {
            fprintf(stderr, "Error: No se pudo asignar memoria\n");
            return 1;
        }
    }

    // Inicialización de X, WK, WQ, WV
    double X[6][4] = {{0, 6, 12, 18}, {1, 7, 13, 19}, {2, 8, 14, 20},
                     {3, 9, 15, 21}, {D, 10, 16, 22}, {5, 11, 17, 23}};
    double WK[4][4] = {{-0.2, -0.1, 0.0, 0.1}, {-0.2, -0.1, 0.0, 0.1},
                      {-0.2, -0.1, 0.0, 0.1}, {-0.2, -0.1, 0.0, 0.1}};
    double WQ[4][4] = {{-0.2, -0.2, -0.2, -0.2}, {-0.1, -0.1, -0.1, -0.1},
                      {0.0, 0.0, 0.0, 0.0}, {0.1, 0.1, 0.1, 0.1}};
    double WV[4][4] = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0},
                      {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};

    // Inicialización de bK, bQ, bV
    double bK_values[4] = {-1.0, -1.0, -1.0, -1.0};
    double bQ_values[4] = {0.1, 0.1, 0.1, 0.1};
    double bV_values[4] = {0.0, 0.0, 0.0, 0.0};
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < D; j++) {
            bK[i][j] = bK_values[j];
            bQ[i][j] = bQ_values[j];
            bV[i][j] = bV_values[j];
        }
    }

    // Calcular Kn
    double Kn[N][D];
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < D; ++j) {
            Kn[i][j] = 0;
            for (size_t k = 0; k < D; ++k) {
                Kn[i][j] += X[i][k] * WK[k][j];
            }
            Kn[i][j] += *bK[j];
        }
    }

    // Calcular Qn
    double Qn[N][D];
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < D; ++j) {
            Qn[i][j] = 0;
            for (size_t k = 0; k < D; ++k) {
                Qn[i][j] += X[i][k] * WQ[k][j];
            }
            Qn[i][j] += *bQ[j];
        }
    }

    // Calcular Vn
    double Vn[N][D];
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < D; ++j) {
            Vn[i][j] = 0;
            for (size_t k = 0; k < D; ++k) {
                Vn[i][j] += X[i][k] * WV[k][j];
            }
            Vn[i][j] += *bV[j];
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
    
    double A[N][N];
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

    // Imprimir A
    printf("\nMatriz A:\n");
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            printf("%f ", A[i][j]);
        }
        printf("\n");
    }

    double Anorm[N][N];
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

    // Imprimir Anorm
    printf("\nMatriz Anorm:\n");
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            printf("%f ", Anorm[i][j]);
        }
        printf("\n");
    }


    double c[N][D];
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


     for (int i = 0; i < N; i++) {
        free(bK[i]);
        free(bQ[i]);
        free(bV[i]);
    }
    free(bK);
    free(bQ);
    free(bV);

    return 0;

}

