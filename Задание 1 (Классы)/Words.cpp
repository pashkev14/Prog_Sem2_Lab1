#include "Words.h"
#include "Files.h"

void Words::solve(int size, char key, Files* fileObj) {
	fileObj->logFile << "Производится попытка решить задачу - найти в предложении последнее слово, начинающееся на ключевой символ.\n";
	int i = size - 1;
	while (i >= 0 && this->words[i].getChar(0) != key) --i;
	if (i == -1) {
		fileObj->logFile << "В предложении не было обнаружено ни одного слова, начинающегося на заданный символ. Ответ не получен.\nПрограмма завершила свою работу.";
		fileObj->outFile << "Ответ не получен: в предолжении нет слова, начинающегося на заданный символ.";
	}
	else {
		fileObj->outFile << "Последнее слово в предложении, оканчивающееся на символ '" << key << "' - ";
		fileObj->logFile << "Найдено слово-ответ на задачу: ";
		for (int j = 0; j < this->words[i].getSize(); ++j) {
			fileObj->logFile << this->words[i].getChar(j);
			fileObj->outFile << this->words[i].getChar(j);
		}
		fileObj->logFile << "\nПрограмма завершила свою работу.";
	}
}

void Words::readInStopMode(Files* fileObj) {
	/* Условия корректности символов:
	*  1. Остановка и ключ не могут быть пробелом (пробел зарезервирован под разделение слов)
	*  2. Остановка и ключ не могут быть одним и тем же символом (иначе вся система со словами и предложением рухнет)
	*/
	fileObj->logFile << "Режим чтения: по стоп-символу.\nПроизводится попытка определить стоп-символ.\n";
	fileObj->inFile.unsetf(std::ios::skipws);
	char tmp, stop, key;
	fileObj->inFile >> tmp;
	fileObj->inFile >> stop;
	if (!fileObj->inFile.eof() && stop != ' ') {
		fileObj->logFile << "Определен стоп-символ - '" << stop << "'.\nПроизводится попытка определить ключевой символ.\n";
		fileObj->inFile >> tmp;
		fileObj->inFile >> key;
		if (!fileObj->inFile.eof() && key != ' ' && key != stop) { // спецсимволы корректны
			// работаем по отработанной схеме считывания
			fileObj->logFile << "Определен ключевой символ - '" << key << "'.\nПроизводится попытка считать строку.\n";
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
			if (wordsCount) { // если слова есть, мероприятие имеет смысл, записываем
				fileObj->inFile.clear();
				fileObj->inFile.seekg(startPos - 1, std::ios::beg);
				fileObj->inFile >> tmp;
				this->words = new Word[wordsCount];
				char* tmpText = NULL;
				int curPos = fileObj->inFile.tellg(), charsCount = 0, curIndex = 0;
				while (true) {
					if (fileObj->inFile.eof() || tmp == stop || tmp == '\n') {
						if (fileObj->inFile.eof()) {
							fileObj->logFile << "Файл закончился до стоп-символа, однако предложение все равно будет обработано.\n";
						}
						else if (tmp == '\n') {
							fileObj->logFile << "Предложение дошло до перехода на новую строку; обработке подлежит только часть до перехода.\n";
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
				fileObj->logFile << "В массив были записаны следующие слова:\n";
				for (int i = 0; i < wordsCount; ++i) {
					for (int j = 0; j < this->words[i].getSize(); ++j) {
						fileObj->logFile << this->words[i].getChar(j);
					}
					fileObj->logFile << '\n';
				}
				solve(wordsCount, key, fileObj); // после записи отправляем на решение задачи; в стерильных условиях это конечный пункт
				// после отработки нужно освободить выделенную память
				delete[] tmpText;
			}
			else {
				fileObj->outFile << "Ответ не получен: в файле не было обнаружено ни одного слова. (слово - непрерывная последовательность символов, ограниченная пробелами)";
				fileObj->logFile << "В файле не было обнаружено ни одного слова. Ответ не получен.\nПрограмма завершила свою работу.";
			}
		}
		// выписываем причины остановки
		else if (fileObj->inFile.eof()) {
			fileObj->outFile << "Ошибка: файл преждевременно достиг своего конца. (попытка определить ключевой символ)";
			fileObj->logFile << "Файл достиг своего преждевременного конца.\nПрограмма завершила свою работу.";
		}
		else if (key == ' ') {
			fileObj->logFile << "Считанный ключевой символ является пробельным символом - недопустимый символ.\nПрограмма завершила свою работу.";
			fileObj->outFile << "Ошибка: в качестве ключевого символа был указан пробельный символ - недопустимый символ!";
		}
		else if (key == stop) {
			fileObj->logFile << "Считанный ключевой символ ранее уже был зарезервирован под стоп-символ - ошибка.\nПрограмма завершила свою работу.";
			fileObj->outFile << "Ошибка: стоп-символ и ключевой символ совпадают.";
		}
	}
	// выписываем причины остановки
	else if (fileObj->inFile.eof()) {
		fileObj->logFile << "Файл достиг своего преждевременного конца.\nПрограмма завершила свою работу.";
		fileObj->outFile << "Ошибка: файл преждевременно достиг своего конца. (попытка определить стоп-символ)";
	}
	else if (stop == ' ') {
		fileObj->outFile << "Ошибка: в качестве стоп-символа был указан пробельный символ - недопустимый символ!";
		fileObj->logFile << "Считанный стоп-символ является пробельным символом - недопустимый символ.\nПрограмма завершила свою работу.";
	}
}

void Words::readInLimitMode(Files* fileObj) {
	/* Условия корректного считывания:
	*  1. Считанное число символов -- положительное число, а не что-то другое
	*  2. Квота по заказанному числу символов выполнена
	*/
	fileObj->logFile << "Режим чтения: по заданному числу символов.\nПроизводится попытка определить число символов.\n";
	int charsCount;
	char tmp, key;
	fileObj->inFile >> charsCount;
	if (!fileObj->inFile.eof() && charsCount > 0) {
		fileObj->logFile << "Определенно заданное кол-во символов - " << charsCount << ".\nПроизводится попытка считать ключевой символ.\n";
		fileObj->inFile.unsetf(std::ios::skipws);
		fileObj->inFile >> tmp;
		fileObj->inFile >> key;
		if (!fileObj->inFile.eof() && key != ' ') {
			fileObj->logFile << "Определен ключевой символ - '" << key << "'.\nПроизводится попытка считать строку.\n";
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
			if (curCount < charsCount) fileObj->logFile << "До конца предложения было считано меньше символов, чем задано.\nПрограмма обработает все, что было считано.\n";
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
					fileObj->logFile << "В массив были записаны следующие слова:\n";
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
					fileObj->outFile << "Ответ не получен: в файле не было обнаружено ни одного слова. (слово - непрерывная последовательность символов, ограниченная пробелами)";
					fileObj->logFile << "В файле не было обнаружено ни одного слова. Ответ не получен.\nПрограмма завершила свою работу.";
				}
			}
			else {
				fileObj->outFile << "Ответ не получен: кол-во корректно считанных символов не соответствует заданному.\nСтрока читается до перехода на новую строку.";
				fileObj->logFile << "Кол-во корректно считанных символов не соответствует заданному. Ответ не получен.\nПрограмма завершила свою работу.";
			}
			delete[] chars;
		}
		else if (fileObj->inFile.eof()) {
			fileObj->outFile << "Ошибка: файл преждевременно достиг своего конца. (попытка определить ключевой символ)";
			fileObj->logFile << "Файл преждевременно достиг своего конца.\nПрограмма завершила свою работу.";
		}
		else if (key == ' ') {
			fileObj->logFile << "Считанный ключевой символ является пробельным символом - недопустимый символ.\nПрограмма завершила свою работу.";
			fileObj->outFile << "Ошибка: в качестве ключевого символа был указан пробельный символ - недопустимый символ!";
		}
	}
	else if (fileObj->inFile.eof()) {
		fileObj->outFile << "Ошибка: файл преждевременно достиг своего конца. (попытка определить число символов)";
		fileObj->logFile << "Файл преждевременно достиг своего конца.\nПрограмма завершила свою работу.";
	}
	else if (charsCount <= 0) {
		fileObj->outFile << "Ошибка: считанное число символов не является положительным - недопустимое значение!";
		fileObj->logFile << "Считанное число символов не является положительным - ошибка.\nПрограмма завершила свою работу.";
	}
}

void Words::process(Files* fileObj) { // запуск процесса, попытка определить режим чтения строки
	fileObj->logFile << "Программа начала свою работу.\nПроизводится попытка определить параметр чтения.\n";
	if (fileObj->inFile.is_open()) {
		char mode; // режим обозначается как 1 или 2 (для остановочного символа и заданного кол-ва символов, соответственно)
		fileObj->inFile >> mode;
		if (mode == '1') {
			this->readInStopMode(fileObj);
		}
		else if (mode == '2') {
			this->readInLimitMode(fileObj);
		}
		else { // другие символы в этой строке значат, что файл уже некорректен, нужно бросать это дело и выводить причину остановки
			fileObj->outFile << "Ошибка: указан некорректный параметр на чтение.\nНужно указать либо символ '1', либо символ '2'.";
			fileObj->logFile << "Считанный параметр не соответствует шаблонам (либо символ '1', либо '2').\nПрограмма завершила свою работу.";
		}
	}
	else {
		fileObj->outFile << "Ошибка: входной файл не найден. Укажите путь к нему в main.cpp или создайте файл in.txt в директории с программой.";
		fileObj->logFile << "Входной файл не найден.\nПрограмма завершила свою работу.";
	}
}