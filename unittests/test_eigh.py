import unittest
import numpy as np
import dist_linalg

from mpi4py import MPI

class TestOperator(unittest.TestCase):

    def test_real(self):

        N=70
        distSetup = dist_linalg.distribute(N)

        m = distSetup.num_rows
        n = distSetup.num_cols
        m0 = distSetup.start_row
        n0 = distSetup.start_col

        myBlock = np.zeros((m,n), dtype=np.double)
        for i in range(m):
            for j in range(n):
                myBlock[i,j] = 1/(1+m0+n0+i+j)

        ev, V = dist_linalg.eigh(myBlock, distSetup)


        if MPI.COMM_WORLD.Get_rank() == 0:
            
            mat = np.zeros((N,N), dtype=np.float64)
            for i in range(N):
                for j in range(N):
                    mat[i,j] = 1/(1+i+j)

            ev0, V0 = np.linalg.eigh(mat)

            self.assertTrue( np.linalg.norm(ev - ev0) < 1e-14)
    

    def test_complex(self):

        N=70
        distSetup = dist_linalg.distribute(N)

        m = distSetup.num_rows
        n = distSetup.num_cols
        m0 = distSetup.start_row
        n0 = distSetup.start_col

        myBlock = np.zeros((m,n), dtype=np.cdouble)
        for i in range(m):
            for j in range(n):
                myBlock[i,j] = 1/(1+m0+n0+i+j)

        ev, V = dist_linalg.eigh(myBlock, distSetup)

        if MPI.COMM_WORLD.Get_rank() == 0:
            
            mat = np.zeros((N,N), dtype=np.cdouble)
            for i in range(N):
                for j in range(N):
                    mat[i,j] = 1/(1+i+j)

            ev0, V0 = np.linalg.eigh(mat)

            self.assertTrue( np.linalg.norm(ev - ev0) < 1e-14)
