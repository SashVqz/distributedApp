/*
	fileManagerImp.cpp --- Sash Vazquez
	.cpp file fileManagerImp, contains the methods of the class fileManagerImp (Server)	
*/

#include "fileManagerImp.h"

FileManagerImp::FileManagerImp(int idClient){
	this->idClient = idClient;
	if(this->idClient < 0) std::cout << "Error: initializing connection with client > " << idClient << std::endl;
}

FileManagerImp::~FileManagerImp(){
	closeConnection(this->idClient);
}

void FileManagerImp::recvOp(){
	std::vector<unsigned char> bufferIn;
	std::vector<unsigned char> bufferOut;
	
	recvMSG(this->idClient, bufferIn);
	
	fileManagerOp op = unpack<fileManagerOp>(bufferIn);		
	switch(op){
		case opBuilder:{
			std::vector<char> path;
			int pathLen = unpack<int>(bufferIn);		
			
			path.resize(pathLen);
					
			unpackv(bufferIn, (char*)path.data(), pathLen);
			
			this->f = new FileManager(std::string(path.data()));
					
			pack(bufferOut, (int)1);
		}break;
		
		case opDestroyer:{
			delete this->f;
			
			this->f = nullptr;
			this->exit = true;
			
			pack(bufferOut, (int)1);
		}break;

		case opListFile:{					
			vector<string*>* fileList = this->f->listFiles();
			
			pack(bufferOut, (int)1);				
			pack(bufferOut, (int)(fileList->size()));
			
			int fileSize = 0;
			for(int i = 0; i < fileList->size(); i++) {
				std::string* file =  fileList->at(i);
				fileSize = file->size();
				
				pack(bufferOut, (int)fileSize);
						
				packv(bufferOut, (char*)file->data(), fileSize);
			}
			
			freeListedFiles(fileList);
		}break;
				
		case opReadFile:{
			std::string fileName;
			int fileNameLen = unpack<int>(bufferIn);

			fileName.resize(fileNameLen);
			unpackv(bufferIn, (char*)fileName.data(), fileNameLen);
					
			char* data = nullptr;
			unsigned long int dataLen = 0;
					
			this->f->readFile((char*)fileName.data(), data, dataLen);		

			pack(bufferOut, dataLen);
			packv(bufferOut, data, dataLen);
					
			pack(bufferOut, (int)1);
		}break;
			
		case opWriteFile:{
			std::string fileNameW;
			int fileNameLenW = unpack<int>(bufferIn);
			
			fileNameW.resize(fileNameLenW);
			unpackv(bufferIn, (char*)fileNameW.data(), fileNameLenW);
			
			std::string dataW;
			unsigned long int dataLenW = unpack<unsigned long int>(bufferIn);
			
			dataW.resize(dataLenW);
			unpackv(bufferIn, (char*)dataW.data(), dataLenW);
			
			this->f->writeFile((char*)fileNameW.data(), (char*)dataW.data(), dataLenW);
			
			pack(bufferOut, (int)1);
		}break;			
		
		default:{
			std::cout << "Error: invalid op" << std::endl;
		}break;
	}

	sendMSG(this->idClient, bufferOut);
}

void FileManagerImp::freeListedFiles(vector<string*>* fileList) {
	for(vector<string*>::iterator i = fileList->begin(); i != fileList->end(); ++i) delete *i;
	
	delete fileList;
}
