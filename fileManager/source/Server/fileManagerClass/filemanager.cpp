// .cpp of the FileManager class (provided in the statement)

#include "filemanager.h"

FileManager::FileManager(string path){
    this->dirPath = path;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            if(ent->d_type == DT_REG){
                string* f = new string(ent->d_name);
                this->files[*f] = f;
            }
        }
        closedir(dir);
    } else {
        string* f = new string("ERROR: File or directory does not exist");
        this->files[*f] = f;
        std::cout << "ERROR: File or directory does not exist\n";
    }
}

FileManager::~FileManager(){

}

vector<string*>* FileManager::listFiles(){
    vector<string*>* flist = new vector<string*>();
    
    for(map<string, string*>::iterator i = files.begin(); i != files.end(); ++i){
        flist->push_back(new string(i->first));
    }
    
    return flist;
}

void FileManager::readFile(char* fileName, char* &data, unsigned long int & dataLength){
    string path = this->dirPath + "/" + string(fileName);
    FILE* f = fopen(path.c_str(), "r");

    fseek(f, 0L, SEEK_END);
    dataLength = ftell(f);
    fseek(f, 0L, SEEK_SET);
    data = new char[dataLength];

    fread(data, dataLength, 1, f);
    fclose(f);
}

void FileManager::writeFile(char* fileName, char* data, unsigned long dataLength){
    string path = this->dirPath + "/" + string(fileName);
    FILE* f = fopen(path.c_str(), "w");
    fwrite(data, dataLength, 1, f);
    fclose(f);
    
    if(files.find(string(fileName)) == files.end()) files[string(fileName)] = new string(fileName);
}

void FileManager::freeListedFiles(vector<string*>* fileList){
    for(vector<string*>::iterator i = fileList->begin(); i != fileList->end(); ++i) delete *i;
    delete fileList;
}