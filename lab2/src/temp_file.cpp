#include <cstdio> 
#include <cstdlib> 
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include "temp_file.hpp"


std::string TempFile::create_temp_file() {
    char temp_template[] = "/tmp/tempfile_XXXXXX";
    int fd = mkstemp(temp_template); 
    if (fd == -1) {
        perror("Failed to create temporary file.");
        exit(EXIT_FAILURE);
    }
    close(fd);
    return std::string(temp_template);
}
TempFile::TempFile() : file_path(create_temp_file()) {}

TempFile::~TempFile() {
        remove_temp_file();
}

void TempFile::write(const std::string& data) {
    std::ofstream file(file_path);
    if (file.is_open()) {
        file << data;
        file.close();
    } else {
        perror("Unable to open file for writing");
    }
}

void TempFile::remove_temp_file() {
    if (std::remove(file_path.c_str()) != 0) {
        perror("Error deleting file");
    }
}

std::string TempFile::get_name() const {
    return file_path;
}

