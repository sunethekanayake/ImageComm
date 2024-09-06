#pragma once

const size_t IMAGE_HEIGHT = 4;
const size_t IMAGE_WIDTH = 4;
const size_t PIXEL_SIZE = 3;
const size_t IMAGE_SIZE = IMAGE_WIDTH * IMAGE_HEIGHT * PIXEL_SIZE;

const size_t STATUS_SHM_SIZE = 2 * sizeof(int);
const size_t DATA_SHM_SIZE = IMAGE_SIZE * sizeof(int);
const size_t RX_DATA_SHM_SIZE = sizeof(int);

const char* SHARED_MEM_DATA = "/tmp/shm_data";
const char* SHARED_MEM_STATUS = "/tmp/shm_status";
