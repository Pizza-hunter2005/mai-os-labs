#include <cstdlib>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "parent.h"


void Parent(const std::string& filename, std::ostream &output){
    int fd_open = open(filename.c_str(), O_RDONLY);
    if (fd_open == -1) {
        perror("Ошибка открытия файла");
        return;
    }
    struct stat info;
    if(fstat(fd_open, &info) == -1) {
        perror("Ошибка получения информации о файле");
        close(fd_open);
        return;
    }
    std::string shm_name = "/shared_memory_" + std::to_string(getpid());;
    const size_t mem_size = info.st_size + 256;

    int shm_fd = shm_open(shm_name.c_str(), O_CREAT | O_RDWR, 0666);
    if(shm_fd == -1) {
        perror("Ошибка при создании разделяемой памяти");
        close(fd_open);
        return;
    }

    if(ftruncate(shm_fd, mem_size)) {
        perror("Ошибка выделения памяти shm");
        shm_unlink(shm_name.c_str());
        close(fd_open);
        return;
    }

    void* shared_mem = mmap(nullptr, mem_size, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shared_mem == MAP_FAILED) {
        perror("Ошибка отображения файла в память");
        shm_unlink(shm_name.c_str());
        close(fd_open);
        return;
    }
    if (pread(fd_open, shared_mem, mem_size, 0) == -1) {
        perror("Ошибка чтения данных в разделяемую память");
        munmap(shared_mem, mem_size);
        shm_unlink(shm_name.c_str());
        close(fd_open);
        return;
    }
    close(fd_open);
    
    pid_t pid = fork();
    if (pid == -1) {
        perror("Ошибка создания дочернего процесса");
        munmap(shared_mem, mem_size);
        shm_unlink(shm_name.c_str());
        return;
    }
    if (pid == 0) {
        std::string mem_size_str = std::to_string(mem_size);
        execl("./child", "./child",shm_name.c_str(), mem_size_str.c_str(), nullptr);
        perror("Ошибка исполнения дочернего процесса");
        munmap(shared_mem, mem_size);
        return;
    }
    else {
        wait(nullptr);
        char* result = static_cast<char*>(shared_mem) + info.st_size;
        output << result;
        munmap(shared_mem, mem_size);
        shm_unlink(shm_name.c_str());
    }
}
