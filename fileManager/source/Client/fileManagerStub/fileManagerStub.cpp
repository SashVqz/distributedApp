/*
	fileManager.cpp --- Sash Vazquez
	.cpp file to implement the fileManager class for the client
*/

#include "fileManagerStub.h"

void sendString(int serverId, std::string data, fileManagerOp op){
	static std::vector<unsigned char> bufferIn;
	static std::vector<unsigned char> bufferOut;
	
	bufferIn.resize(0);
	bufferOut.resize(0);

	int dataLen = (data.length() + 1);

	pack(bufferOut, op);
	pack(bufferOut, dataLen);
	packv(bufferOut, data.c_str(), dataLen);

	sendMSG(serverId, bufferOut);

	recvMSG(serverId, bufferIn);

	int ok = unpack<int>(bufferIn);
	if (!ok) std::cout << "Error: " << __FILE__ << ":" << __LINE__ << std::endl;
}

FileManagerStub::FileManagerStub(std::string path){	
	this->server = initClient(this->ip, this->port);
	sendString(this->server.serverId, path, (fileManagerOp)opBuilder);
};

FileManagerStub::~FileManagerStub(){
	std::vector<unsigned char> bufferIn;
	std::vector<unsigned char> bufferOut;
	
	bufferIn.resize(0);
	bufferOut.resize(0);

	pack(bufferOut, opDestroyer);

	sendMSG(this->server.serverId, bufferOut);
			
	recvMSG(this->server.serverId, bufferIn);
	
	int ok = unpack<int>(bufferIn);
	if (!ok) std::cout << "Error: " << __FILE__ << __LINE__ << std::endl;

	closeConnection(this->server.serverId);
};

std::vector<std::string*>* FileManagerStub::listFiles(){
	std::vector<std::string*>* files = new std::vector<std::string*>();
	static std::vector<unsigned char> bufferIn;
	static std::vector<unsigned char> bufferOut;

	bufferIn.resize(0);
	bufferOut.resize(0);
	
	pack(bufferOut, opListFile);
	
	sendMSG(this->server.serverId, bufferOut);
	
	recvMSG(this->server.serverId, bufferIn);

	int ok = unpack<int>(bufferIn);
	if (ok){
		int filesLen = unpack<int>(bufferIn);
		files->resize(filesLen);
		
		int fileLen = 0;
		
		for (int i = 0; i < filesLen; i++) {
			fileLen = unpack<int>(bufferIn); 

			std::string* file = new std::string();
			file->resize(fileLen);
			
			unpackv(bufferIn, (char*)file->data(), fileLen);
			
			(*files)[i] = file;
		}
	} else {
		std::cout << "Error: " << __FILE__ << ":" << __LINE__ << std::endl;
	}
	
	return files;
}

void FileManagerStub::readFile(char* fileName, char* &data, unsigned long int &dataLen){
	std::vector<unsigned char> bufferIn;
	std::vector<unsigned char> bufferOut;

	bufferIn.resize(0);
	bufferOut.resize(0);
		
	pack(bufferOut, opReadFile);
				
	int fileNameLen = (strlen(fileName) + 1);
	pack(bufferOut, fileNameLen);
	packv(bufferOut, fileName, fileNameLen);
			
	sendMSG(this->server.serverId, bufferOut);
					
	recvMSG(this->server.serverId, bufferIn);
	
	dataLen = unpack<unsigned long int>(bufferIn);
	data = new char[dataLen];
	unpackv(bufferIn, data, dataLen);

	int ok = unpack<int>(bufferIn);
	if (!ok) std::cout << "Error: " << __FILE__ << ":" << __LINE__ << std::endl;
}

void FileManagerStub::writeFile(char* fileName, char* data, unsigned long int dataLen){
	std::vector<unsigned char> bufferIn;
	std::vector<unsigned char> bufferOut;
			
	bufferIn.resize(0);
	bufferOut.resize(0);

	pack(bufferOut, opWriteFile);
			
	int fileNameLen = (strlen(fileName) + 1);
	pack(bufferOut, fileNameLen);
	packv(bufferOut, fileName, fileNameLen);
			
	pack(bufferOut, dataLen);
	packv(bufferOut, data, dataLen);
			
	sendMSG(this->server.serverId, bufferOut);
			
	recvMSG(this->server.serverId, bufferIn);

	int ok = unpack<int>(bufferIn);
	if (!ok) std::cout << "Error: " << __FILE__ << ":" << __LINE__ << std::endl;
}

void FileManagerStub::freeListedFiles(std::vector<std::string*>* fileList){
	for(std::vector<std::string*>::iterator i = fileList->begin(); i!= fileList->end(); ++i) delete *i;

	delete fileList;
}
