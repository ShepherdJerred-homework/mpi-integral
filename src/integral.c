#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int MAIN_PID = 0;
int MAX_NUMBER_OF_COEFFICIENTS = 10;

double calcX1ForProcess(double x1, double x2, int i, int p) {
    return x1 + ((double) i / p) * (x2 - x1);
}

double calcX2ForProcess(double x1, double x2, int i, int p) {
    return x1 + ((double) (i + 1) / p) * (x2 - x1);
}

double f(double *coeff, double x, int numberOfCoeffs) {
    double y = 0;
    int i;
    for (i = 0; i < numberOfCoeffs; ++i) {
        double t = coeff[i];
        int j;
        for (j = 0; j < numberOfCoeffs - 1 - i; ++j) {
            t *= x;
        }
        y += t;
    }
    return y;
}

double integral(double *coeff, double xBegin, double xEnd, int n, int numberOfCoeffs) {
    double xDistance = xEnd - xBegin;
    double totalArea = 0;
    int i;
    for (i = 0; i < n; ++i) {
        double x1 = xBegin + i * xDistance / n;
        double x2 = xBegin + (i + 1) * xDistance / n;

        double y1 = f(coeff, x1, numberOfCoeffs);
        double y2 = f(coeff, x2, numberOfCoeffs);

        double intArea = (y1 + y2) / 2 * (x2 - x1);
        totalArea += intArea;
    }
    return totalArea;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int pid;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);

    int numberOfProcesses;
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);

    double coeff[MAX_NUMBER_OF_COEFFICIENTS];
    double x1;
    double x2;
    int n;
    int numberOfCoeffs;

    if (pid == MAIN_PID) {
        scanf("%lf", &x1);
        scanf("%lf", &x2);
        scanf("%i", &n);

        numberOfCoeffs = 0;

        char line[100];

        while (fgets(line, sizeof(line), stdin) != NULL) {
            if (line[0] == '\n') {
                continue;
            }

            coeff[numberOfCoeffs] = strtod(line, NULL);
            numberOfCoeffs += 1;
        }
    }

    int i;
    for (i = 0; i < numberOfCoeffs; i++) {
//        printf("%lf", coeff[i]);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&coeff, MAX_NUMBER_OF_COEFFICIENTS, MPI_DOUBLE, MAIN_PID, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&x1, 1, MPI_DOUBLE, MAIN_PID, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&x2, 1, MPI_DOUBLE, MAIN_PID, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_DOUBLE, MAIN_PID, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&numberOfCoeffs, 1, MPI_INT, MAIN_PID, MPI_COMM_WORLD);

    double myX1 = calcX1ForProcess(x1, x2, pid, numberOfProcesses);
    double myX2 = calcX2ForProcess(x1, x2, pid, numberOfProcesses);

    double integralValue = integral(coeff, myX1, myX2, n, numberOfCoeffs);
    double recv;

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Reduce(&integralValue, &recv, 1, MPI_DOUBLE, MPI_SUM, MAIN_PID, MPI_COMM_WORLD);

    if (pid == MAIN_PID) {
//        recv += integralValue;
        printf("%lf", recv);
    }

    MPI_Finalize();
}