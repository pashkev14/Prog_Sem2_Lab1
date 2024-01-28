#pragma once

class Word {
private:
	char* m_text;
	int m_size;

	void setSize(const int size);

public:
	explicit Word(const char text[] = "", const int size = 1) {
		this->m_text = new char[size];
		for (int i = 0; i < size; ++i) this->m_text[i] = text[i];
		this->setSize(size);
	}

	~Word() {
		delete[] m_text;
	}

	void setText(const char text[], const int size);

	const char getChar(int index);

	const int getSize();
};