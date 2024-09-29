#include "multMatrixStub.h"

MultMatrixStub::MultMatrixStub() {
	std::vector<unsigned char> bufferIn;
	std::vector<unsigned char> bufferOut;
	
	this->server = initClient(this->ip, this->port);
	
	pack(bufferOut, opBuilder);

	sendMSG(this->server.serverId, bufferOut); 

	recvMSG(this->server.serverId, bufferIn);

	int ok = unpack<int>(bufferIn);
	if (!ok) std::cout << "Error: " << __FILE__ << __LINE__ << std::endl;
}

MultMatrixStub::~MultMatrixStub() {
	std::vector<unsigned char> bufferIn;
	std::vector<unsigned char> bufferOut;

	pack(bufferOut, opDestroyer);

	sendMSG(this->server.serverId, bufferOut);
		
	recvMSG(this->server.serverId, bufferIn);

	int ok = unpack<int>(bufferIn);
	if (!ok) std::cout << "Error: " << __FILE__ << __LINE__ << std::endl;

	closeConnection(this->server.serverId);
}

matrix_t* MultMatrixStub::readMatrix(const char* fileName) {
	std::vector<unsigned char> bufferIn;
	std::vector<unsigned char> bufferOut;
	matrix_t* matrix = new matrix_t[1];
	int fileNameLen = (strlen(fileName) + 1);

	bufferIn.resize(0);
	bufferOut.resize(0);
	
	pack(bufferOut, opReadMatrix);
	
	pack(bufferOut, fileNameLen);

	packv(bufferOut, fileName, fileNameLen);
	
	sendMSG(this->server.serverId, bufferOut);

	recvMSG(this->server.serverId, bufferIn);

	matrix->rows = unpack<int>(bufferIn);
	matrix->cols = unpack<int>(bufferIn);
	matrix->data = new int[(matrix->rows * matrix->cols)];
	unpackv(bufferIn, matrix->data, (matrix->rows * matrix->cols));

	pack(bufferOut, (int)1);
	sendMSG(this->server.serverId, bufferOut);

	return matrix;
};

void MultMatrixStub::writeMatrix(matrix_t* m, const char* fileName) {
	std::vector<unsigned char> bufferIn;
	std::vector<unsigned char> bufferOut;
	int fileNameLen = (strlen(fileName) + 1);

	bufferIn.resize(0);
	bufferOut.resize(0);

	pack(bufferOut, opWriteMatrix);

	pack(bufferOut, m->rows);
	pack(bufferOut, m->cols);
	packv(bufferOut, m->data, (m->rows * m->cols));
	
	pack(bufferOut, fileNameLen);
	packv(bufferOut, fileName, fileNameLen);

	sendMSG(this->server.serverId, bufferOut);
	
	recvMSG(this->server.serverId, bufferIn);

	int ok = unpack<int>(bufferIn);
	if (!ok) std::cout << "Error: " << __FILE__ << __LINE__ << std::endl;
}
	
matrix_t* MultMatrixStub::multMatrices(matrix_t* m1, matrix_t* m2) {
	static std::vector<unsigned char> bufferIn;
	static std::vector<unsigned char> bufferOut;
	matrix_t* matrix = new matrix_t[1];
		
	bufferIn.resize(0);
	bufferOut.resize(0);

	pack(bufferOut, opMultMatrix);

	pack(bufferOut, m1->rows);
	pack(bufferOut, m1->cols);
	packv(bufferOut, m1->data, (m1->rows * m1->cols));

	pack(bufferOut, m2->rows);
	pack(bufferOut, m2->cols);
	packv(bufferOut, m2->data, (m2->rows * m2->cols));

	sendMSG(this->server.serverId, bufferOut);

	recvMSG(this->server.serverId, bufferIn);

	matrix->rows = unpack<int>(bufferIn);
	matrix->cols = unpack<int>(bufferIn);
	matrix->data = new int[(matrix->rows * matrix->cols)];
	unpackv<int>(bufferIn, (int*)matrix->data, (matrix->rows * matrix->cols));
	
	pack(bufferOut, (int)1);
	sendMSG(this->server.serverId, bufferOut);

	return matrix;
};

matrix_t* MultMatrixStub::createIdentity(int rows, int cols){
	static std::vector<unsigned char> bufferIn;
	static std::vector<unsigned char> bufferOut;
	
	matrix_t* matrix = new matrix_t[1];
	matrix->rows = rows;
	matrix->cols = cols;
	matrix->data = new int[(rows * cols)];

	bufferIn.resize(0);
	bufferOut.resize(0);

	pack(bufferOut, opCreateIdent);

	pack(bufferOut, rows);
	pack(bufferOut, cols);

	sendMSG(this->server.serverId, bufferOut);

	recvMSG(this->server.serverId, bufferIn);

	matrix->rows = unpack<int>(bufferIn);
	matrix->cols = unpack<int>(bufferIn);
	unpackv<int>(bufferIn, matrix->data, (matrix->rows * matrix->cols));
	
	pack(bufferOut, (int)1);
	sendMSG(this->server.serverId, bufferOut);

	return matrix;
}

matrix_t* MultMatrixStub::createRandMatrix(int rows, int cols){
	static std::vector<unsigned char> bufferIn;
	static std::vector<unsigned char> bufferOut;

	matrix_t* matrix = new matrix_t[1];
	matrix->rows = rows;
	matrix->cols = cols;
	matrix->data = new int[(rows * cols)];
	
	bufferIn.resize(0);
	bufferOut.resize(0);

	pack(bufferOut, opCreateRand);
	
	pack(bufferOut, rows);
	pack(bufferOut, cols);

	sendMSG(this->server.serverId, bufferOut);
	
	recvMSG(this->server.serverId, bufferIn);

	matrix->rows = unpack<int>(bufferIn);
	matrix->cols = unpack<int>(bufferIn);
	unpackv<int>(bufferIn, matrix->data, (matrix->rows * matrix->cols));

	pack(bufferOut, (int)1);
	sendMSG(this->server.serverId, bufferOut);

	return matrix;
}