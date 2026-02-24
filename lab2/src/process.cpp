#include <fstream>
#include <pthread.h>
#include <sstream>
#include "Hex512.hpp"
#include <iostream>


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct ThreadData {
    const std::vector<Hex512>* numbers;
    Hex512* sum;
    Hex512* count;
    size_t start;
    size_t end;
};

void* process(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    Hex512 local_sum = Hex512();
    Hex512 local_count = Hex512();

    for (size_t i = data->start; i < data->end; ++i) {
        local_sum += (*data->numbers)[i];
        local_count += Hex512("1");
    }

    pthread_mutex_lock(&mutex);
    *(data->sum) += local_sum;
    *(data->count) += local_count;
    pthread_mutex_unlock(&mutex);
    return nullptr;
}

// Суммирование чисел в отдельном блоке общего массива чисел
void thread_process(std::vector<Hex512>& nums, size_t num_threads, Hex512& total_sum, Hex512& total_count) {
    // Если чисел очень мало, то эффективнее их обработать одним потоком
    size_t block_size;
    if (nums.size() <= num_threads) {
        block_size = nums.size();
        num_threads = 1;
    }
    else {block_size = nums.size() / num_threads;}
    std::vector<ThreadData> thread_data(num_threads);
    std::vector<pthread_t> threads(num_threads);

    for (int i = 0; i < num_threads; ++i) {
        size_t start = block_size * i;
        size_t end = (i == num_threads - 1 ? nums.size() : start + block_size);
        thread_data[i] = {&nums, &total_sum, &total_count, start, end};
        pthread_create(&threads[i], nullptr, process, &thread_data[i]);
    }

    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], nullptr);
    }
    nums.clear();    
}

Hex512 average(int num_threads, const std::string& filename, const size_t& max_memory_mb) {
    // Рассчитываем, сколько чисел можно считать из файла за раз, чтобы не выйти за ограничение оперативной памяти
    size_t memory_limit_bytes = max_memory_mb * 1024 * 1024;
    size_t max_numbers_in_memory = memory_limit_bytes / 128; // Если каждое число занимает 512 бит (128 байт)
    std::ifstream file(filename);
    if (!file) {
        perror("Error opening file");
        return Hex512();
    }

    Hex512 total_sum = Hex512();
    Hex512 total_count = Hex512();
    std::vector<Hex512> nums;
    nums.reserve(max_numbers_in_memory);

    std::string line;

    while(std::getline(file, line)) {
        std::istringstream iss(line);
        std::string hexnum;
        while(iss >> hexnum) {
            nums.push_back(Hex512(hexnum));
            if (nums.size() >= max_numbers_in_memory) {
                thread_process(nums, num_threads, total_sum, total_count);
            }
        }
    }
    if (!nums.empty()) {
        thread_process(nums, num_threads, total_sum, total_count);
    }
    Hex512 average = total_sum / total_count;
    return average;
}