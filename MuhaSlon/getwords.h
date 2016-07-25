#pragma once
#include <string>
#include <fstream>
#include <vector>

using namespace std;

// Построчное чтение текстового файла
// аргумент: имя файла
// возвращает массив строк
// В случае ошибки выбрасывает исключение
vector <string> getWords(string fName)
{
	vector<string> words;
	string word;
	ifstream fin(fName);

	if (!fin.is_open())
	{
		throw exception("File not open");
		//return words;
	}
	while (!fin.eof())
	{
		getline(fin, word);
		if (word.length() > 0) words.push_back(word);
	}
	fin.close();
	return words;
}