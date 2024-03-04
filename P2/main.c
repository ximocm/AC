#include<math.h>
#include<stdio.h>
#include<omp.h>

int main(int argc, char *argv[]){
    int n,i;
    printf("Intoduce la precision: ");
    scanf("%d", &n);
    double pi5d = 3.141592653589793238462643;
    double h = 1.0 / n;
    double sum = 0.0;

    #pragma omp parallel for shared(num)
    for(i=1; i<=n;i++){
        #pragma omp atomic
        double x = h*((double)i - 0.5);
        sum += 4.0 / (1.0 + x * x);
    }
    double pi = h * sum;
    printf("El valor de pi es: %f, con un error de %f\n", pi, fabs(pi - pi5d));
    return 0;
}