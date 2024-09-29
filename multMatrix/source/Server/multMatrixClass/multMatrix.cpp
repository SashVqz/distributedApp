#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>

#include "multMatrix.h"

MultMatrix::MultMatrix(){
    srand(getpid());
}

MultMatrix::~MultMatrix(){
    
}

matrix_t* MultMatrix::readMatrix(const char* fileName){
    FILE* file = fopen(fileName,"r");
    if(file == 0){
        std::cout << "Error: failed to open file " << fileName << std::endl;
        return NULL;
    }

    matrix_t* matrix = new matrix_t[1];

    fscanf(file, "%d %d", &matrix->rows, &matrix->cols);
    
    matrix->data = new int[(matrix->rows * matrix->cols)];

    for(int i = 0; i < matrix->rows * matrix->cols; i++) 
        fscanf(file, "%d", &matrix->data[i]);

    fclose(file);
    return matrix;
}

void MultMatrix::writeMatrix(matrix_t* m, const char* fileName){
    FILE* file = fopen(fileName, "w");
    fprintf(file, "%d %d\n", m->rows, m->cols);

    for(int i = 0; i < m->rows * m->cols; i++) 
        fprintf(file, "%d\n", m->data[i]);
    
    fclose(file);
}

matrix_t* MultMatrix::multMatrices(matrix_t* m1, matrix_t* m2){
    if(m1->cols != m2->rows){
        std::cout << "ERROR: Incompatible matrices\n";
        return NULL;
    }

    matrix_t* mres = new matrix_t[1];
    mres->rows = m1->rows;
    mres->cols = m2->cols;
    mres->data = new int[mres->rows * mres->cols];
    for(int i = 0; i < m1->rows; i++) // each row of m1
    {
        for(int j = 0; j < m2->cols; j++) // each column of m2
        {
            mres->data[i * mres->cols + j] = 0;

            for(int k = 0; k < m1->cols; k++) // dot product of m1 row by m2 column
            {
                mres->data[i * mres->cols + j] += m1->data[i * m1->cols + k] * m2->data[k * m2->cols + j];
            }
        }
    }

    return mres;
}

matrix_t* MultMatrix::createIdentity(int rows, int cols){
    matrix_t* m = new matrix_t[1];
    m->rows = rows;
    m->cols = cols;
    m->data = new int[rows * cols];
    
    memset(m->data, 0, sizeof(int) * rows * cols);
    
    for(int i = 0; i < rows; i++)
        m->data[i * cols + i] = 1;
    
    return m;
}

matrix_t* MultMatrix::createRandMatrix(int rows, int cols){
    matrix_t* m = new matrix_t[1];
    m->rows = rows;
    m->cols = cols;
    m->data = new int[(rows * cols)];

    for(int i = 0; i < (rows * cols); i++) 
        m->data[i] = rand();

    return m;
}
