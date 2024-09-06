#pragma once

#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "common.h"

using namespace std;

class SharedMemoryManager{

public:
    SharedMemoryManager(size_t dataSize, size_t statusSize);
    ~SharedMemoryManager();

    void* getDataPtr() const {return dataPtr;}
    void* getStatusPtr() const {return statusPtr;}

    size_t getDataSize() const {return dataSize;}
    size_t getStatusSize() const {return statusSize;}

    // set values in the shared status 
    bool checkStatus(int bit){return true;}
    void setStatus(int bit){}

private:
    int dataFd;
    void* dataPtr;
    size_t dataSize;

    int statusFd;
    void* statusPtr;
    size_t statusSize;

    void accessSharedData(bool create);
    void accessSharedStatus(bool create);


};

SharedMemoryManager::SharedMemoryManager(size_t dataSize, size_t statusSize) : 
    dataSize(dataSize), statusSize(statusSize){

    accessSharedData(true);
    accessSharedStatus(true);

}

void SharedMemoryManager::accessSharedData(bool create){

    dataFd = open(SHARED_MEM_DATA, (create ? (O_CREAT | O_RDWR) : O_RDWR), 0666);
    if(dataFd < 0){
        cerr << "Failed to open shared memory file" << endl;
        return;
    }

    if(create && ftruncate(dataFd, dataSize) < 0){
        cerr << "File truncate failed" << endl;
        close(dataFd);
        return;
    }

    dataPtr = mmap(nullptr, dataSize, PROT_READ | PROT_WRITE, MAP_SHARED, dataFd, 0);
    if(dataPtr == MAP_FAILED){
        cerr << "Failed to map memory" << endl;
        close(dataFd);
        dataPtr = nullptr;
        return;
    }

}


void SharedMemoryManager::accessSharedStatus(bool create){

    statusFd = open(SHARED_MEM_STATUS, (create ? (O_CREAT | O_RDWR) : O_RDWR), 0666);
    if(statusFd < 0){
        cerr << "Failed to open shared memory file" << endl;
        return;
    }

    if(create && ftruncate(statusFd, statusSize) < 0){
        cerr << "File truncate failed" << endl;
        close(statusFd);
        return;
    }

    statusPtr = mmap(nullptr, statusSize, PROT_READ | PROT_WRITE, MAP_SHARED, statusFd, 0);
    if(statusPtr == MAP_FAILED){
        cerr << "Failed to map memory" << endl;
        close(statusFd);
        statusPtr = nullptr;
        return;
    }

}


SharedMemoryManager::~SharedMemoryManager(){
    if(dataPtr){
        munmap(dataPtr, dataSize);
    }

    if(dataFd >= 0){
        close(dataFd);
    }


    if(statusPtr){
        munmap(statusPtr, statusSize);
    }

    if(statusFd >= 0){
        close(statusFd);
    }
}
