/*
Autor: Carol Arevalo
Para compilar: gcc -o riemann riemann.c -lm
Fecha: viernes 11 de agosto de 2022
Version: 1.0
*/

// librerias
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "omp.h"

// Funciones
double f1(double x) {
    return x * x; // Se define la funcion x^2
}

double f2(double x) {
    return 2 * x * x * x; // Se define la funcion 2x^3
}

double f3(double x) {
    return sin(x); // Se define la funcion sin(x)
}

// Funcion trapecios
double trapezoides(double a, double b, int n, double (*func)(double)) {
    double h = (b - a) / n; // Se calcula el ancho de cada trapecio
    double sum = 0.5 * (func(a) + func(b)); // Se calcula la suma de las alturas de los trapecios

    // Se calcula la suma de las alturas de los trapecios
    for (int i = 1; i < n; ++i) {
        double x = a + i * h;
        sum += func(x);
    }

    // Se calcula el area total
    return h * sum;
}

//Funcion principal
int main(int argc, char* argv[]) {
    // Parametros por defecto
    double a = 2.0; 
    double b = 10.0; 
    int num_threads = 4;
    int n = 1000000; // Valor por defecto para n 

    // Si se ingresan parametros, se toman esos valores
    if (argc > 3) {
        a = atof(argv[1]);
        b = atof(argv[2]);
        num_threads = atoi(argv[3]); 
    }

    // se calcula el ancho de cada trapecio
    double h = (b - a) / n;

    // Se cambbia a una variable global
    double* local_sums = (double*)malloc(num_threads * sizeof(double)); // Se reserva memoria para los resultados locales

    // Se calcula la integral con la funcion trapezoides
    #pragma omp parallel num_threads(num_threads)
    {
        int thread_id = omp_get_thread_num(); // Se obtiene el id del hilo
        int n_local = n / num_threads; // Se calcula la cantidad de trapecios por hilo
        double a_local = a + thread_id * n_local * h; // Se calcula el limite inferior de cada hilo
        double b_local = a_local + n_local * h; // Se calcula el limite superior de cada hilo

        double local_sum = trapezoides(a_local, b_local, n_local, f1);  // Se calcula la suma de las alturas de los trapecios de cada hilo

        local_sums[thread_id] = local_sum; // Se guarda el resultado local
    }

    // Se calcula la suma total de las alturas de los trapecios
    double total_sum = 0.0;
    for (int i = 0; i < num_threads; ++i) {
        total_sum += local_sums[i];
    }

    printf("Con n = %d trapezoides, nuestra aproximacion de la integral de %.6lf a %.6lf es %.10lf\n", n, a, b, total_sum);

    free(local_sums); // Se libera la memoria

    return 0;
}