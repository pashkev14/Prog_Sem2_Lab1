#include "process.h"
#include "word.h"

void solve(Word* words, int size, char key, Files* files) {
	files->logFile << "Производится попытка решить задачу - найти в предложении последнее слово, начинающееся на ключевой символ.\n";
	int i = size - 1;
	while (i >= 0 && words[i].text[0] != key) --i;
	if (i == -1) {
		files->logFile << "В предложении не было обнаружено ни одного слова, начинающегося на заданный символ. Ответ не получен.\nПрограмма завершила свою работу.";
		files->outFile << "Ответ не получен: в предолжении нет слова, начинающегося на заданный символ.";
	}
	else {
		files->outFile << "Последнее слово в предложении, оканчивающееся на символ '" << key << "' - ";
		files->logFile << "Найдено слово-ответ на задачу: ";
		int j = 0;
		while (words[i].text[j] != words[i].mark) {
			files->logFile << words[i].text[j];
			files->outFile << words[i].text[j];
			++j;
		}
		files->logFile << "\nПрограмма завершила свою работу.";
	}
	
}

void readInStopMode(Files* files) {
	/* Условия корректности символов:
	*  1. Остановка и ключ не могут быть пробелом (пробел зарезервирован под разделение слов)
	*  2. Остановка и ключ не могут быть одним и тем же символом (иначе вся система со словами и предложением рухнет)
	*/
	files->logFile << "Режим чтения: по стоп-символу.\nПроизводится попытка определить стоп-символ.\n";
	files->inFile.unsetf(std::ios::skipws);
	char tmp, stop, key;
	files->inFile >> tmp;
	files->inFile >> stop;
	if (!files->inFile.eof() && stop != ' ') {
		files->logFile << "Определен стоп-символ - '" << stop << "'.\nПроизводится попытка определить ключевой символ.\n";
		files->inFile >> tmp;
		files->inFile >> key;
		if (!files->inFile.eof() && key != ' ' && key != stop) { // спецсимволы корректны
			// работаем по отработанной схеме считывания
			files->logFile << "Определен ключевой символ - '" << key << "'.\nПроизводится попытка считать строку.\n";
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
			if (wordsCount) { // если слова есть, мероприятие имеет смысл, записываем
				files->inFile.clear();
				files->inFile.seekg(startPos - 1, std::ios::beg);
				files->inFile >> tmp;
				Word* words = new Word[wordsCount];
				int curPos = files->inFile.tellg(), charsCount = 0, curIndex = 0;
				while (true) {
					if (files->inFile.eof() || tmp == stop || tmp == '\n') {
						if (files->inFile.eof()) {
							files->logFile << "Файл закончился до стоп-символа, однако предложение все равно будет обработано.\n";
						}
						else if (tmp == '\n') {
							files->logFile << "Предложение дошло до перехода на новую строку; обработке подлежит только часть до перехода.\n";
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
				files->logFile << "В массив были записаны следующие слова:\n";
				for (int i = 0; i < wordsCount; ++i) {
					int j = 0;
					while (words[i].text[j] != words[i].mark) {
						files->logFile << words[i].text[j];
						++j;
					}
					files->logFile << '\n';
				}
				solve(words, wordsCount, key, files); // после записи отправляем на решение задачи; в стерильных условиях это конечный пункт
				for (int i = 0; i < wordsCount; ++i) delete[] words[i].text; // после отработки нужно освободить выделенную память
				delete[] words;
			}
			else {
				files->outFile << "Ответ не получен: в файле не было обнаружено ни одного слова. (слово - непрерывная последовательность символов, ограниченная пробелами)";
				files->logFile << "В файле не было обнаружено ни одного слова. Ответ не получен.\nПрограмма завершила свою работу.";
			}
		}
		// выписываем причины остановки
		else if (files->inFile.eof()) {
			files->outFile << "Ошибка: файл преждевременно достиг своего конца. (попытка определить ключевой символ)";
			files->logFile << "Файл достиг своего преждевременного конца.\nПрограмма завершила свою работу.";
		}
		else if (key == ' ') {
			files->logFile << "Считанный ключевой символ является пробельным символом - недопустимый символ.\nПрограмма завершила свою работу.";
			files->outFile << "Ошибка: в качестве ключевого символа был указан пробельный символ - недопустимый символ!";
		}
		else if (key == stop) {
			files->logFile << "Считанный ключевой символ ранее уже был зарезервирован под стоп-символ - ошибка.\nПрограмма завершила свою работу.";
			files->outFile << "Ошибка: стоп-символ и ключевой символ совпадают.";
		}
	}
	// выписываем причины остановки
	else if (files->inFile.eof()) {
		files->logFile << "Файл достиг своего преждевременного конца.\nПрограмма завершила свою работу.";
		files->outFile << "Ошибка: файл преждевременно достиг своего конца. (попытка определить стоп-символ)";
	}
	else if (stop == ' ') {
		files->outFile << "Ошибка: в качестве стоп-символа был указан пробельный символ - недопустимый символ!";
		files->logFile << "Считанный стоп-символ является пробельным символом - недопустимый символ.\nПрограмма завершила свою работу.";
	}
}

void readInLimitMode(Files* files) {
	/* Условия корректного считывания:
	*  1. Считанное число символов -- положительное число, а не что-то другое
	*  2. Квота по заказанному числу символов выполнена
	*/
	files->logFile << "Режим чтения: по заданному числу символов.\nПроизводится попытка определить число символов.\n";
	int charsCount;
	char tmp, key;
	files->inFile >> charsCount;
	if (!files->inFile.eof() && charsCount > 0) {
		files->logFile << "Определенно заданное кол-во символов - " << charsCount << ".\nПроизводится попытка считать ключевой символ.\n";
		files->inFile.unsetf(std::ios::skipws);
		files->inFile >> tmp;
		files->inFile >> key;
		if (!files->inFile.eof() && key != ' ') {
			files->logFile << "Определен ключевой символ - '" << key << "'.\nПроизводится попытка считать строку.\n";
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
			if (curCount < charsCount) files->logFile << "До конца предложения было считано меньше символов, чем задано.\nПрограмма обработает все, что было считано.\n";
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
					files->logFile << "В массив были записаны следующие слова:\n";
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
				files->outFile << "Ответ не получен: кол-во корректно считанных символов не соответствует заданному.\nСтрока читается до перехода на новую строку.";
				files->logFile << "Кол-во корректно считанных символов не соответствует заданному. Ответ не получен.\nПрограмма завершила свою работу.";
			}
			delete[] chars;
		}
		else if (files->inFile.eof()) {
			files->outFile << "Ошибка: файл преждевременно достиг своего конца. (попытка определить ключевой символ)";
			files->logFile << "Файл преждевременно достиг своего конца.\nПрограмма завершила свою работу.";
		}
		else if (key == ' ') {
			files->logFile << "Считанный ключевой символ является пробельным символом - недопустимый символ.\nПрограмма завершила свою работу.";
			files->outFile << "Ошибка: в качестве ключевого символа был указан пробельный символ - недопустимый символ!";
		}
	}
	else if (files->inFile.eof()) {
		files->outFile << "Ошибка: файл преждевременно достиг своего конца. (попытка определить число символов)";
		files->logFile << "Файл преждевременно достиг своего конца.\nПрограмма завершила свою работу.";
	}
	else if (charsCount <= 0) {
		files->outFile << "Ошибка: считанное число символов не является положительным - недопустимое значение!";
		files->logFile << "Считанное число символов не является положительным - ошибка.\nПрограмма завершила свою работу.";
	}
}

void process(Files* files) { // запуск процесса, попытка определить режим чтения строки
	if (files->inFile.is_open()) {
		files->logFile << "Программа начала свою работу.\nПроизводится попытка определить параметр чтения.\n";
		char mode; // режим обозначается как 1 или 2 (для остановочного символа и заданного кол-ва символов, соответственно)
		files->inFile >> mode;
		if (mode == '1') {
			readInStopMode(files);
		}
		else if (mode == '2') {
			readInLimitMode(files);
		}
		else { // другие символы в этой строке значат, что файл уже некорректен, нужно бросать это дело и выводить причину остановки
			files->outFile << "Ошибка: указан некорректный параметр на чтение.\nНужно указать либо символ '1', либо символ '2'.";
			files->logFile << "Считанный параметр не соответствует шаблонам (либо символ '1', либо '2').\nПрограмма завершила свою работу.";
		}
	}
	else {
		files->outFile << "Ошибка: входной файл не найден.";
		files->logFile << "Входной файл не найден.\nПрограмма завершила свою работу.";
	}
}