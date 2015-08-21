#pragma once

#include <string>
#include <vector>

struct LibraryBinary {
	std::string path;
	std::string arch;
	std::string target;

	static const std::vector<std::string> archs;
	static const std::vector<std::string> targets;
};