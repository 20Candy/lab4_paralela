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

// Funcion principal
int main(int argc, char* argv[]) {

    // Parametros por defecto
    double a = 2.0; 
    double b = 10.0; 
    int n = 4;

    // Si se ingresan parametros, se toman esos valores
    if (argc > 1) {
        a = atof(argv[1]);
        b = atof(argv[2]);
        n = atoi(argv[3]);
    }

    // Se calcula la integral para cada funcion
    double result1 = trapezoides(a, b, n, f1); // Se calcula la integral para x^2
    double result2 = trapezoides(a, b, n, f2); // Se calcula la integral para 2x^3
    double result3 = trapezoides(a, b, n, f3); // Se calcula la integral para sin(x)

    // Se imprimen los resultados
    printf("Con n = %d trapezoides, nuestra aproximacion de la integral de %.6lf a %.6lf:\n", n, a, b);
    printf("Para x^2: %.10lf\n", result1);
    printf("Para 2x^3: %.10lf\n", result2);
    printf("Para sin(x): %.10lf\n", result3);

    return 0;
}
