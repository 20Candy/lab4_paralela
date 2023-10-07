//Carol Arevalo 20461
//Corto 3 paralela

//compilacion
//mpicc rieman_mpi.c -o rieman_mpi -lm
//mpirun -np 4 ./rieman_mpi

#include <stdio.h>
#include <mpi.h>
#include <math.h>


// // Funcio compleja
// double f(double x) {
//     return exp(-x * x) * cos(x * x) * sin(x) + sqrt(x);
// }


// Fuución a integrar 2x^3
double f(double x) {
    return 2 * x * x * x; // Se define la funcion 2x^3
}

int main(int argc, char* argv[]) {
    int rank, size;
    double a = 2, b = 10;  // Intervalo [a, b]
    int n = 1000;  // Número total de subintervalos
    double h = (b - a) / n;  // Tamaño de un subintervalo
    double local_sum = 0.0;
    double integral;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_n = n / size;  // Número de subintervalos por proceso
    double local_a = a + rank * local_n * h;
    double local_b = local_a + local_n * h;

    for (int i = 0; i < local_n; i++) {
        double x = local_a + i * h;
        local_sum += f(x) * h;
    }

    if (rank != 0) {
        MPI_Send(&local_sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        integral = local_sum;
        for (int src = 1; src < size; src++) {
            MPI_Recv(&local_sum, 1, MPI_DOUBLE, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            integral += local_sum;
        }
    }

    if (rank == 0) {
        printf("Estimación de la integral de f(x) en el intervalo [%.2f, %.2f] = %f\n", a, b, integral);
    }

    MPI_Finalize();
    return 0;
}
