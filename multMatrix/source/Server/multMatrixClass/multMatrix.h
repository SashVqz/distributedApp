#ifndef MULTMATRIX_H
#define MULTMATRIX_H

typedef struct matrix_t{
    int rows;
    int cols;
    int* data;
}matrix_t;

class MultMatrix{
    public:
        MultMatrix();
        ~MultMatrix();
        matrix_t* readMatrix(const char* fileName);
        void writeMatrix(matrix_t* m, const char* fileName);
        matrix_t* multMatrices(matrix_t* m1, matrix_t* m2);
        matrix_t* createIdentity(int rows, int cols);
        matrix_t* createRandMatrix(int rows, int cols);
};

#endif // MULTMATRIX_H