#include "Words.h"
#include "Files.h"

void Words::solve(int size, char key, Files* fileObj) {
	fileObj->logFile << "������������ ������� ������ ������ - ����� � ����������� ��������� �����, ������������ �� �������� ������.\n";
	int i = size - 1;
	while (i >= 0 && this->words[i].getChar(0) != key) --i;
	if (i == -1) {
		fileObj->logFile << "� ����������� �� ���� ���������� �� ������ �����, ������������� �� �������� ������. ����� �� �������.\n��������� ��������� ���� ������.";
		fileObj->outFile << "����� �� �������: � ����������� ��� �����, ������������� �� �������� ������.";
	}
	else {
		fileObj->outFile << "��������� ����� � �����������, �������������� �� ������ '" << key << "' - ";
		fileObj->logFile << "������� �����-����� �� ������: ";
		for (int j = 0; j < this->words[i].getSize(); ++j) {
			fileObj->logFile << this->words[i].getChar(j);
			fileObj->outFile << this->words[i].getChar(j);
		}
		fileObj->logFile << "\n��������� ��������� ���� ������.";
	}
}

void Words::readInStopMode(Files* fileObj) {
	/* ������� ������������ ��������:
	*  1. ��������� � ���� �� ����� ���� �������� (������ �������������� ��� ���������� ����)
	*  2. ��������� � ���� �� ����� ���� ����� � ��� �� �������� (����� ��� ������� �� ������� � ������������ ������)
	*/
	fileObj->logFile << "����� ������: �� ����-�������.\n������������ ������� ���������� ����-������.\n";
	fileObj->inFile.unsetf(std::ios::skipws);
	char tmp, stop, key;
	fileObj->inFile >> tmp;
	fileObj->inFile >> stop;
	if (!fileObj->inFile.eof() && stop != ' ') {
		fileObj->logFile << "��������� ����-������ - '" << stop << "'.\n������������ ������� ���������� �������� ������.\n";
		fileObj->inFile >> tmp;
		fileObj->inFile >> key;
		if (!fileObj->inFile.eof() && key != ' ' && key != stop) { // ����������� ���������
			// �������� �� ������������ ����� ����������
			fileObj->logFile << "��������� �������� ������ - '" << key << "'.\n������������ ������� ������� ������.\n";
			fileObj->inFile >> tmp;
			fileObj->inFile >> tmp;
			int startPos = fileObj->inFile.tellg(), charsCount = 0, wordsCount = 0;
			while (true) {
				if (fileObj->inFile.eof() || tmp == stop || tmp == '\n') {
					if (charsCount) ++wordsCount;
					break;
				}
				else if (tmp == ' ') {
					if (charsCount) ++wordsCount;
					charsCount = 0;
				}
				else ++charsCount;
				fileObj->inFile >> tmp;
			}
			if (wordsCount) { // ���� ����� ����, ����������� ����� �����, ����������
				fileObj->inFile.clear();
				fileObj->inFile.seekg(startPos - 1, std::ios::beg);
				fileObj->inFile >> tmp;
				this->words = new Word[wordsCount];
				char* tmpText = NULL;
				int curPos = fileObj->inFile.tellg(), charsCount = 0, curIndex = 0;
				while (true) {
					if (fileObj->inFile.eof() || tmp == stop || tmp == '\n') {
						if (fileObj->inFile.eof()) {
							fileObj->logFile << "���� ���������� �� ����-�������, ������ ����������� ��� ����� ����� ����������.\n";
						}
						else if (tmp == '\n') {
							fileObj->logFile << "����������� ����� �� �������� �� ����� ������; ��������� �������� ������ ����� �� ��������.\n";
						}
						if (charsCount) {
							delete[] tmpText;
							fileObj->inFile.seekg(curPos - 1, std::ios::beg);
							tmpText = new char[charsCount];
							for (int i = 0; i < charsCount; ++i) fileObj->inFile >> tmpText[i];
							this->words[curIndex].setText(tmpText, charsCount);
							++curIndex;
						}
						break;
					}
					else if (tmp == ' ') {
						if (charsCount) {
							delete[] tmpText;
							fileObj->inFile.seekg(curPos - 1, std::ios::beg);
							tmpText = new char[charsCount];
							for (int i = 0; i < charsCount; ++i) fileObj->inFile >> tmpText[i];
							this->words[curIndex].setText(tmpText, charsCount);
							++curIndex;
						}
						charsCount = 0;
					}
					else {
						if (!charsCount) {
							curPos = fileObj->inFile.tellg();
						}
						++charsCount;
					}
					fileObj->inFile >> tmp;
				}
				fileObj->logFile << "� ������ ���� �������� ��������� �����:\n";
				for (int i = 0; i < wordsCount; ++i) {
					for (int j = 0; j < this->words[i].getSize(); ++j) {
						fileObj->logFile << this->words[i].getChar(j);
					}
					fileObj->logFile << '\n';
				}
				solve(wordsCount, key, fileObj); // ����� ������ ���������� �� ������� ������; � ���������� �������� ��� �������� �����
				// ����� ��������� ����� ���������� ���������� ������
				delete[] tmpText;
			}
			else {
				fileObj->outFile << "����� �� �������: � ����� �� ���� ���������� �� ������ �����. (����� - ����������� ������������������ ��������, ������������ ���������)";
				fileObj->logFile << "� ����� �� ���� ���������� �� ������ �����. ����� �� �������.\n��������� ��������� ���� ������.";
			}
		}
		// ���������� ������� ���������
		else if (fileObj->inFile.eof()) {
			fileObj->outFile << "������: ���� �������������� ������ ������ �����. (������� ���������� �������� ������)";
			fileObj->logFile << "���� ������ ������ ���������������� �����.\n��������� ��������� ���� ������.";
		}
		else if (key == ' ') {
			fileObj->logFile << "��������� �������� ������ �������� ���������� �������� - ������������ ������.\n��������� ��������� ���� ������.";
			fileObj->outFile << "������: � �������� ��������� ������� ��� ������ ���������� ������ - ������������ ������!";
		}
		else if (key == stop) {
			fileObj->logFile << "��������� �������� ������ ����� ��� ��� �������������� ��� ����-������ - ������.\n��������� ��������� ���� ������.";
			fileObj->outFile << "������: ����-������ � �������� ������ ���������.";
		}
	}
	// ���������� ������� ���������
	else if (fileObj->inFile.eof()) {
		fileObj->logFile << "���� ������ ������ ���������������� �����.\n��������� ��������� ���� ������.";
		fileObj->outFile << "������: ���� �������������� ������ ������ �����. (������� ���������� ����-������)";
	}
	else if (stop == ' ') {
		fileObj->outFile << "������: � �������� ����-������� ��� ������ ���������� ������ - ������������ ������!";
		fileObj->logFile << "��������� ����-������ �������� ���������� �������� - ������������ ������.\n��������� ��������� ���� ������.";
	}
}

