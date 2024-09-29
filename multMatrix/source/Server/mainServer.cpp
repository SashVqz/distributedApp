#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>

#include "../utilsApi/utils.h"
#include "../operations/operations.h"
#include "./multMatrixImp/multMatrixImp.h"

void initializeConnection(int clientId);

int main(){
	int clientId = -1;
	int server = initializeServer(15000);
	
	while(1){
		if(checkClient()){
			std::thread* thread = new std::thread(initializeConnection, getLastClientID());
		}else{
			usleep(1000);
		}
	}

	close(server);

	return 0;
}

void initializeConnection(int clientId){
	MultMatrixImp* multMatrixImp = new MultMatrixImp(clientId);

	while(multMatrixImp->exit == false){
		multMatrixImp->receiveOperation();
	}
}
