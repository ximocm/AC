#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <mpi.h>

typedef enum { false, true } bool;

int main(int argc, char *argv[]) {
    int world_size, world_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    double start_time = MPI_Wtime();  // Inicia el temporizador

    srand(time(NULL) + world_rank);

    bool debug = false;
    if (world_rank == 0) {
        if (argc == 2 && strcmp(argv[1], "-d") == 0) {
            printf("Modo debug activado.\n");
            debug = true;
        } else if (argc != 4) {
            printf("Uso: %s -e <D> <N>\n", argv[0]);
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }
    }
    MPI_Bcast(&debug, 1, MPI_C_BOOL, 0, MPI_COMM_WORLD);

    int D, N;
    if (world_rank == 0) {
        D = atoi(argv[2]);
        N = atoi(argv[3]);
    }
    MPI_Bcast(&D, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    double **Qn = (double **)malloc(N * sizeof(double *));
    double **Kn = (double **)malloc(N * sizeof(double *));
    double **Vn = (double **)malloc(N * sizeof(double *));
    double **A = (double **)malloc(N * sizeof(double *));
    double **Anorm = (double **)malloc(N * sizeof(double *));
    double **c = (double **)malloc(N * sizeof(double *));
    for (int i = 0; i < N; ++i) {
        Qn[i] = (double *)malloc(D * sizeof(double));
        Kn[i] = (double *)malloc(D * sizeof(double));
        Vn[i] = (double *)malloc(D * sizeof(double));
        A[i] = (double *)malloc(N * sizeof(double));
        Anorm[i] = (double *)malloc(N * sizeof(double));
        c[i] = (double *)malloc(D * sizeof(double));
        for (int j = 0; j < D; ++j) {
            Qn[i][j] = rand() / (double)RAND_MAX;
            Kn[i][j] = rand() / (double)RAND_MAX;
            Vn[i][j] = rand() / (double)RAND_MAX;
        }
    }

    // Aquí va el código del cálculo de las matrices y otras operaciones

     double start = MPI_Wtime();

    // Paralelizando el cálculo de la matriz A
    for (int i = 0; i < N; i++) {
        for (int j = world_rank; j < N; j += world_size) {
            double sumatorio = 0.0;
            A[i][j] = 0.0;
            for (int d = 0; d < D; ++d) {
                sumatorio += Qn[i][d] * Kn[j][d];
            }
            A[i][j] += sumatorio / sqrt(D);
        }
    }

    MPI_Allreduce(MPI_IN_PLACE, *A, N * N, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    // Paralelizando el cálculo de la matriz Anorm
    for (int i = 0; i < N; i++) {
        for (int j = world_rank; j < N; j += world_size) {
            double sumatorio_A = 0.0;
            for (int d = 0; d < N; ++d) {
                sumatorio_A += exp(A[i][d]);
            }
            double exponente = exp(A[i][j]);
            Anorm[i][j] = exponente / sumatorio_A;
        }
    }

    MPI_Allreduce(MPI_IN_PLACE, *Anorm, N * N, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);


    double end_time = MPI_Wtime();  // Detiene el temporizador
    double cpu_time_used = end_time - start_time;  // Calcula el tiempo de ejecución

    if (world_rank == 0) {
        printf("Tiempo de ejecución: %f segundos\n", cpu_time_used);
    }

    // Liberar memoria y finalizar MPI
    for (int i = 0; i < N; ++i) {
        free(Qn[i]);
        free(Kn[i]);
        free(Vn[i]);
        free(A[i]);
        free(Anorm[i]);
        free(c[i]);
    }
    free(Qn);
    free(Kn);
    free(Vn);
    free(A);
    free(Anorm);
    free(c);

    MPI_Finalize();
    return 0;
}
