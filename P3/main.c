#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {

    double start, end;
    double cpu_time_used;
    int size, rank;

    srand(time(NULL));

    bool debug = true;
/*
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
*/
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
    
    double *Kn = malloc(N * D * sizeof(double *));
    double *Qn = malloc(N * D * sizeof(double *));
    double *Vn = malloc(N * D * sizeof(double *));
    double *Anorm = malloc(N * N * sizeof(double *));
    double *c = malloc(N * D * sizeof(double *));
    double *bK = malloc(D * sizeof(double *));
    double *bQ = malloc(D * sizeof(double *));
    double *bV = malloc(D * sizeof(double *));
    double *WK = malloc(D * D * sizeof(double *));
    double *WQ = malloc(D * D * sizeof(double *));
    double *WV = malloc(D * D * sizeof(double *));
    double *X = malloc(N * D * sizeof(double *));
 

    printf("Asignación de valores bias\n");

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
        double bK_values[4] = {-1.0 , -1.0, -1.0, -1.0, };

        double bQ_values[4] = {0.1, 0.1, 0.1, 0.1};

        double bV_values[4] = {0.0, 0.0, 0.0, 0.0};

        for (int i = 0; i < D; i++) {
            bK[i] = bK_values[i];
            bQ[i] = bQ_values[i];
            bV[i] = bV_values[i];
        }

        for (int i = 0; i < D; i++) {
            for (int j = 0; j < D; j++) {
                WK[i*D+j] = WK_values[i][j];
                WQ[i*D+j] = WQ_values[i][j];
                WV[i*D+j] = WV_values[i][j];
            }
        }

        for(int i = 0; i < N; i++) {
            for(int j = 0; j < D; j++) {
                X[i*D+j] = X_values[i][j];
            }
        }

    }
    else{
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < D; j++) {
                X[i*D+j] = (float)rand()/RAND_MAX * 10.0;
            }
        }

        printf("Asignación de valores entrada\n");
        
        for (int i = 0; i < D; i++) {
            for (int j = 0; j < D; j++) {
                WK[i*D+j] = 0.001*((rand() / (float)RAND_MAX) - 0.5);
                WQ[i*D+j] = 0.001*((rand() / (float)RAND_MAX) - 0.5);
                WV[i*D+j] = 0.001*((rand() / (float)RAND_MAX) - 0.5);
            }
        }
    }

    //Mostrar X
    if (rank == 0) {
        printf("Mostrar X\n");
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < D; ++j) {
                printf("%f ", X[i*D+j]);
            }
            printf("\n");
        }
    }

    //mostar WK
    if (rank == 0) {
         printf("Mostrar WK\n");
        for (size_t i = 0; i < D; ++i) {
            for (size_t j = 0; j < D; ++j) {
                printf("%f ", WK[i*D+j]);
            }
            printf("\n");
        }
    }

    //mostar WQ
    
    if (rank == 0) {
        printf("Mostrar WQ\n");
        for (size_t i = 0; i < D; ++i) {
            for (size_t j = 0; j < D; ++j) {
                printf("%f ", WQ[i*D+j]);
            }
            printf("\n");
        }
    }

    //mostar WV
    
    if (rank == 0) {
        printf("Mostrar WV\n");
        for (size_t i = 0; i < D; ++i) {
            for (size_t j = 0; j < D; ++j) {
                printf("%f ", WV[i*D+j]);
            }
            printf("\n");
        }
    }

    MPI_Init(&argc, &argv);
    start = MPI_Wtime();
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* INICIO DEL PROGRAMA */

    printf("Punto de control 1\n");
    start = 0;

    double *X_local = malloc((N/size) * D * sizeof(double *));
    double *kn_local = malloc((N/size) * D * sizeof(double *));
    double *qn_local = malloc((N/size) * D * sizeof(double *));
    double *vn_local = malloc((N/size) * D * sizeof(double *));
    double *_list = malloc((N/size) * D * sizeof(double *));
    double *Anorm_local = malloc((N/size) * N * sizeof(double *));
    double *c_local = malloc((N/size) * D * sizeof(double *));

    printf("Punto de control 2\n");

    // Repartición de datos entre los procesos con MPI_Scatter
    MPI_Scatter(X, (N/size)*D, MPI_DOUBLE, X_local, (N/size)*D, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Repartición de datos entre los procesos con MPI_Bcast
    MPI_Bcast(WK, D*D, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(WQ, D*D, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(WV, D*D, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(bK, D, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(bQ, D, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(bV, D, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    printf("Punto de control 3\n");

    // Calcular Kn, Qn, Vn
    for (size_t i = 0; i < N/size; ++i) {
        for (size_t j = 0; j < D; ++j) {
            kn_local[i*D+j] = 0;
            qn_local[i*D+j] = 0;
            vn_local[i*D+j] = 0;
            for (size_t k = 0; k < D; ++k) {
                kn_local[i*D+j] += X_local[i*D+k] * WK[k*D+j];
                qn_local[i*D+j] += X_local[i*D+k] * WQ[k*D+j];
                vn_local[i*D+j] += X_local[i*D+k] * WV[k*D+j];
            }
            kn_local[i*D+j] += bK[j];
            qn_local[i*D+j] += bQ[j];
            vn_local[i*D+j] += bV[j];
        }
    }

    
    MPI_Allgather(kn_local, (N/size)*D, MPI_DOUBLE, Kn, (N/size)*D, MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Allgather(vn_local, (N/size)*D, MPI_DOUBLE, Vn, (N/size)*D, MPI_DOUBLE, MPI_COMM_WORLD);
    //Se pot llevar el qn_local a Qn
    MPI_Allgather(qn_local, (N/size)*D, MPI_DOUBLE, Qn, (N/size)*D, MPI_DOUBLE, MPI_COMM_WORLD);

    //Mostrar Kn_local
    
    if (rank == 0) {
        printf("Mostrar Kn_local\n");
        for (size_t i = 0; i < N/size; ++i) {
            for (size_t j = 0; j < D; ++j) {
                printf("%f ", Kn[i*D+j]);
            }
            printf("\n");
        }
    }

    //Mostrar Qn_local
    
    if (rank == 0) {
        printf("Mostrar Qn_local\n");
        for (size_t i = 0; i < N/size; ++i) {
            for (size_t j = 0; j < D; ++j) {
                printf("%f ", Qn[i*D+j]);
            }
            printf("\n");
        }
    }
    
    //Mostrar Vn_local
    
    if (rank == 0) {
        printf("Mostrar Vn_local\n");
        for (size_t i = 0; i < N/size; ++i) {
            for (size_t j = 0; j < D; ++j) {
                printf("%f ", Vn[i*D+j]);
            }
            printf("\n");
        }
    }

    printf("Punto de control 2\n");

    double sumatorio = 0.0;
    for(size_t i=0; i<N/size; i++){
        for(size_t j=0; j<N; j++){
            sumatorio = 0.0;
            Anorm_local[i*N+j] = 0.0;
            for (size_t d = 0; d < D; ++d) {
                sumatorio += qn_local[i*D+d] * Kn[j*D+d];
            }
            Anorm_local[i*N+j] += sumatorio/sqrt(D);
        }
    }

    //Mostrar Anorm_local
    if (rank == 0) {
        printf("Mostrar Anorm_local\n");
        for (size_t i = 0; i < N/size; ++i) {
            for (size_t j = 0; j < D; ++j) {
                printf("%f ", Anorm_local[i*D+j]);
            }
            printf("\n");
        }
    }

   printf("Punto de control 3\n");
    
    double sumatorio_A = 0.0;
    // Calculamos el sumatorio de A
    for (size_t i = 0; i < N/size; ++i) {
        sumatorio_A = 0.0;
        for (size_t d = 0; d < N; ++d) {
                sumatorio_A += exp(Anorm_local[i*N+d]);
        }
        for (size_t j = 0; j < N; ++j) {
            Anorm_local[i*N+j] =  exp(Anorm_local[i*N+j])/sumatorio_A; // Calculamos Anorm_local[i][j]
        }
    }

    if (rank == 0) {
        printf("Mostrar Anorm_local\n");
        for (size_t i = 0; i < N/size; ++i) {
            for (size_t j = 0; j < D; ++j) {
                printf("%f ", Anorm_local[i*D+j]);
            }
            printf("\n");
        }
    }

    printf("Punto de control 4\n");

    for(size_t i=0; i<N/size; i++){
        for(size_t j=0; j<D; j++){
            c_local[i*D+j] = 0.0;
            for (size_t k = 0; k < N; ++k) {
                c_local[i*D+j] += Anorm_local[i*N+k] * Vn[k*D+j];
            }
        }
    }

    printf("Punto de control 5\n");
    
    end = 410;
        cpu_time_used = end - start;

    printf("Tiempo de ejecución: %f\n", cpu_time_used);

    MPI_Gather(c_local, (N/size)*D, MPI_DOUBLE, c, (N/size)*D, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    //Mostramos c
    if (rank == 0) {
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < D; ++j) {
                printf("%f ", c[i*D+j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();

    return 0;


}

