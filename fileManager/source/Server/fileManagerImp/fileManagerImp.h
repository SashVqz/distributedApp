/*
	fileManagerImp.h --- Sash Vazquez
	Header file FileManagerImp, contains the prototypes of the FileManagerImp class (Server)	
*/

#pragma once

#include "../fileManagerClass/filemanager.h"

#include "../../operaciones/operaciones.h"
#include "../../untilsApi/utils.h"

class FileManagerImp {
	private:
		FileManager* f = nullptr;
		int idClient = -1;
	public:
		bool exit = false;
		FileManagerImp(int idClient);		
		~FileManagerImp();
		void recvOp();
		void freeListedFiles(vector<string*>* fileList);
};
