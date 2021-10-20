#include "wrappers.hpp"

#include <elpa/elpha.h>


void eig_sol_backend(double* A, double* w, double* V, DistLinalgSetup setup) {

    elpa_t handle;
    int error;

    handle = elpa_allocate(&error);
    if (error != ELPA_OK) {
        printf("Error in elpa_allocate, error = %d\n", error);
    }

    /* Set parameters the matrix and it's MPI distribution */
    elpa_set(handle, "na", setup.matrix_dim, &error);                                           // size of the na x na matrix
    elpa_set(handle, "nev", setup.matrix_dim, &error);                                         // number of eigenvectors that should be computed ( 1<= nev <= na)
    elpa_set(handle, "local_nrows", setup.num_rows, &error);                             // number of local rows of the distributed matrix on this MPI task 
    elpa_set(handle, "local_ncols", setup.num_cols, &error);                             // number of local columns of the distributed matrix on this MPI task
    elpa_set(handle, "nblk", setup.block_dim, &error);                                       // size of the BLACS block cyclic distribution
    elpa_set(handle, "mpi_comm_parent", setup.mpi_comm, &error);    // the global MPI communicator
    elpa_set(handle, "process_row", setup.grid_row_idx, &error);                             // row coordinate of MPI process
    elpa_set(handle, "process_col", setup.grid_col_idx, &error);                             // column coordinate of MPI process

    error = elpa_setup(handle);

    elpa_set(handle, "solver", ELPA_SOLVER_2STAGE, &error);

    elpa_eigenvectors(handle, a, ev, z, &error);

    /* cleanup */
    elpa_deallocate(handle);
    elpa_uninit();

}
