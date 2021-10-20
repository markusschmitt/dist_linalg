import os
import sys
import subprocess

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from setuptools.command.install import install
from setuptools.command.develop import develop

from Cython.Build import cythonize
import numpy as np

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
            ext.libraries = ["scalapack", "lapack", "blas"]
            ext.sources = ext.sources + ["src/wrap_scalapack.cpp"]
        
        if self.linalg_backend == 'elpa':
            ext.libraries = ["elpa", "lapack", "blas"]
            ext.sources = ext.sources + ["src/wrap_elpa.cpp"]

        super().build_extension(ext)


ext = Extension("dist_linalg",
                sources=["src/dist_linalg.pyx", "src/wrap_blacs_init.cpp"],
                language="c++")

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
)
