#include "wrappers.h"

#include <elpa/elpa.h>



void eig_sol_backend_real(double* A, double* w, double* V, DistLinalgSetup setup) {
    printf("In backend.\n");

    elpa_t handle;
    int error;

    elpa_init(20200417);

    handle = elpa_allocate(&error);
    if (error != ELPA_OK) {
        printf("Error in elpa_allocate, error = %d\n", error);
    }

    int my_blacs_ctxt;
    int na_rows, na_cols;
    int sc_desc[9];
    int info;
    set_up_blacsgrid_f(setup.mpi_comm, setup.grid_dim, setup.grid_dim, 'R', &my_blacs_ctxt, &setup.grid_row_idx, &setup.grid_col_idx);
    set_up_blacs_descriptor_f(setup.matrix_dim, setup.block_dim, setup.grid_row_idx, setup.grid_col_idx, setup.grid_dim, setup.grid_dim, &na_rows, &na_cols, sc_desc, my_blacs_ctxt, &info);

    /* Set parameters the matrix and it's MPI distribution */
    elpa_set(handle, "na", setup.matrix_dim, &error);                                           // size of the na x na matrix
    assert_elpa_ok(error);
    elpa_set(handle, "nev", setup.matrix_dim, &error);                                         // number of eigenvectors that should be computed ( 1<= nev <= na)
    assert_elpa_ok(error);
    elpa_set(handle, "local_nrows", setup.num_rows, &error);                             // number of local rows of the distributed matrix on this MPI task 
    assert_elpa_ok(error);
    elpa_set(handle, "local_ncols", setup.num_cols, &error);                             // number of local columns of the distributed matrix on this MPI task
    assert_elpa_ok(error);
    elpa_set(handle, "nblk", setup.block_dim, &error);                                       // size of the BLACS block cyclic distribution
    assert_elpa_ok(error);
    elpa_set(handle, "mpi_comm_parent", setup.mpi_comm, &error);    // the global MPI communicator
    assert_elpa_ok(error);
    elpa_set(handle, "process_row", setup.grid_row_idx, &error);                             // row coordinate of MPI process
    assert_elpa_ok(error);
    elpa_set(handle, "process_col", setup.grid_col_idx, &error);                             // column coordinate of MPI process
    assert_elpa_ok(error);
    printf("Created handle.\n");

    assert_elpa_ok(elpa_setup(handle));

    elpa_set(handle, "solver", ELPA_SOLVER_1STAGE, &error);
    assert_elpa_ok(error);

    elpa_set(handle, "gpu", 2, &error); 
    assert_elpa_ok(error);

    elpa_store_settings(handle, "/p/project/qudyngpu/dist_linalg/elpa_settings.txt", &error);

    printf("Run solver.\n");
    // Encountering integer division by zero error...
    elpa_eigenvectors(handle, A, w, V, &error);
    assert_elpa_ok(error);
    printf("Solver done.\n");

    /* cleanup */
    elpa_deallocate(handle, &error);
    elpa_uninit(&error);

}


void eig_sol_backend_complex(cpx* A, double* w, cpx* V, DistLinalgSetup setup) {

    elpa_t handle;
    int error;
    
    elpa_init(20200417);

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

    elpa_eigenvectors(handle, A, w, V, &error);

    /* cleanup */
    elpa_deallocate(handle, &error);
    elpa_uninit(&error);

}
