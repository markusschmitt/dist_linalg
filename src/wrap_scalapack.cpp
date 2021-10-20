#include "wrappers.hpp"

extern "C" void descinit_(int*, int*, int*, int*, int*, int*, int*, int*, int*, int*);
extern "C" void blacs_gridexit_(int*);

extern "C" void pdsyev_(char*, char*, int*, double*, int*, int*, int*, double*, double*, int*, int*, int*, double*, int*, int*);
extern "C" void pzheevd_(char*, char*, int*, std::complex<double>*, int*, int*, int*, double*, std::complex<double>*, int*, int*, int*, std::complex<double>*, int*, double*, int*, int*, int*, int*);


void eig_sol_backend(double* A, double* w, double* V, DistLinalgSetup setup) {

    int ictxt = init_context(setup);

    char uplo = 'U';
    int ione = 1;
    int izero = 0;
    double workTest;

    int descA[9];
    int info;
    int lddA = setup.num_rows;
    
    descinit_( descA,  &setup.matrix_dim, &setup.matrix_dim, &setup.block_dim, &setup.block_dim, &izero, &izero, &ictxt, &lddA, &info);
    if(info != 0) {
        printf("Error in descinit, info = %d\n", info);
    }

    char jobz = 'V';
    int lwork = -1;

    pdsyev_(&jobz, &uplo, &setup.matrix_dim, A, &ione, &ione, descA, w, V, &ione, &ione, descA, &workTest, &lwork, &info);

    lwork = (int) workTest+1;

    std::vector<double> work = std::vector<double>(lwork);
    
    pdsyev_(&jobz, &uplo, &setup.matrix_dim, A, &ione, &ione, descA, w, V, &ione, &ione, descA, &work[0], &lwork, &info);

    blacs_gridexit_(&ictxt);

}


void eig_sol_backend(std::complex<double>* A, double* w, std::complex<double>* V, DistLinalgSetup setup) {

    int ictxt = init_context(setup);

    char uplo = 'U';
    int ione = 1;
    int izero = 0;
    std::complex<double> workTest;
    double rworkTest;
    int iworkTest;

    int descA[9];
    int info;
    int lddA = setup.num_rows;
    
    descinit_( descA,  &setup.matrix_dim, &setup.matrix_dim, &setup.block_dim, &setup.block_dim, &izero, &izero, &ictxt, &lddA, &info);
    if(info != 0) {
        printf("Error in descinit, info = %d\n", info);
    }

    char jobz = 'V';
    int lwork = -1;
    int lrwork = -1;
    int liwork = -1;

    pzheevd_(&jobz, &uplo, &setup.matrix_dim, A, &ione, &ione, descA, w, V, &ione, &ione, descA, &workTest, &lwork, &rworkTest, &lrwork, &iworkTest, &liwork, &info);

    lwork = (int) std::real(workTest)+1;
    lrwork = (int) rworkTest+1;
    liwork = iworkTest;

    std::vector<std::complex<double> > work = std::vector<std::complex<double> >(lwork);
    std::vector<double> rwork = std::vector<double>(lrwork);
    std::vector<int> iwork = std::vector<int>(liwork);
    
    pzheevd_(&jobz, &uplo, &setup.matrix_dim, A, &ione, &ione, descA, w, V, &ione, &ione, descA, &work[0], &lwork, &rwork[0], &lrwork, &iwork[0], &liwork, &info);

    blacs_gridexit_(&ictxt);

}
