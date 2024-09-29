/*
	multMatrixImp.cpp --- Sash Vazquez
	.cpp file multMatrixImp, contains the methods of the class multMatrixImp (Server)	
*/

#include "multMatrixImp.h"

MultMatrixImp::MultMatrixImp(int idClient){
	this->idClient = idClient;
	if(this->idClient < 0) std::cout << "Error: initializing connection with client > " << idClient << std::endl;
}

MultMatrixImp::~MultMatrixImp(){
	closeConnection(this->idClient);
}

void MultMatrixImp::recvOp(){
	std::vector<unsigned char> bufferIn;
	std::vector<unsigned char> bufferOut;
	
	bufferIn.resize(0);
	bufferOut.resize(0);

	recvMSG(this->idClient, bufferIn);
			
	mulMatrixOp op = unpack<mulMatrixOp>(bufferIn);		
	switch(op){
		case opBuilder:{
			this->m = new MultMatrix();
			
			pack<int>(bufferOut, (int)1);
			sendMSG(this->idClient, bufferOut);	
		}break;
		
		case opDestroyer:{
			delete this->m;

			this->m = nullptr;
			this->exit = true;
			
			pack<int>(bufferOut, (int)1);
			sendMSG(this->idClient, bufferOut);
		}break;

		case opReadMatrix:{
			int fileNameLenR = unpack<int>(bufferIn);
			std::string fileNameR;
			fileNameR.resize(fileNameLenR);

			unpackv(bufferIn, (char*)fileNameR.data(), fileNameLenR);
					
			matrix_t* matrix = new matrix_t[1];
			matrix = this->m->readMatrix(fileNameR.data());
					
			pack(bufferOut, matrix->rows);
			pack(bufferOut, matrix->cols);
			packv(bufferOut, matrix->data, (matrix->rows * matrix->cols));
			
			sendMSG(this->idClient, bufferOut);
			
			recvMSG(this->idClient, bufferIn);
			
			int ok = unpack<int>(bufferIn);
			if (!ok) std::cout << "Error: " << __FILE__ << __LINE__ << std::endl;
			
			freeMatrix(matrix);
		}break;
		
		case opWriteMatrix:{
			matrix_t* matrix = new matrix_t[1];
					
			matrix->rows = unpack<int>(bufferIn);
			matrix->cols = unpack<int>(bufferIn);
			matrix->data = new int[(matrix->rows * matrix->cols)];
			unpackv<int>(bufferIn, matrix->data, (matrix->rows * matrix->cols));
			
			int fileNameLenW = unpack<int>(bufferIn);
			std::string fileNameW;
			fileNameW.resize(fileNameLenW);

			unpackv(bufferIn, (char*)fileNameW.data(), fileNameLenW);
			
			this->m->writeMatrix(matrix, (char*)fileNameW.data());
			
			pack(bufferOut, (int)1);

			sendMSG(this->idClient, bufferOut);

			freeMatrix(matrix);
		}break;

		case opMultMatrix:{
			matrix_t* m1 = new matrix_t[1];
			m1->rows = unpack<int>(bufferIn);
			m1->cols = unpack<int>(bufferIn);
			m1->data = new int[(m1->rows * m1->cols)];
			unpackv<int>(bufferIn, m1->data, (m1->rows * m1->cols));
			
			matrix_t* m2 = new matrix_t[1];
			m2->rows = unpack<int>(bufferIn);
			m2->cols = unpack<int>(bufferIn);
			m2->data = new int[(m2->rows * m2->cols)];
			unpackv<int>(bufferIn, m2->data, (m2->rows * m2->cols));

			matrix_t* mResult =	new matrix_t[1];
			mResult = this->m->multMatrices(m1, m2);
			
			/*
			for (int i = 0; i < mResult->rows; i++) {
				for (int j = 0; j < mResult->cols; j++) {
					std::cout << mResult->data[i * mResult->cols + j] << " ";
				}
				std::cout << std::endl;
			}
			*/
			
			pack(bufferOut, mResult->rows);
			pack(bufferOut, mResult->cols);
			packv<int>(bufferOut, mResult->data, (mResult->rows * mResult->cols));

			sendMSG(this->idClient, bufferOut);

			recvMSG(this->idClient, bufferIn);
			
			int ok = unpack<int>(bufferIn);
			if (!ok) std::cout << "Error: " << __FILE__ << __LINE__ << std::endl;

			freeMatrix(m1);
			freeMatrix(m2);
			freeMatrix(mResult);
		}break;

		case opCreateIdent:{
			matrix_t* matrix = new matrix_t[1];
			matrix->rows = unpack<int>(bufferIn);
			matrix->cols = unpack<int>(bufferIn);
			matrix->data = new int[0];

			matrix = this->m->createIdentity(matrix->rows, matrix->cols);
			pack(bufferOut, matrix->rows);
			pack(bufferOut, matrix->cols);
			packv(bufferOut, matrix->data, (matrix->rows * matrix->cols));
			
			/*
			for (int i = 0; i < matrix->rows; i++) {
				for (int j = 0; j < matrix->cols; j++) {
					std::cout << matrix->data[i * matrix->cols + j] << " ";
				}
				std::cout << std::endl; 
			}
			*/

			sendMSG(this->idClient, bufferOut);
			
			recvMSG(this->idClient, bufferIn);

			int ok = unpack<int>(bufferIn);
			if (!ok) std::cout << "Error: " << __FILE__ << __LINE__ << std::endl;
			
			freeMatrix(matrix);
		}break;
			
		case opCreateRand:{
			matrix_t* matrix = new matrix_t[1];
			matrix->rows = 0;
			matrix->cols = 0;
			matrix->data = new int[0];

			int rows = unpack<int>(bufferIn);
			int cols = unpack<int>(bufferIn);
			
			matrix = this->m->createRandMatrix(rows, cols);
			
			pack(bufferOut, matrix->rows);
			pack(bufferOut, matrix->cols);
			packv(bufferOut, matrix->data, (matrix->rows * matrix->cols));
			
			/*
			for (int i = 0; i < matrix->rows; i++) {
				for (int j = 0; j < matrix->cols; j++) {
					std::cout << matrix->data[i * matrix->cols + j] << " ";
				}
				std::cout << std::endl;
			}
			*/
			
			sendMSG(this->idClient, bufferOut);

			recvMSG(this->idClient, bufferIn);
			
			int ok = unpack<int>(bufferIn);
			if (!ok) std::cout << "Error: " << __FILE__ << __LINE__ << std::endl;
			
			freeMatrix(matrix);
		}break;	

		default:{
			std::cout << "Error: invalid op" << std::endl;	
		}break;	
	}
};

void MultMatrixImp::freeMatrix(matrix_t* m){
	delete[] m->data;
	delete[] m;
}
