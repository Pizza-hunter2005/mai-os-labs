#include <cstdlib>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "parent.h"


void Parent(const char* filename, const char* temp_output_filename){
    int fd_open = open(filename, O_RDONLY);
    if (fd_open == -1) {
        perror("Ошибка открытия файла");
        return;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Ошибка создания канала");
        return;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("Ошибка создания дочернего процесса");
        return;
    }
    if (pid == 0) {
        close(pipefd[0]);
        if (dup2(fd_open, STDIN_FILENO) == -1) {
            perror("Ошибка перенаправления дескриптора");
            close(fd_open);
            return;
        }
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("Ошибка перенаправления в канал");
            close(pipefd[1]);
            return;
        }
        close(pipefd[1]);
        const char* child_binary = getenv("CHILD_BINARY_PATH");
        if (child_binary == nullptr) {
            perror("Не удалось получить путь к бинарному файлу");
            return;
        }
        execl(child_binary, child_binary, nullptr);
        perror("Ошибка исполнения дочернего процесса");
        return;
    }
    else {
        close(pipefd[1]);
        // Открываем временный файл для записи
        std::ofstream temp_file(temp_output_filename);
        if (!temp_file.is_open()) {
            perror("Ошибка открытия временного файла");
            return;
        }
        char buffer[256];
        ssize_t bytesRead;
        //читаем данные из канала в buffer, bytesRead - количество считанных символов, завершаем считанные данные символом конца строоки
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '\0';
            temp_file << buffer;
            std::cout << buffer;
        }
        close(pipefd[0]);
        wait(NULL);
    }
}
