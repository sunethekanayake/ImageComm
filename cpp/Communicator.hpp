#pragma once
#include <iostream>
#include "common.h"
#include "SharedMemoryManager.hpp"
#include <chrono>
#include <thread>

using namespace std;

class Communicator{

public:
    Communicator(SharedMemoryManager& shmm) : shmMan(shmm){}
    ~Communicator(){}

    void registerSendCallback(function<void()> callback){
        sendCallback = callback;
    }
    void registerReceiveCallback(function<void()> callback){
        receiveCallback = callback;
    }

    void run();

private:
    SharedMemoryManager& shmMan;
    function<void()> sendCallback;
    function<void()> receiveCallback;
};


void Communicator::run(){
    while(true){
        if(sendCallback && shmMan.checkStatus(0)){
            sendCallback();
            shmMan.setStatus(0);
        }

        if(receiveCallback && shmMan.checkStatus(1)){
            receiveCallback();
            shmMan.setStatus(1);
        }

        this_thread::sleep_for(chrono::seconds(10));
    }
}