import os
import sys
import subprocess

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

from Cython.Build import cythonize

class my_build_ext(build_ext):

    user_options = build_ext.user_options + [
        ('linalg-backend=', None, 'Parallel linear algebra library to be used (\'scalapack\' or \'elpa\')')
    ]

    def initialize_options(self):
        build_ext.initialize_options(self)
        self.linalg_backend = 'scalapack'

    def finalize_options(self):
        build_ext.finalize_options(self)

    def build_extension(self, ext):
        if self.linalg_backend == 'scalapack':
            #if len(ext.libraries) == 0:
            #    ext.libraries = ["scalapack", "lapack", "blas"]
            ext.sources = ext.sources + ["src/wrap_scalapack.c"]
        
        if self.linalg_backend == 'elpa':
            #ext.libraries = ["elpa", "lapack", "blas"]
            #ext.library_dirs = ["/p/software/juwelsbooster/stages/2020/software/ELPA/2020.11.001-gpsmkl-2021/lib"]
            #ext.libraries = ["elpa",
            #                "mkl_scalapack_lp64", "/p/software/juwelsbooster/stages/2020/software/imkl/2021.2.0-gpsmpi-2021/mkl/2021.2.0/lib/intel64/libmkl_blacs_openmpi_lp64.a", "mkl_intel_lp64", "mkl_sequential",
            #                "mkl_core", "pthread", "dl", "stdc++"]
            ext.sources = ext.sources + ["src/wrap_elpa.c"]

        super().build_extension(ext)

ext = Extension("dist_linalg",
                sources=["src/dist_linalg.pyx", "src/wrap_blacs_init.c"],
                extra_compile_args=["-std=c99"],
#                language="c++"
                )

setup(
    name="dist_linalg",
    version="0.0.1",
    author="Markus Schmitt",
    author_email="markus.schmitt@uni-koeln.de",
    description="A ScaLAPACK/ELPA wrapper for distributed eigensolvers",
    long_description="",
    ext_modules=cythonize(ext),
    cmdclass={'build_ext': my_build_ext},
    zip_safe=False,
    install_requires=[
        'mpi4py',
        'numpy',
    ],
    setup_requires=[
        'cython',
    ],
)
