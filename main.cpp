#include <stdio.h>

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

    // Imprimir la matriz transpuesta WKt
    printf("Matriz WK transpuesta:\n");
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            printf("%f ", WKt[i][j]);
        }
        printf("\n");
    }

    return 0;
}

