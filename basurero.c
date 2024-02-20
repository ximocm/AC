#include <stdio.h>
#include <math.h>

#define D 4 // Definimos D como constante para mayor claridad   

int main() {
    double X[6][4] = {{0, 6, 12, 18}, {1, 7, 13, 19}, {2, 8, 14, 20},
                     {3, 9, 15, 21}, {4, 10, 16, 22}, {5, 11, 17, 23}};
    double WK[4][4] = {{-0.2, -0.1, 0.0, 0.1}, {-0.2, -0.1, 0.0, 0.1},
                      {-0.2, -0.1, 0.0, 0.1}, {-0.2, -0.1, 0.0, 0.1}};
    double WQ[4][4] = {{-0.2, -0.2, -0.2, -0.2}, {-0.1, -0.1, -0.1, -0.1},
                      {0.0, 0.0, 0.0, 0.0}, {0.1, 0.1, 0.1, 0.1}};
    double WV[4][4] = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0},
                      {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
    double bK[4] = {-1.0, -1.0, -1.0, -1.0};
    double bQ[4] = {0.1, 0.1, 0.1, 0.1};
    double bV[4] = {0.0, 0.0, 0.0, 0.0};

    // Calcular Kn
    double Kn[6][4];
    for (size_t i = 0; i < 6; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            Kn[i][j] = 0;
            for (size_t k = 0; k < 4; ++k) {
                Kn[i][j] += X[i][k] * WK[k][j];
            }
            Kn[i][j] += bK[j];
        }
    }

    // Calcular Qn
    double Qn[6][4];
    for (size_t i = 0; i < 6; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            Qn[i][j] = 0;
            for (size_t k = 0; k < 4; ++k) {
                Qn[i][j] += X[i][k] * WQ[k][j];
            }
            Qn[i][j] += bQ[j];
        }
    }

    // Calcular Vn
    double Vn[6][4];
    for (size_t i = 0; i < 6; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            Vn[i][j] = 0;
            for (size_t k = 0; k < 4; ++k) {
                Vn[i][j] += X[i][k] * WV[k][j];
            }
            Vn[i][j] += bV[j];
        }
    }

    // Imprimir Kn
    printf("Matriz Kn:\n");
    for (size_t i = 0; i < 6; i++) {
        for (size_t j = 0; j < 4; j++) {
            printf("%f ", Kn[i][j]);
        }
        printf("\n");
    }

    // Imprimir Qn
    printf("\nMatriz Qn:\n");
    for (size_t i = 0; i < 6; i++) {
        for (size_t j = 0; j < 4; j++) {
            printf("%f ", Qn[i][j]);
        }
        printf("\n");
    }

    // Imprimir Vn
    printf("\nMatriz Vn:\n");
    for (size_t i = 0; i < 6; i++) {
        for (size_t j = 0; j < 4; j++) {
            printf("%f ", Vn[i][j]);
        }
        printf("\n");
    }
    
    double A[6][6];
    double sumatorio = 0.0;
    for(size_t i=0; i<6; i++){
        for(size_t j=0; j<6; j++){
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
    for (size_t i = 0; i < 6; i++) {
        for (size_t j = 0; j < 6; j++) {
            printf("%f ", A[i][j]);
        }
        printf("\n");
    }

    double Anorm[6][6];
    double exponente = 0.0;
    double sumatorio_A = 0.0;
    // Calculamos el sumatorio de A
    for (size_t i = 0; i < 6; ++i) {
        for (size_t j = 0; j < 6; ++j) {
            sumatorio_A = 0.0;
            for (size_t d = 0; d < D; ++d) {
                sumatorio_A += exp(A[i][d]);
            }
            exponente = exp(A[i][j]); // Calculamos A[i][j]
            Anorm[i][j] = exponente/sumatorio_A; // Calculamos Anorm[i][j]
        }
    }

    // Imprimir Anorm
    printf("\nMatriz Anorm:\n");
    for (size_t i = 0; i < 6; i++) {
        for (size_t j = 0; j < 6; j++) {
            printf("%f ", Anorm[i][j]);
        }
        printf("\n");
    }


    double c[6][4];
    for(size_t i=0; i<6; i++){
        for(size_t j=0; j<4; j++){
            c[i][j] = 0.0;
            for (size_t k = 0; k < 6; ++k) {
                c[i][j] += Anorm[i][k] * Vn[k][j];
            }
        }
    }
    
    // Imprimir c
    printf("\nMatriz c:\n");
    for (size_t i = 0; i < 6; i++) {
        for (size_t j = 0; j < 4; j++) {
            printf("%.1lf ", c[i][j]);
        }
        printf("\n");
    }

    return 0;

}

