/* 
    Main multMatrix --- Sash Vazquez
    Execution file for the multMatrix class for the client
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "./multMatrixStub/multMatrixStub.h"

void freeMatrix(matrix_t* m);

int main(){
    MultMatrixStub* m = new MultMatrixStub();
    matrix_t* m1 = m->createRandMatrix(5, 5);
    matrix_t* m2 = m->createIdentity(5, 5);
    matrix_t* mres = m->multMatrices(m1, m2);
    m->writeMatrix(mres, "result.txt");
    matrix_t* m3 = m->readMatrix("result.txt");
    matrix_t* mres2 = m->multMatrices(m3, m3);
    m->writeMatrix(mres2, "result2.txt");

    freeMatrix(m1);
    freeMatrix(m2);
    freeMatrix(mres);
    freeMatrix(m3);
    freeMatrix(mres2);
    delete m;
}

void freeMatrix(matrix_t* m){ 
    delete[] m->data;
    delete[] m;
}