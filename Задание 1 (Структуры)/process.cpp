#include "process.h"
#include "word.h"

void solve(Word* words, int size, char key, Files* files) {
	files->logFile << "������������ ������� ������ ������ - ����� � ����������� ��������� �����, ������������ �� �������� ������.\n";
	int i = size - 1;
	while (i >= 0 && words[i].text[0] != key) --i;
	if (i == -1) {
		files->logFile << "� ����������� �� ���� ���������� �� ������ �����, ������������� �� �������� ������. ����� �� �������.\n��������� ��������� ���� ������.";
		files->outFile << "����� �� �������: � ����������� ��� �����, ������������� �� �������� ������.";
	}
	else {
		files->outFile << "��������� ����� � �����������, �������������� �� ������ '" << key << "' - ";
		files->logFile << "������� �����-����� �� ������: ";
		int j = 0;
		while (words[i].text[j] != words[i].mark) {
			files->logFile << words[i].text[j];
			files->outFile << words[i].text[j];
			++j;
		}
		files->logFile << "\n��������� ��������� ���� ������.";
	}
	
}

void readInStopMode(Files* files) {
	/* ������� ������������ ��������:
	*  1. ��������� � ���� �� ����� ���� �������� (������ �������������� ��� ���������� ����)
	*  2. ��������� � ���� �� ����� ���� ����� � ��� �� �������� (����� ��� ������� �� ������� � ������������ ������)
	*/
	files->logFile << "����� ������: �� ����-�������.\n������������ ������� ���������� ����-������.\n";
	files->inFile.unsetf(std::ios::skipws);
	char tmp, stop, key;
	files->inFile >> tmp;
	files->inFile >> stop;
	if (!files->inFile.eof() && stop != ' ') {
		files->logFile << "��������� ����-������ - '" << stop << "'.\n������������ ������� ���������� �������� ������.\n";
		files->inFile >> tmp;
		files->inFile >> key;
		if (!files->inFile.eof() && key != ' ' && key != stop) { // ����������� ���������
			// �������� �� ������������ ����� ����������
			files->logFile << "��������� �������� ������ - '" << key << "'.\n������������ ������� ������� ������.\n";
			files->inFile >> tmp;
			files->inFile >> tmp;
			int startPos = files->inFile.tellg(), charsCount = 0, wordsCount = 0;
			while (true) {
				if (files->inFile.eof() || tmp == stop || tmp == '\n') {
					if (charsCount) ++wordsCount;
					break;
				}
				else if (tmp == ' ') {
					if (charsCount) ++wordsCount;
					charsCount = 0;
				}
				else ++charsCount;
				files->inFile >> tmp;
			}
			if (wordsCount) { // ���� ����� ����, ����������� ����� �����, ����������
				files->inFile.clear();
				files->inFile.seekg(startPos - 1, std::ios::beg);
				files->inFile >> tmp;
				Word* words = new Word[wordsCount];
				int curPos = files->inFile.tellg(), charsCount = 0, curIndex = 0;
				while (true) {
					if (files->inFile.eof() || tmp == stop || tmp == '\n') {
						if (files->inFile.eof()) {
							files->logFile << "���� ���������� �� ����-�������, ������ ����������� ��� ����� ����� ����������.\n";
						}
						else if (tmp == '\n') {
							files->logFile << "����������� ����� �� �������� �� ����� ������; ��������� �������� ������ ����� �� ��������.\n";
						}
						if (charsCount) {
							files->inFile.seekg(curPos - 1, std::ios::beg);
							words[curIndex].text = new char[charsCount + 1];
							for (int i = 0; i < charsCount; ++i) files->inFile >> words[curIndex].text[i];
							words[curIndex].text[charsCount] = words[curIndex].mark = '@';
							++curIndex;
						}
						break;
					}
					else if (tmp == ' ') {
						if (charsCount) {
							files->inFile.seekg(curPos - 1, std::ios::beg);
							words[curIndex].text = new char[charsCount + 1];
							for (int i = 0; i < charsCount; ++i) files->inFile >> words[curIndex].text[i];
							words[curIndex].text[charsCount] = words[curIndex].mark = '@';
							++curIndex;
						}
						charsCount = 0;
					}
					else {
						if (!charsCount) {
							curPos = files->inFile.tellg();
						}
						++charsCount;
					}
					files->inFile >> tmp;
				}
				files->logFile << "� ������ ���� �������� ��������� �����:\n";
				for (int i = 0; i < wordsCount; ++i) {
					int j = 0;
					while (words[i].text[j] != words[i].mark) {
						files->logFile << words[i].text[j];
						++j;
					}
					files->logFile << '\n';
				}
				solve(words, wordsCount, key, files); // ����� ������ ���������� �� ������� ������; � ���������� �������� ��� �������� �����
				for (int i = 0; i < wordsCount; ++i) delete[] words[i].text; // ����� ��������� ����� ���������� ���������� ������
				delete[] words;
			}
			else {
				files->outFile << "����� �� �������: � ����� �� ���� ���������� �� ������ �����. (����� - ����������� ������������������ ��������, ������������ ���������)";
				files->logFile << "� ����� �� ���� ���������� �� ������ �����. ����� �� �������.\n��������� ��������� ���� ������.";
			}
		}
		// ���������� ������� ���������
		else if (files->inFile.eof()) {
			files->outFile << "������: ���� �������������� ������ ������ �����. (������� ���������� �������� ������)";
			files->logFile << "���� ������ ������ ���������������� �����.\n��������� ��������� ���� ������.";
		}
		else if (key == ' ') {
			files->logFile << "��������� �������� ������ �������� ���������� �������� - ������������ ������.\n��������� ��������� ���� ������.";
			files->outFile << "������: � �������� ��������� ������� ��� ������ ���������� ������ - ������������ ������!";
		}
		else if (key == stop) {
			files->logFile << "��������� �������� ������ ����� ��� ��� �������������� ��� ����-������ - ������.\n��������� ��������� ���� ������.";
			files->outFile << "������: ����-������ � �������� ������ ���������.";
		}
	}
	// ���������� ������� ���������
	else if (files->inFile.eof()) {
		files->logFile << "���� ������ ������ ���������������� �����.\n��������� ��������� ���� ������.";
		files->outFile << "������: ���� �������������� ������ ������ �����. (������� ���������� ����-������)";
	}
	else if (stop == ' ') {
		files->outFile << "������: � �������� ����-������� ��� ������ ���������� ������ - ������������ ������!";
		files->logFile << "��������� ����-������ �������� ���������� �������� - ������������ ������.\n��������� ��������� ���� ������.";
	}
}

