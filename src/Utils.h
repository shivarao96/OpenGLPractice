#pragma once
#include <sys/stat.h>
#include <iostream>

namespace Utils {
	bool checkFile(const char* path) {
		struct stat buffer;
		if (stat(path, &buffer) != 0) {
			std::cout << "File doesn't exist, please provide proper file path::" << path << std::endl;
			return false;
		}
		return true;
	}
};