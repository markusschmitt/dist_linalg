#include "wrappers.h"

void blacs_get_(int*, int*, int*);
void blacs_gridinit_(int*, char*, int*, int*);
void blacs_gridinfo_(int*, int*, int*, int*, int*);
void blacs_gridexit_(int*);
int numroc_(int*, int*, int*, int*, int*);

DistLinalgSetup init_setup(int matrixDim, \
                    int blockDim, \
                    int gridDim, \
                    int numRows, \
                    int numCols, \
                    int startRow, \
                    int startCol, \
                    int gridRowIdx, \
                    int gridColIdx, \
                    int mpiComm) {
        DistLinalgSetup setup = {
                                    .matrix_dim = matrixDim, \
                                    .block_dim = blockDim, \
                                    .grid_dim = gridDim, \
                                    .num_rows = numRows, \
                                    .num_cols = numCols, \
                                    .start_row = startRow, \
                                    .start_col = startCol, \
                                    .grid_row_idx = gridRowIdx, \
                                    .grid_col_idx = gridColIdx, \
                                    .mpi_comm = mpiComm
                                };
        return setup;
    }

int init_context(DistLinalgSetup setup) {

    int ictxt;
    int izero = 0;
    char layout = 'R';

    int grid_dim = setup.grid_dim;

    blacs_get_( &izero, &izero, &ictxt );
    blacs_gridinit_( &ictxt, &layout, &grid_dim, &grid_dim );

    return ictxt;

}


DistLinalgSetup init_backend(int matrix_dim, int block_dim, int grid_dim) {
 
    //DistLinalgSetup setup = DistLinalgSetup(matrix_dim, block_dim, grid_dim);
    DistLinalgSetup setup = init_setup(matrix_dim, block_dim, grid_dim, -1, -1, -1, -1, -1, -1, -1);

    int ictxt = init_context(setup);
    int izero = 0;
    int gridRowIdx, gridColIdx;

    blacs_gridinfo_(&ictxt, &grid_dim, &grid_dim, &gridRowIdx, &gridColIdx );

    setup.num_rows = numroc_( &matrix_dim, &block_dim, &gridRowIdx, &izero, &grid_dim );
    setup.num_cols = numroc_( &matrix_dim, &block_dim, &gridColIdx, &izero, &grid_dim );

    setup.start_row = gridRowIdx * block_dim;
    setup.start_col = gridColIdx * block_dim;

    setup.grid_row_idx = gridRowIdx;
    setup.grid_col_idx = gridRowIdx;

    blacs_gridexit_(&ictxt);

    return setup;

}