void readInLimitMode(Files* files) {
	/* ������� ����������� ����������:
	*  1. ��������� ����� �������� -- ������������� �����, � �� ���-�� ������
	*  2. ����� �� ����������� ����� �������� ���������
	*/
	files->logFile << "����� ������: �� ��������� ����� ��������.\n������������ ������� ���������� ����� ��������.\n";
	int charsCount;
	char tmp, key;
	files->inFile >> charsCount;
	if (!files->inFile.eof() && charsCount > 0) {
		files->logFile << "����������� �������� ���-�� �������� - " << charsCount << ".\n������������ ������� ������� �������� ������.\n";
		files->inFile.unsetf(std::ios::skipws);
		files->inFile >> tmp;
		files->inFile >> key;
		if (!files->inFile.eof() && key != ' ') {
			files->logFile << "��������� �������� ������ - '" << key << "'.\n������������ ������� ������� ������.\n";
			files->inFile >> tmp;
			char* chars = new char[charsCount];
			int curCount = 0;
			do {
				if (curCount == charsCount) {
					break;
				}
				files->inFile >> tmp;
				if (files->inFile.eof() || tmp == '\n') break;
				else {
					chars[curCount] = tmp;
					++curCount;
				}
			} 
			while (true);
			if (curCount < charsCount) files->logFile << "�� ����� ����������� ���� ������� ������ ��������, ��� ������.\n��������� ���������� ���, ��� ���� �������.\n";
			if (curCount <= charsCount) {
				int wordsCount = 0, curPos = 0, charsCount = 0;
				while (true) {
					if (curPos == curCount) {
						if (charsCount) ++wordsCount;
						break;
					}
					else if (chars[curPos] == ' ') {
						if (charsCount) ++wordsCount;
						charsCount = 0;
					}
					else ++charsCount;
					++curPos;
				}
				if (wordsCount) {
					Word* words = new Word[wordsCount];
					int curPos = 0, charsCount = 0, startPos = 0, curIndex = 0;
					while (true) {
						if (curPos == curCount) {
							if (charsCount) {
								words[curIndex].text = new char[charsCount + 1];
								curPos = startPos;
								int i = 0;
								while (i < charsCount) {
									words[curIndex].text[i] = chars[curPos];
									++i; ++curPos;
								}
								words[curIndex].text[charsCount] = words[curIndex].mark = '@';
								++curIndex;
							}
							break;
						}
						else if (chars[curPos] == ' ') {
							if (charsCount) {
								words[curIndex].text = new char[charsCount + 1];
								curPos = startPos;
								int i = 0;
								while (i < charsCount) {
									words[curIndex].text[i] = chars[curPos];
									++i; ++curPos;
								}
								words[curIndex].text[charsCount] = words[curIndex].mark = '@';
								++curIndex;
							}
							charsCount = 0;
						}
						else {
							if (!charsCount) {
								startPos = curPos;
							}
							++charsCount;
						}
						++curPos;
					}
					files->logFile << "� ������ ���� �������� ��������� �����:\n";
					for (int i = 0; i < wordsCount; ++i) {
						int j = 0;
						while (words[i].text[j] != words[i].mark) {
							files->logFile << words[i].text[j];
							++j;
						}
						files->logFile << '\n';
					}
					solve(words, wordsCount, key, files);
					for (int i = 0; i < wordsCount; ++i) delete[] words[i].text;
					delete[] words;
				}
			}
			else {
				files->outFile << "����� �� �������: ���-�� ��������� ��������� �������� �� ������������� ���������.\n������ �������� �� �������� �� ����� ������.";
				files->logFile << "���-�� ��������� ��������� �������� �� ������������� ���������. ����� �� �������.\n��������� ��������� ���� ������.";
			}
			delete[] chars;
		}
		else if (files->inFile.eof()) {
			files->outFile << "������: ���� �������������� ������ ������ �����. (������� ���������� �������� ������)";
			files->logFile << "���� �������������� ������ ������ �����.\n��������� ��������� ���� ������.";
		}
		else if (key == ' ') {
			files->logFile << "��������� �������� ������ �������� ���������� �������� - ������������ ������.\n��������� ��������� ���� ������.";
			files->outFile << "������: � �������� ��������� ������� ��� ������ ���������� ������ - ������������ ������!";
		}
	}
	else if (files->inFile.eof()) {
		files->outFile << "������: ���� �������������� ������ ������ �����. (������� ���������� ����� ��������)";
		files->logFile << "���� �������������� ������ ������ �����.\n��������� ��������� ���� ������.";
	}
	else if (charsCount <= 0) {
		files->outFile << "������: ��������� ����� �������� �� �������� ������������� - ������������ ��������!";
		files->logFile << "��������� ����� �������� �� �������� ������������� - ������.\n��������� ��������� ���� ������.";
	}
}

void process(Files* files) { // ������ ��������, ������� ���������� ����� ������ ������
	if (files->inFile.is_open()) {
		files->logFile << "��������� ������ ���� ������.\n������������ ������� ���������� �������� ������.\n";
		char mode; // ����� ������������ ��� 1 ��� 2 (��� ������������� ������� � ��������� ���-�� ��������, ��������������)
		files->inFile >> mode;
		if (mode == '1') {
			readInStopMode(files);
		}
		else if (mode == '2') {
			readInLimitMode(files);
		}
		else { // ������ ������� � ���� ������ ������, ��� ���� ��� �����������, ����� ������� ��� ���� � �������� ������� ���������
			files->outFile << "������: ������ ������������ �������� �� ������.\n����� ������� ���� ������ '1', ���� ������ '2'.";
			files->logFile << "��������� �������� �� ������������� �������� (���� ������ '1', ���� '2').\n��������� ��������� ���� ������.";
		}
	}
	else {
		files->outFile << "������: ������� ���� �� ������.";
		files->logFile << "������� ���� �� ������.\n��������� ��������� ���� ������.";
	}
}