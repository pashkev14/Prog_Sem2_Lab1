/*
	Программа по обработке строки и нахождению последнего слова, начинающегося на заданный символ
	Автор: Савин Павел Алексеевич, группа 2309. Версия 1.1.1
	Даты: Начало: 07.02.23 Окончание: 14.02.23
*/

#include "files.h"
#include "process.h"

int main() {
	Files files;
	files.inFile.open("in.txt");
	files.outFile.open("out.txt");
	files.logFile.open("log.txt");
	process(&files); // собираем файлы в структуру и запускаем процесс
	files.inFile.close();
	files.outFile.close();
	files.logFile.close();
	return 0;
}