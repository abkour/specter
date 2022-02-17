#pragma once
#include <string>

namespace specter {

std::string_view getDirectoryName(std::string_view& view);
std::string_view getExtensionName(std::string_view& view);
std::string_view getFileName(std::string_view& view);

}