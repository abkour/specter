#pragma once 
#include <string>
#include <string_view>

namespace specter::filesystem {

std::string get_file_extension(const std::string& filename) {
    auto dot_pos = filename.find_last_of('.');
    return filename.substr(dot_pos + 1);
}

}