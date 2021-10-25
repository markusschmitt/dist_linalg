#include "wrappers.h"

void descinit_(int*, int*, int*, int*, int*, int*, int*, int*, int*, int*);
void blacs_gridexit_(int*);

void pdsyev_(char*, char*, int*, double*, int*, int*, int*, double*, double*, int*, int*, int*, double*, int*, int*);
void pzheevd_(char*, char*, int*, cpx*, int*, int*, int*, double*, cpx*, int*, int*, int*, cpx*, int*, double*, int*, int*, int*, int*);


void eig_sol_backend_real(double* A, double* w, double* V, DistLinalgSetup setup) {

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

    //std::vector<double> work = std::vector<double>(lwork);
    double* work = (double*) malloc(lwork*sizeof(double));
    
    pdsyev_(&jobz, &uplo, &setup.matrix_dim, A, &ione, &ione, descA, w, V, &ione, &ione, descA, &work[0], &lwork, &info);

    free(work);

    blacs_gridexit_(&ictxt);

}


void eig_sol_backend_complex(cpx* A, double* w, cpx* V, DistLinalgSetup setup) {

    int ictxt = init_context(setup);

    char uplo = 'U';
    int ione = 1;
    int izero = 0;
    cpx workTest;
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

    lwork = (int) creal(workTest)+1;
    lrwork = (int) rworkTest+1;
    liwork = iworkTest;

    //std::vector<cpx > work = std::vector<cpx >(lwork);
    //std::vector<double> rwork = std::vector<double>(lrwork);
    //std::vector<int> iwork = std::vector<int>(liwork);
    
    cpx* work = (cpx*) malloc(lwork*sizeof(cpx));
    double* rwork = (double*) malloc(lrwork*sizeof(double));
    int* iwork = (int*) malloc(liwork*sizeof(int));
    
    pzheevd_(&jobz, &uplo, &setup.matrix_dim, A, &ione, &ione, descA, w, V, &ione, &ione, descA, &work[0], &lwork, &rwork[0], &lrwork, &iwork[0], &liwork, &info);

    free(work);
    free(rwork);
    free(iwork);

    blacs_gridexit_(&ictxt);

}
