#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>
#include <cstring>
#include "parent.h"


void Parent(const char* filename, std::ostream& out_stream){
    int fd_open = open(filename, O_RDONLY);
    if (fd_open == -1) {
        std::cerr << "Ошибка открытия файла" << std::endl;
        return;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        std::cerr << "Ошибка создания канала" << std::endl;
        return;
    }

    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Ошибка создания дочернего процесса" << std::endl;
        return;
    }
    if (pid == 0) {
        close(pipefd[0]);
        if (dup2(fd_open, STDIN_FILENO) == -1) {
            std::cerr << "Ошибка перенаправления дескриптора" << std::endl;
            close(fd_open);
            return;
        }
        close(fd_open);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            std::cerr << "Ошибка перенаправления в канал" << std::endl;
            close(pipefd[1]);
            return;
        }
        close(pipefd[1]);
        execl("./child", "./child", NULL);
        std::cerr << "Ошибка исполнения дочернего процесса" << std::endl;
        return;
    }
    else {
        close(pipefd[1]);
        char buffer[256];
        ssize_t bytesRead;
        //читаем данные из канала в buffer, bytesRead - количество считанных символов, завершаем считанные данные символом конца строоки
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '\0';
            out_stream << buffer;
        }
        close(pipefd[0]);
        wait(NULL);
    }
}
