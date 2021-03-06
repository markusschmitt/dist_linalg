#cython: language_level=3

import numpy as np
from mpi4py import MPI
from math import floor

cdef extern from "wrappers.h":

    DistLinalgSetup init_setup(int, int, int, int, int, int, int, int, int, int, int)
    DistLinalgSetup init_backend(int, int, int)
    void eig_sol_backend_real(double*, double*, double*, DistLinalgSetup)
    void eig_sol_backend_complex(double complex*, double*, double complex*, DistLinalgSetup)

#cdef extern from "wrappers.hpp":
#    cdef cppclass DistLinalgSetup:
#        int num_rows, num_cols, start_row, start_col, grid_dim, block_dim, matrix_dim, mpi_comm
#        DistLinalgSetup() except +
#        DistLinalgSetup(int, int, int, int, int, int, int, int) except +
cdef extern from "wrappers.h":
    ctypedef struct DistLinalgSetup:
        int num_rows
        int num_cols
        int start_row
        int start_col
        int grid_dim
        int block_dim
        int matrix_dim
        int num_procs
        int mpi_comm
        int grid_row_idx
        int grid_col_idx


#cdef public class DistLinalgSetup [object c_DLS, type c_DLS_t]:
cdef class PyDistLinalgSetup:

    cdef DistLinalgSetup ptr

    def __init__(self, int matrixDim, int blockDim, int gridDim, int numProcs, int numRows=-1, int numCols=-1, int startRow=-1, int startCol=-1, int gridRowIdx=-1, int gridColIdx=-1):

        self.ptr = init_setup(matrixDim, blockDim, gridDim, numRows, numCols, startRow, startCol, gridRowIdx, gridColIdx, numProcs, MPI.COMM_WORLD.py2f())

#    def __del__(self):
#        if self.ptr != NULL:
#            del self.ptr

    property matrix_dim:
        def __get__(self):
            return self.ptr.matrix_dim
        def __set__(self, int val):
            self.ptr.matrix_dim = val
    
    property block_dim:
        def __get__(self):
            return self.ptr.block_dim
        def __set__(self, int val):
            self.ptr.block_dim = val
    
    property grid_dim:
        def __get__(self):
            return self.ptr.grid_dim
        def __set__(self, int val):
            self.ptr.grid_dim = val
    
    property num_rows:
        def __get__(self):
            return self.ptr.num_rows
        def __set__(self, int val):
            self.ptr.num_rows = val
    
    property num_cols:
        def __get__(self):
            return self.ptr.num_cols
        def __set__(self, int val):
            self.ptr.num_cols = val
    
    property start_row:
        def __get__(self):
            return self.ptr.start_row
        def __set__(self, int val):
            self.ptr.start_row = val
    
    property start_col:
        def __get__(self):
            return self.ptr.start_col
        def __set__(self, int val):
            self.ptr.start_col = val
    
    property grid_row_idx:
        def __get__(self):
            return self.ptr.grid_row_idx
        def __set__(self, int val):
            self.ptr.grid_row_idx = val
    
    property grid_col_idx:
        def __get__(self):
            return self.ptr.grid_col_idx
        def __set__(self, int val):
            self.ptr.grid_col_idx = val
    
    property num_procs:
        def __get__(self):
            return self.ptr.num_procs
        def __set__(self, int val):
            self.ptr.num_procs = val
    
    property mpi_comm:
        def __get__(self):
            return self.ptr.mpi_comm
        def __set__(self, int val):
            self.ptr.mpi_comm = val


def init(int matrix_dim, int block_dim, int grid_dim, int numProcs):
 
    #return init_backend(matrix_dim, block_dim, grid_dim)
    cdef DistLinalgSetup setup = init_backend(matrix_dim, block_dim, grid_dim)

    return PyDistLinalgSetup(setup.matrix_dim, setup.block_dim, setup.grid_dim, numProcs, setup.num_rows, setup.num_cols, setup.start_row, setup.start_col, setup.grid_row_idx, setup.grid_col_idx)


cdef eigh_real(double[::1,:] A, double[::1] w, double[::1,:] Z, DistLinalgSetup setup):

    eig_sol_backend_real(&A[0,0], &w[0], &Z[0,0], setup)  


cdef eigh_complex(double complex[::1,:] A, double[::1] w, double complex[::1,:] Z, DistLinalgSetup setup):

    eig_sol_backend_complex(&A[0,0], &w[0], &Z[0,0], setup)  


def eigh(A, PyDistLinalgSetup py_setup):


    cdef DistLinalgSetup setup = init_setup(py_setup.matrix_dim, py_setup.block_dim, py_setup.grid_dim, py_setup.num_rows, py_setup.num_cols, py_setup.start_row, py_setup.start_col, py_setup.grid_row_idx, py_setup.grid_col_idx, py_setup.num_procs, py_setup.mpi_comm)
    #def eigh(A, DistLinalgSetup setup):
    if MPI.COMM_WORLD.rank < py_setup.num_procs:

        if not A.flags['F_CONTIGUOUS']:
            A = np.asfortranarray(A)

        dataType = A.dtype

        Z = np.zeros((setup.num_rows, setup.num_cols), order='F', dtype=dataType)
        w = np.zeros((setup.matrix_dim, ))
      
        if dataType == np.float64:
            eigh_real(A, w, Z, setup)

        if dataType == np.cdouble:
            eigh_complex(A, w, Z, setup)

        return w, Z

    else:

        return None, None


def distribute(matrix_dim, num_processes=None):

    comm = MPI.COMM_WORLD

    if num_processes is None:
        num_processes = comm.size

    if comm.size < num_processes:
        num_processes = comm.size

    grid_dim = floor(np.sqrt(num_processes))
    num_processes = grid_dim*grid_dim

    block_dim = (matrix_dim + grid_dim - 1) // grid_dim

    return init(matrix_dim, block_dim, grid_dim, num_processes)
