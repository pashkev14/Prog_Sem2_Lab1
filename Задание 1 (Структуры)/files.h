#pragma once

#include <fstream>

struct Files {
	std::ifstream inFile;
	std::ofstream outFile;
	std::ofstream logFile;
};