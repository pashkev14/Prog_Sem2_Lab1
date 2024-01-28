#include "Word.h"

void Word::setSize(const int size) {
	this->m_size = size;
}

void Word::setText(const char text[], const int size) {
	delete[] this->m_text;
	this->m_text = new char[size];
	for (int i = 0; i < size; ++i) this->m_text[i] = text[i];
	this->setSize(size);
}

const char Word::getChar(int index) {
	if (index >= 0 && index < this->m_size) return this->m_text[index];
}

const int Word::getSize() {
	return this->m_size;
}