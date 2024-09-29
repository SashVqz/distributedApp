/*
    mainServer.cpp --- Sash Vazquez
    This .cpp file is the main file that will be executed on the server
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>

#include "../untilsApi/utils.h"
#include "../operaciones/operations.h"
#include "./fileManagerImp/fileManagerImp.h"

void initConnection(int idClient);

int main(){
    int server = initServer(15000);
    
    while(1){
        if(checkClient()){
            std::thread* thread = new std::thread(initConnection, getLastClientID());
        }else{
            usleep(1000);
        }
    }

    close(server);
    return 0;
}

void initConnection(int idClient){
    FileManagerImp* fileManagerImp = new FileManagerImp(idClient);
    
    while(fileManagerImp->exit == false){
        fileManagerImp->recvOp();
    }
}
