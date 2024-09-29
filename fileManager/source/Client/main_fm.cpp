/*
    main_fm.cpp --- Sash Vazquez
    main for the file manager client, where we initialize the directories of a server and the client itself
*/

#include "./fileManagerStub/fileManagerStub.h"
#include "./localFileManager/localFilemanager.h"

int main(){
    std::string command;
    bool executingMenu = true;
    bool commandError = true;

    FileManagerStub* fileManagerStub = new FileManagerStub("./dirprueba/");
    LocalFileManager* localFileManager = new LocalFileManager("./dirprueba/");

    char* data = nullptr;
    unsigned long int fileLen = 0;

    std::cout << "Enter 'help' to see the commands\n";
    do{
        std::getline(std::cin, command);
        commandError = true;
        
        // help 
        if(!command.compare("help")){
            std::cout << "Command> ls, download <filename.txt>, upload <filename.txt>, exit\n";
            commandError = false;
        }

        // exit
        if(!command.compare("exit")){
            executingMenu = false;
            commandError = false;
        }

        // ls
        if(!command.compare("ls")){
            std::vector<std::string*>* localFileList = localFileManager->listFiles();
            std::cout << "LocalFiles:\n";
            for(int i = 0; i < localFileList->size(); i++){
                std::cout << "\t" << localFileList->at(i)->c_str() << "\n";
            }
            
            std::vector<std::string*>* serverFileList = fileManagerStub->listFiles();
            std::cout << "ServerFiles:\n";
            for(int i = 0; i < serverFileList->size(); i++){
                std::cout << "\t" << serverFileList->at(i)->c_str() << "\n";
            }
            

            fileManagerStub->freeListedFiles(serverFileList);
            localFileManager->freeListedFiles(localFileList);
            commandError = false;
        }

        // download and upload
        std::stringstream* auxCommand = new std::stringstream(command);
        std::string* commandFilename = new std::string();
        std::vector<std::string>* commandData = new std::vector<std::string>();
        
        while(std::getline(*auxCommand, *commandFilename , ' '))commandData->push_back(*commandFilename );
        
        if(commandData->size() == 2){
            
            // download
            if(!commandData->at(0).compare("download")){
                // Read the file from the server
                fileManagerStub->readFile((char*)commandData->at(1).c_str(), data, fileLen);
                
                // Write the server file locally
                localFileManager->writeFile((char*)commandData->at(1).c_str(), data, fileLen);
                
                std::cout << commandData->at(1).c_str() << " downloaded!\n";
                commandError = false;
            }

            // upload
            if(!commandData->at(0).compare("upload")){
                // Read the local file
                localFileManager->readFile((char*)commandData->at(1).c_str(), data, fileLen);

                // Write the local file to the server
                fileManagerStub->writeFile((char*)commandData->at(1).c_str(), data, fileLen);

                std::cout << commandData->at(1).c_str() << " uploaded!\n";
                commandError = false;
            }
        }

        delete auxCommand;
        delete commandFilename;
        delete commandData;

        // error
        if(commandError) std::cout << "Error: invalid command\n";
    }while(executingMenu);

    delete fileManagerStub;
    delete localFileManager;
    if(data != nullptr) delete[] data;
    return 0;
}
