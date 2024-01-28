#pragma once

#include <fstream>

class Files {
public:
	std::ifstream inFile;
	std::ofstream outFile;
	std::ofstream logFile;


	explicit Files(const char in[] = "in.txt", const char out[] = "out.txt", const char log[] = "log.txt") {
		inFile.open(in);
		outFile.open(out);
		logFile.open(log);
	}

	~Files() {
		inFile.close();
		outFile.close();
		logFile.close();
	}
};