void Words::readInLimitMode(Files* fileObj) {
	/* ������� ����������� ����������:
	*  1. ��������� ����� �������� -- ������������� �����, � �� ���-�� ������
	*  2. ����� �� ����������� ����� �������� ���������
	*/
	fileObj->logFile << "����� ������: �� ��������� ����� ��������.\n������������ ������� ���������� ����� ��������.\n";
	int charsCount;
	char tmp, key;
	fileObj->inFile >> charsCount;
	if (!fileObj->inFile.eof() && charsCount > 0) {
		fileObj->logFile << "����������� �������� ���-�� �������� - " << charsCount << ".\n������������ ������� ������� �������� ������.\n";
		fileObj->inFile.unsetf(std::ios::skipws);
		fileObj->inFile >> tmp;
		fileObj->inFile >> key;
		if (!fileObj->inFile.eof() && key != ' ') {
			fileObj->logFile << "��������� �������� ������ - '" << key << "'.\n������������ ������� ������� ������.\n";
			fileObj->inFile >> tmp;
			char* chars = new char[charsCount];
			int curCount = 0;
			do {
				if (curCount == charsCount) {
					break;
				}
				fileObj->inFile >> tmp;
				if (fileObj->inFile.eof() || tmp == '\n') break;
				else {
					chars[curCount] = tmp;
					++curCount;
				}
			} 
			while (true);
			if (curCount < charsCount) fileObj->logFile << "�� ����� ����������� ���� ������� ������ ��������, ��� ������.\n��������� ���������� ���, ��� ���� �������.\n";
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
					this->words = new Word[wordsCount];
					char* tmpText = NULL;
					int curPos = 0, charsCount = 0, startPos = 0, curIndex = 0;
					while (true) {
						if (curPos == curCount) {
							if (charsCount) {
								delete[] tmpText;
								tmpText = new char[charsCount];
								curPos = startPos;
								int i = 0;
								while (i < charsCount) {
									tmpText[i] = chars[curPos];
									++i; ++curPos;
								}
								this->words[curIndex].setText(tmpText, charsCount);
								++curIndex;
							}
							break;
						}
						else if (chars[curPos] == ' ') {
							if (charsCount) {
								delete[] tmpText;
								tmpText = new char[charsCount];
								curPos = startPos;
								int i = 0;
								while (i < charsCount) {
									tmpText[i] = chars[curPos];
									++i; ++curPos;
								}
								this->words[curIndex].setText(tmpText, charsCount);
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
					fileObj->logFile << "� ������ ���� �������� ��������� �����:\n";
					for (int i = 0; i < wordsCount; ++i) {
						for (int j = 0; j < this->words[i].getSize(); ++j) {
							fileObj->logFile << this->words[i].getChar(j);
						}
						fileObj->logFile << '\n';
					}
					solve(wordsCount, key, fileObj);
					delete[] tmpText;
				}
				else {
					fileObj->outFile << "����� �� �������: � ����� �� ���� ���������� �� ������ �����. (����� - ����������� ������������������ ��������, ������������ ���������)";
					fileObj->logFile << "� ����� �� ���� ���������� �� ������ �����. ����� �� �������.\n��������� ��������� ���� ������.";
				}
			}
			else {
				fileObj->outFile << "����� �� �������: ���-�� ��������� ��������� �������� �� ������������� ���������.\n������ �������� �� �������� �� ����� ������.";
				fileObj->logFile << "���-�� ��������� ��������� �������� �� ������������� ���������. ����� �� �������.\n��������� ��������� ���� ������.";
			}
			delete[] chars;
		}
		else if (fileObj->inFile.eof()) {
			fileObj->outFile << "������: ���� �������������� ������ ������ �����. (������� ���������� �������� ������)";
			fileObj->logFile << "���� �������������� ������ ������ �����.\n��������� ��������� ���� ������.";
		}
		else if (key == ' ') {
			fileObj->logFile << "��������� �������� ������ �������� ���������� �������� - ������������ ������.\n��������� ��������� ���� ������.";
			fileObj->outFile << "������: � �������� ��������� ������� ��� ������ ���������� ������ - ������������ ������!";
		}
	}
	else if (fileObj->inFile.eof()) {
		fileObj->outFile << "������: ���� �������������� ������ ������ �����. (������� ���������� ����� ��������)";
		fileObj->logFile << "���� �������������� ������ ������ �����.\n��������� ��������� ���� ������.";
	}
	else if (charsCount <= 0) {
		fileObj->outFile << "������: ��������� ����� �������� �� �������� ������������� - ������������ ��������!";
		fileObj->logFile << "��������� ����� �������� �� �������� ������������� - ������.\n��������� ��������� ���� ������.";
	}
}

void Words::process(Files* fileObj) { // ������ ��������, ������� ���������� ����� ������ ������
	fileObj->logFile << "��������� ������ ���� ������.\n������������ ������� ���������� �������� ������.\n";
	if (fileObj->inFile.is_open()) {
		char mode; // ����� ������������ ��� 1 ��� 2 (��� ������������� ������� � ��������� ���-�� ��������, ��������������)
		fileObj->inFile >> mode;
		if (mode == '1') {
			this->readInStopMode(fileObj);
		}
		else if (mode == '2') {
			this->readInLimitMode(fileObj);
		}
		else { // ������ ������� � ���� ������ ������, ��� ���� ��� �����������, ����� ������� ��� ���� � �������� ������� ���������
			fileObj->outFile << "������: ������ ������������ �������� �� ������.\n����� ������� ���� ������ '1', ���� ������ '2'.";
			fileObj->logFile << "��������� �������� �� ������������� �������� (���� ������ '1', ���� '2').\n��������� ��������� ���� ������.";
		}
	}
	else {
		fileObj->outFile << "������: ������� ���� �� ������. ������� ���� � ���� � main.cpp ��� �������� ���� in.txt � ���������� � ����������.";
		fileObj->logFile << "������� ���� �� ������.\n��������� ��������� ���� ������.";
	}
}