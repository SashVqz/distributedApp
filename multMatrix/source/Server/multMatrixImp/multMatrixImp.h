/*
	multMatrixImp.cpp --- Sash Vazquez
	multMatrixImp.cpp file, contains the methods of the multMatrixImp class (Server)	
*/

#pragma once

#include "../multMatrixClass/multMatrix.h"
#include "../../operaciones/operaciones.h"
#include "../../untilsApi/utils.h"

class MultMatrixImp {
	private:
		int idClient;
		MultMatrix* m = nullptr;
	public:
		bool exit = false;
		MultMatrixImp(int idClient);
		~MultMatrixImp();
		void recvOp();
		void freeMatrix(matrix_t* m);
};
