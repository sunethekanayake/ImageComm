#include <iostream>
#include "common.h"
#include "SharedMemoryManager.hpp"
#include "ImageProcessor.hpp"
#include "Communicator.hpp"
#include <thread>

using namespace std;

void runApp(Communicator* communicator){
    communicator->run();
}

int main(){

    SharedMemoryManager shmMan(DATA_SHM_SIZE, STATUS_SHM_SIZE);
    ImageProcessor imageProcessor(shmMan);

    Communicator communicator(shmMan);
    communicator.registerSendCallback([&imageProcessor](){
        imageProcessor.sendImageToMem();
    });

    communicator.registerReceiveCallback([&imageProcessor](){
        imageProcessor.receiveFromSharedMem();
    });

    thread listener(runApp, &communicator);
    listener.join();


    return 0;
}