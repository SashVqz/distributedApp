#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "../../untilsApi/utils.h"
#include "../../operaciones/operaciones.h"
#include "../../Server/multMatrixClass/multMatrix.h"

class MultMatrixStub {
    private:
        std::string ip = "";
        int port = 31082;
        connection_t server;
    
    public:
        MultMatrixStub();
        ~MultMatrixStub();
	    matrix_t* readMatrix(const char* fileName);
        void writeMatrix(matrix_t* m, const char* fileName);
	    matrix_t* multMatrices(matrix_t* m1, matrix_t* m2);	
	    matrix_t* createIdentity(int rows, int cols);
        matrix_t* createRandMatrix(int rows, int cols);
};