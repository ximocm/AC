#include <stdio.h>

#define D 4 // Definimos D como constante para mayor claridad

int main() {
    float X[6][4] = {{0, 6, 12, 18}, {1, 7, 13, 19}, {2, 8, 14, 20},
                     {3, 9, 15, 21}, {4, 10, 16, 22}, {5, 11, 17, 23}};
    float WK[4][4] = {{-0.2, -0.1, 0.0, 0.1}, {-0.2, -0.1, 0.0, 0.1},
                      {-0.2, -0.1, 0.0, 0.1}, {-0.2, -0.1, 0.0, 0.1}};
    float WQ[4][4] = {{-0.2, -0.2, -0.2, -0.2}, {-0.1, -0.1, -0.1, -0.1},
                      {0.0, 0.0, 0.0, 0.0}, {0.1, 0.1, 0.1, 0.1}};
    float WV[4][4] = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0},
                      {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
    float bK[4] = {-1.0, -1.0, -1.0, -1.0};
    float bQ[4] = {0.1, 0.1, 0.1, 0.1};
    float bV[4] = {0.0, 0.0, 0.0, 0.0};

    // Transponer la matriz WK
    float WKt[4][4];
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            WKt[j][i] = WK[i][j];
        }
    }

    // Transponer la matriz WQ
    float WQt[4][4];
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            WQt[j][i] = WQ[i][j];
        }
    }

    // Transponer la matriz WV
    float WVt[4][4];
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            WVt[j][i] = WV[i][j];
        }
    }

    // Calcular Kn
    float Kn[6][4];
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
    float Qn[6][4];
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
    float Vn[6][4];
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

    


    return 0;
}