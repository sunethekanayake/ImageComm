#pragma once

#include <iostream>
#include <fstream>
#include "common.h"
#include "SharedMemoryManager.hpp"
// #include <opencv2/opencv.hpp>   // for my testing

using namespace std;


class ImageProcessor{

public:
    ImageProcessor(SharedMemoryManager& shmm):shmMan(shmm){}

    void generateImage(const string& path);
    void sendImageToMem();
    void receiveFromSharedMem();
    void displayImage(const char* path);

private:

    SharedMemoryManager& shmMan;
    string imgPath;

    void readFromFile(const string& path, void* dest, size_t size);
    

};


void ImageProcessor::generateImage(const string& path){
    ofstream image;
    imgPath = path;
    image.open(imgPath);

    if(!image.is_open()){
        cerr << "ERROR in generating image" << endl;
        return;
    }

    //header information
    image << "P3" << endl;
    image << IMAGE_HEIGHT << " " << IMAGE_WIDTH << endl;
    image << "255" << endl;

    // PIXEL_SIZE value generation
    srand(0);
    for(int height = 0; height < IMAGE_HEIGHT; height++){
        for(int width = 0; width < IMAGE_WIDTH; width++){
            image << std::setw(3) << std::setfill('0') << rand() % 256 << " "
                  << std::setw(3) << std::setfill('0') << rand() % 256 << " "
                  << std::setw(3) << std::setfill('0') << rand() % 256 << std::endl;
        }
    }
    image.close();
}


void ImageProcessor::readFromFile(const string& path, void* dest, size_t size){

    ifstream imageSrc(path, ios::binary);
    if(!imageSrc.is_open()){
        cerr << "Failed top open image source" << endl;
        return;
    }

    imageSrc.read((char*)dest, DATA_SHM_SIZE);
    imageSrc.close();

}


void ImageProcessor::sendImageToMem(){

    void* dataPtr = shmMan.getDataPtr();
    if(!dataPtr)
        return;
    readFromFile(imgPath, dataPtr, shmMan.getDataSize());

    //set status

}


void ImageProcessor::receiveFromSharedMem(){
    void* dataPtr = shmMan.getDataPtr();
    if(!dataPtr)
        return;

    // casting has to be changed according to file type
    int* count = reinterpret_cast<int*>(dataPtr);

    for(int i = 0; i < IMAGE_HEIGHT; i++){
        cout << "Value " << count[PIXEL_SIZE * i + 0] << 
                            count[PIXEL_SIZE * i + 1] <<
                            count[PIXEL_SIZE * i + 2] << endl;
    }
    
    //set status

}


void ImageProcessor::displayImage(const char* path) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        std::cerr << "Failed to open shared memory file." << std::endl;
        return;
    }

    void* ptr = mmap(nullptr, IMAGE_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        std::cerr << "Failed to map memory." << std::endl;
        close(fd);
        return;
    }

    // Convert the memory-mapped data to a Mat object for displaying
    // cv::Mat image(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3, ptr);

    // // Display the image using OpenCV
    // cv::imshow("Image from Memory", image);
    // cv::waitKey(0); // Wait for a key press to close the window

    // munmap(ptr, IMAGE_SIZE);
    // close(fd);
}

