#pragma once

#include "Word.h"
#include "Files.h"

class Words {
	Word* words;

public:
	Words() {
		this->words = 0;
	}

	~Words() {
		delete[] this->words;
	}

	void solve(int size, char key, Files* fileObj);

	void readInStopMode(Files* fileObj);

	void readInLimitMode(Files* fileObj);

	void process(Files* fileObj);
};