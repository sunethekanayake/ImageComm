
import mmap
import os
import time
import sys
from collections import Counter

IMG_WIDTH = 4
IMG_HEIGHT = 4
PIXEL_SIZE = 3
DATA_SIZE = IMG_WIDTH * IMG_HEIGHT * (PIXEL_SIZE * 4)
SHM_DATA = "/tmp/shm_data"
SHM_STATUS = "/tmp/shm_status"

INT_SIZE = sys.getsizeof(0)
SHM_DATA_SIZE = DATA_SIZE
SHM_STATUS_SIZE = 8


def process_image():

    fd = os.open(SHM_DATA, os.O_RDWR)
    print(SHM_DATA_SIZE)
    print(INT_SIZE)
    shm = mmap.mmap(fd, SHM_DATA_SIZE, access = mmap.ACCESS_WRITE)

    status_fd = os.open(SHM_STATUS, os.O_RDWR)
    status_shm = mmap.mmap(status_fd, SHM_STATUS_SIZE, access = mmap.ACCESS_WRITE)


    while True:

        shm.seek(0)
        image_data = shm[11:DATA_SIZE]

        print(image_data)
        status = status_shm[:SHM_STATUS_SIZE]

        common_colos = [0] * (PIXEL_SIZE * IMG_HEIGHT)
        print("Data", status)
        if status[0] == 1:
            print("Data available")
            for row in range(IMG_HEIGHT):
                start = row * IMG_WIDTH * PIXEL_SIZE * 4
                end = start + IMG_WIDTH * PIXEL_SIZE * 4
                row_data = image_data[start : end]
                print(row_data)
                color_counts = Counter(row_data[i : i + PIXEL_SIZE] for i in range(0, len(row_data), PIXEL_SIZE))
                most_common_color = color_counts.most_common(1)[0][0]
                # print(most_common_color)
                for j in range(PIXEL_SIZE):
                    common_colos[row * PIXEL_SIZE + j] = most_common_color[j]
                    # print()
                    print(common_colos[row * PIXEL_SIZE + j], end=" ")
                print(" ")
            # status[1] = 1
            # status[0] = 0
            # print(common_colos)
            break

        time.sleep(1)

    shm.close()
    os.close(fd)
    status_shm.close()
    os.close(status_fd)


if __name__ == '__main__':
    process_image()
