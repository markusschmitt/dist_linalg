# Description
`dist_linalg` is a ScaLAPACK/ELPA wrapper for distributed eigensolvers.

# Requirements

* ScaLAPACK (can, e.g., be installed using `conda`)
* `gcc` compiler
* `cython`

# Installation

First, clone this repository and enter its top directory:
```
git clone https://github.com/markusschmitt/dist_linalg.git
cd dist_linalg
```

Then create a `setup.cfg` with that specifies the backend (only `scalapack` possible for now) and the compile flags required to link to this library.

If you have ScaLAPACK installed, `setup.cfg` should read
```
[build_ext]
linalg-backend=scalapack
libraries=scalapack lapack blas
```

If you plan on using the MKL implementation, `setup.cfg` should read
```
[build_ext]
linalg-backend=scalapack
libraries=mkl_scalapack_lp64 mkl_blacs_intelmpi_lp64 mkl_rt iomp5 pthread dl stdc++
```

Then run
```
pip install ./
```

To test whether everything worked, run the unit tests:
```
python -m unittest unittests/*.py
```

# Usage

Basic example:

```python
import numpy as np
import dist_linalg

matrix_dim = 70   # Matrix dimension

# Initialize the setup
distSetup = dist_linalg.distribute(matrix_dim)

m = distSetup.num_rows
n = distSetup.num_cols
m0 = distSetup.start_row
n0 = distSetup.start_col

# Create the local block of the matrix
myBlock = np.zeros((distSetup.num_rows,distSetup.num_cols), dtype=np.double)

# here we fill the matrix according to M_{i,j} = 1/(1+i+j)
for i in range(distSetup.num_rows):
    for j in range(distSetup.num_cols):
        idx = 1 + distSetup.num_rows + distSetup.num_cols + distSetup.start_row + distSetup.start_col
        myBlock[i,j] = 1/(1+m0+n0+i+j)

# diagonalize the matrix
ev, V = dist_linalg.eigh(myBlock, distSetup)
```
