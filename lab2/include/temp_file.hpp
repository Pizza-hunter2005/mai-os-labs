#ifndef TEMP_FILE_H
#define TEMP_FILE_H

#include <string>

class TempFile {
private:
    std::string file_path;
    std::string create_temp_file();
public:
    TempFile();
    ~TempFile();
    void write(const std::string& data);
    void remove_temp_file();
    std::string get_name() const;
};

#endif
