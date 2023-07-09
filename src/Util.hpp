#pragma once
#include <string>
#include <fstream>
#include <sstream>

inline auto read_file_to_string(const std::string& path) -> std::string {
    std::fstream file_in(path);

    if(!file_in.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    std::stringstream buffer;
    buffer << file_in.rdbuf();

    if(buffer.str().empty())
        throw std::runtime_error("File is empty: " + path);

    return buffer.str();
}