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
    const size_t mem_size = info.st_size;

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
    
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Ошибка создания канала");
        return;
    }
    pid_t pid = fork();
    if (pid == -1) {
        perror("Ошибка создания дочернего процесса");
        munmap(shared_mem, mem_size);
        shm_unlink(shm_name.c_str());
        close(pipefd[0]);
        close(pipefd[1]);
        return;
    }
    if (pid == 0) {
        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("Ошибка перенаправления в канал");
            munmap(shared_mem, mem_size);
            return;
        }
        close(pipefd[1]);

        std::string mem_size_str = std::to_string(mem_size);
        execl("./child", "./child",shm_name.c_str(), mem_size_str.c_str(), nullptr);
        perror("Ошибка исполнения дочернего процесса");
        munmap(shared_mem, mem_size);
        return;
    }
    else {
        close(pipefd[1]);
        char buffer[256];
        ssize_t bytesRead;
        //читаем данные из канала в buffer, bytesRead - количество считанных символов, завершаем считанные данные символом конца строоки
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '\0';
            output << buffer;
        }
        close(pipefd[0]);
        wait(nullptr);
        munmap(shared_mem, mem_size);
        shm_unlink(shm_name.c_str());
    }
}
