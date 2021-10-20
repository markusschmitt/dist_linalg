#include "wrappers.hpp"

extern "C" void blacs_get_(int*, int*, int*);
extern "C" void blacs_gridinit_(int*, char*, int*, int*);
extern "C" void blacs_gridinfo_(int*, int*, int*, int*, int*);
extern "C" void blacs_gridexit_(int*);
extern "C" int numroc_(int*, int*, int*, int*, int*);

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
 
    DistLinalgSetup setup = DistLinalgSetup(matrix_dim, block_dim, grid_dim);

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
