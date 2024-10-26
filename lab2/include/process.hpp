#ifndef PROCESS_H
#define PROCESS_H

#include "Hex512.hpp"
#include <string>

Hex512 average(int num_threads, const std::string& filename, const size_t& max_memory_mb);

#endif