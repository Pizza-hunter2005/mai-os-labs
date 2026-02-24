#include <chrono>
#include <iostream>
#include "Hex512.hpp"
#include "process.hpp"

int main(int argc, char* argv[]) {
    auto start_of_program = std::chrono::high_resolution_clock::now();
    if (argc != 4) {
        perror("Wrong arguments!");
        return 1;
    }

    std::string filename = argv[2];
    int num_threads = std::stoi(argv[1]);
    size_t max_memory_mb = std::stoull(argv[3]);
    std::cout << "Количество потоков: " << num_threads;
    std::cout << "\nОграничение оперативной памяти: " << max_memory_mb;
    std::cout << "\nИмя файла с числами: " << filename;

    Hex512 result = average(num_threads, filename, max_memory_mb);
    std::cout << "\nСреднее арифметическое: ";
    result.print_num();

    auto end_of_program = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> duration = end_of_program - start_of_program;
    std::cout << "Время выполнения: " << duration.count() << " микросекунд" << std::endl;
    return 0;
}