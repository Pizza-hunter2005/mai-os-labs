#include <iostream>
#include <fcntl.h>
#include <string>
#include <sstream>
#include <sys/mman.h>
#include <sys/wait.h>
#include "child.h"


int main(int argc, const char* argv[]) {
    if (argc != 3) {
        std::cerr << "Неверное количество аргументов" << std::endl;
        return 1;
    }
    const char* shm_name = argv[1];   
    size_t mem_size = std::stoul(argv[2]);
    int shm_fd = shm_open(shm_name, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Ошибка при открытии разделяемой памяти");
        return 1;
    }
 
    void* shared_mem = mmap(nullptr, mem_size, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shared_mem == MAP_FAILED) {
        perror("Ошибка при отображении разделяемой памяти");
        close(shm_fd);
        return 1;
    }

    close(shm_fd);

    std::istringstream input(static_cast<char*>(shared_mem));
    float sum = 0.0f;
    float num = 0.0f;
    while(input >> num) {
        sum += num;
    }
    std::cout << sum << std::endl; 
    munmap(shared_mem, mem_size);
    return 0;
}

