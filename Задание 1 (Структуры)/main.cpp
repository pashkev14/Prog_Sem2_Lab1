/*
	��������� �� ��������� ������ � ���������� ���������� �����, ������������� �� �������� ������
	�����: ����� ����� ����������, ������ 2309. ������ 1.1.1
	����: ������: 07.02.23 ���������: 14.02.23
*/

#include "files.h"
#include "process.h"

int main() {
	Files files;
	files.inFile.open("in.txt");
	files.outFile.open("out.txt");
	files.logFile.open("log.txt");
	process(&files); // �������� ����� � ��������� � ��������� �������
	files.inFile.close();
	files.outFile.close();
	files.logFile.close();
	return 0;
}