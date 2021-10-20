#pragma once

#include <vector>
#include <complex>

class DistLinalgSetup {

public:
    int num_rows;
    int num_cols;
    int start_row;
    int start_col;
    int grid_dim;
    int block_dim;
    int matrix_dim;
    int mpi_comm;
    int grid_row_idx;
    int grid_col_idx;

    DistLinalgSetup() {}

    DistLinalgSetup(int matrixDim, \
                    int blockDim, \
                    int gridDim, \
                    int numRows=-1, \
                    int numCols=-1, \
                    int startRow=-1, \
                    int startCol=-1, \
                    int gridRowIdx = -1, \
                    int gridColIdx = -1, \
                    int mpiComm=-1) {
        matrix_dim = matrixDim;
        block_dim = blockDim;
        grid_dim = gridDim;
        num_rows = numRows;
        num_cols = numCols;
        start_row = startRow;
        start_col = startCol;
        grid_row_idx = gridRowIdx;
        grid_col_idx = gridColIdx;
        mpi_comm = mpiComm;
    }
};

int init_context(DistLinalgSetup setup);

DistLinalgSetup init_backend(int matrix_dim, int block_dim, int grid_dim);

void eig_sol_backend(double* A, double* w, double* V, DistLinalgSetup setup);
void eig_sol_backend(std::complex<double>* A, double* w, std::complex<double>* V, DistLinalgSetup setup);
