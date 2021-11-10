#ifndef WRAPPER_H
#define WRAPPER_H
//#pragma once

//#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <assert.h>

#define cpx double complex

#define assert_elpa_ok(x) assert(x == ELPA_OK)

typedef struct DistLinalgSetup {

    int num_rows;
    int num_cols;
    int start_row;
    int start_col;
    int grid_dim;
    int block_dim;
    int matrix_dim;
    int num_procs;
    int mpi_comm;
    int grid_row_idx;
    int grid_col_idx;

} DistLinalgSetup;

DistLinalgSetup init_setup(int matrixDim, \
                    int blockDim, \
                    int gridDim, \
                    int numRows, \
                    int numCols, \
                    int startRow, \
                    int startCol, \
                    int gridRowIdx, \
                    int gridColIdx, \
                    int numProcs, \
                    int mpiComm);

int init_context(DistLinalgSetup setup);

DistLinalgSetup init_backend(int matrix_dim, int block_dim, int grid_dim);

void eig_sol_backend_real(double* A, double* w, double* V, DistLinalgSetup setup);
void eig_sol_backend_complex(cpx* A, double* w, cpx* V, DistLinalgSetup setup);

#endif
