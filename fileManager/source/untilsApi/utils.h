#ifndef  _UTILS_H_
#define  _UTILS_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <thread>
#include <mutex>

#define ERROR -1
#define SYNC 0
#define SYNC_ACK 1
#define ACK 2
#define END 3

#define DEBUG
#ifdef DEBUG
#define DEBUG_MSG(...)  printf(__VA_ARGS__);
#endif

typedef struct msg_t{
    int size;
    unsigned char* data;
}msg_t;

typedef struct connection_t{
    unsigned int id;
    unsigned int serverId;
    int socket;
    std::list<msg_t*>* buffer;
    bool alive;
}connection_t;

/**
* Initialization of a server-type program on a port of the host machine
*/
int initServer(int port);

/**
* Method invoked by a server-type program to check if there is any incoming connection
* If it returns true, you can access the identifier of the last connected client 
* using "getLastClientID"
*/
bool checkClient();

/**
* Connection of a client program to a server, using a port and an IP
* The identifier of the server used by clients is "0" by default
*/
connection_t initClient(std::string host, int port);

template<typename t>
void sendMSG(int clientID, std::vector<t> &data);
template<typename t>
void recvMSG(int clientID, std::vector<t> &data);

/**
* Method executed by a server-type program. It is invoked automatically asynchronously
* from "initServer"
*/
int waitForConnections(int sock_fd);
void closeConnection(int clientID);
template<typename t>
void getMSG(int clientID, std::vector<t> &data);

bool checkPendingMessages(int clientID);
void recvMSGAsync(connection_t connection);
void waitForConnectionsAsync(int server_fd);

int getNumClients();
int getClientID(int numClient);
int getLastClientID();

extern std::map<unsigned int,connection_t> clientList;

template<typename t>
void recvMSG(int clientID, std::vector<t> &data){
    connection_t connection=clientList[clientID];
    int socket= connection.socket;
    char msg=0;

    //recv sync
    read(socket, &msg, sizeof(char));
    if(msg!=SYNC){
        printf("ERROR: recvMSG -- line : %d connection error\n", __LINE__);
        exit(0);
    }

    //send sync ack
    msg=SYNC_ACK;
    write(socket,&msg,sizeof(char));

    //recv ack
    read(socket, &msg, sizeof(char));
    if(msg!=ACK){
        printf("ERROR: recvMSG -- line : %d connection error\n", __LINE__);
        exit(0);
    }

    int bufferSize=0;
    read(socket, &bufferSize, sizeof(int));
    DEBUG_MSG("DataRead : %d\n",bufferSize);

    msg=ACK;
    write(socket,&msg,sizeof(char));

    int numElements=bufferSize/sizeof(t);
    data.resize(numElements);
    int remaining=bufferSize;
    int idxIn=0;
    while(remaining>0){
        int bufferSizeBlock=read(socket, &(data.data()[bufferSize-remaining]),remaining);
        remaining-=bufferSizeBlock;        
    }
    
    if(remaining!=0){
       printf("ERROR: recvMSG -- line : %d data mismatch %d read, %d expected\n", __LINE__,remaining,bufferSize);
    }

    write(socket,&bufferSize,sizeof(int));

    read(socket, &msg, sizeof(char));
    
    if(msg!=END){
        printf("ERROR: recvMSG -- line : %d connection error\n", __LINE__);
        exit(0);
    }
}

/** preconditions, variables, etc...**/
template<typename t>
void sendMSG(int clientID, std::vector<t> &data){
    int dataLen=data.size()*sizeof(t);
    connection_t connection=clientList[clientID];
    int socket= connection.socket;
    char msg=0;
    int bufferRecv=0;

    //send sync
    msg=SYNC;
    write(socket,&msg,sizeof(char));
    
    //receive sync ack
    read(socket,&msg,sizeof(char));
    if(msg!=SYNC_ACK){
        printf("ERROR: sendMSG -- line : %d connection error\n", __LINE__);
        exit(0);
    }
    
    //send ack
    msg=ACK;
    write(socket,&msg,sizeof(char));

    //send buffer size
    write(socket,&dataLen,sizeof(int));
    
    //recv ack
    read(socket,&msg,sizeof(char));
    if(msg!=ACK){
        printf("ERROR: sendMSG -- line : %d connection error\n", __LINE__);
        exit(0);
    }

    //send buffer
    write(socket,data.data(),dataLen);
    
    //recv buffer size
    read(socket,&bufferRecv,sizeof(int));
    if(dataLen!=bufferRecv){
        printf("ERROR: sendMSG -- line : %d data mismatch\n", __LINE__);
        exit(0);
    }

    //send end
    msg=END;
    write(socket,&msg,sizeof(char));
}

template<typename t>
void getMSG(int clientID,std::vector<t> &data){
    if(!checkPendingMessages(clientID)){
    data.resize(0);
    }else{   
        msg_t* msg=clientList[clientID].buffer->front();
        
        clientList[clientID].buffer->pop_front();
        
        int numElem=msg->size/sizeof(t);
        data->resize(numElem);
        memcpy(data->data(),msg->data,msg->size);
        
        delete[] msg->data;
        delete[] msg;
    }
}

#endif
