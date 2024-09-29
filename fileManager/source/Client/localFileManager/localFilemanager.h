/*
    localFileManager.h --- Sash Vazquez
    Header file to control the client's local file system --> copy of filemanager.cpp
*/

#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <dirent.h>

using namespace std;

class LocalFileManager{
    private:
        string dirPath;
        map<string, string*> files;
    public:
        LocalFileManager(string path);
        vector<string*>* listFiles();
        void readFile(char* fileName, char* &data, unsigned long int &dataLength);
        void writeFile(char* fileName, char* data, unsigned long int dataLength);
        void freeListedFiles(vector<string*>* fileList);
};
