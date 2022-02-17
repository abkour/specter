#include "misc.hpp"

namespace specter {

std::string_view getDirectoryName(std::string_view& view) {
	auto pos = view.find_last_of('\\');
	return view.substr(0, pos + 1);
}

std::string_view getExtensionName(std::string_view& view) {
	auto pos = view.find_last_of('.');
	return view.substr(pos + 1);
}

std::string_view getFileName(std::string_view& view) {
	auto slash = view.find_last_of('\\');
	auto dot = view.find_last_of('.');
	return view.substr(slash + 1, dot);
}

}