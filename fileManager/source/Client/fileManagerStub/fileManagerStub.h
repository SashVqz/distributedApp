/*
	fileManager.h --- Sash Vazquez
	Header file for the FileManager class for the client
*/

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>

#include "../../untilsApi/utils.h"
#include "../../operaciones/operaciones.h"

class FileManagerStub{
	private:
		std::string ip = "172.31.45.139"; // ip maquina 3
		int port = 31083;
		connection_t server;

	public:
		FileManagerStub(std::string path);
		~FileManagerStub();
		std::vector<std::string*>* listFiles();
		void readFile(char* fileName, char* &data, unsigned long int &dataLength);
		void writeFile(char* fileName, char* data, unsigned long int dataLength);
        void freeListedFiles(std::vector<std::string*>* fileList);
};
