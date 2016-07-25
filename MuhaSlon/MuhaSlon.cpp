/**************************
Решение тестового задания 
"Как из мухи сделать слона"
Выполнил Алексей Сперантов
19.07.2016
***************************/
#include "stdafx.h"
#include "getwords.h"
#include <string>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <direct.h>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

class FlyToElefant
{
public:
// поиск цепочки
	vector<string> findChain(string start, string end, unordered_set<string> &dict) 
	{
        vector<string> chain;
        chain.push_back(start);
        unordered_set<string> startWords, endWords;
        startWords.insert(start);
        endWords.insert(end);
        unordered_map<string, vector<string>> children;
        bool forward = true;
        if (searchChains(startWords, endWords, dict, children, forward))
		{
			//print(children);
            buildChain(start, end, children, chain);
		}
		return chain;
    }
private:
// двусторонний поиск
    bool searchChains(unordered_set<string>& startWords, unordered_set<string>& endWords, unordered_set<string>& dict, unordered_map<string, vector<string>>& children, bool forward) 
	{
        forward = !forward;
        if (startWords.empty()) return false;    // головной список пуст - возвращаем false
        if (startWords.size() > endWords.size()) // головной список длиннее хвостового:
            return searchChains(endWords, startWords, dict, children, forward); // меняем местами списки и вызываем функцию рекурсивно

		// вычеркиваем все слова головного списка из словаря
        for (auto it = startWords.begin(); it != startWords.end(); ++it) dict.erase(*it);
		// вычеркиваем все слова хвостового списка из словаря
        for (auto it = endWords.begin(); it != endWords.end(); ++it) dict.erase(*it);

        unordered_set<string> intermediate; // промежуточный список
        bool found = false;
        for (auto it = startWords.begin(); it != startWords.end(); ++it) // перебираем головной список
		{
			string word = *it;												// у каждого слова
            int n = word.length();		
            string temp = word;
			int p;
            for (p = 0; p < n; p++)											// меняем по очереди каждую букву
			{
				char letter = word[p];
				for (int i = 0; i < 33; i++)								// на все подряд по алфавиту, Ё обрабатывается отдельно
				{
					if (i == 6)
						word[p] = 'Ё';
					else if (i < 6)
						word[p] = 'А' + i;
					else
						word[p] = 'А' + i - 1;
					
					if (word[p] != letter)
					{
						if (endWords.find(word) != endWords.end())					// найдено в хвостовом списке: 
						{
							found = true; // цепочка найдена!
							forward ? children[word].push_back(temp) : children[temp].push_back(word); // к списку потомков нового/исх.слова добавляем исх.слово/новое слово
							break;
						}
						else if (!found && dict.find(word) != dict.end())			// найдено в словаре:
						{
							intermediate.insert(word);									// добавляем новое слово в промежуточный список
							forward ? children[word].push_back(temp) : children[temp].push_back(word); // к списку потомков нового/исх.слова добавляем исх.слово/новое слово
						}
						// не найдено нигде: ничего не делаем
					}
				}
				word[p] = letter;
				if (found) break;
			}
			if (found) break;
        }
		// если хоть одно слово головного списка найдено в хвостовом списке, возвращаем true
		// если нет, подставляем вместо головного списка хвостовой, а вместо хвостового - промежуточный, и вызываем функцию рекурсивно
        return found || searchChains(endWords, intermediate, dict, children, forward);
    }

// построение цепочки по спискам потомков
    void buildChain(string &start, string &end, unordered_map<string, vector<string>> &children, vector<string> &chain)  
	{
		if (chain[chain.size()-1] == end && chain.size()>1)
			return;
       
		for (auto it=children[start].begin(); it!=children[start].end(); ++it) // каждого потомка начального слова
		{
			string child = *it;
            chain.push_back(child);													// пробуем пришить к цепочке
            buildChain(child, end, children, chain);								// продолжаем используя этого потомка в качестве начального
            if (chain[chain.size()-1] == end && chain.size()>1)						// цепочка построена?
				return;																	// да, выход
			chain.pop_back();															// не получилось, убираем потомка из цепочки
        }
		return;
    }
};


inline void outPause()
{
	system("pause");
};

int main(int argc, char* argv[])
{
	bool supress = false;
	int wLen;
	string inputFileName;
	string dictFileName;
	string dWord;
	vector<string> words;
	unordered_set<string> Dictionary;
	setlocale(LC_ALL, "Russian");

	if (argc > 1)
	{
		if (argv[1][0] == '-')
			if (argv[1][1] == 's')
				supress = true;			// включен режим подавления сообщений
	}

	if (!supress) cout << "Задайте файл с начальным и конечным словами: ";
	cin >> inputFileName;
	if (!supress) cout << "Задайте файл словаря: ";
	cin >> dictFileName;
	
	if (inputFileName.find('\\') == 0) // если указано только имя файла, предполагаем, что он в текущем каталоге
	{
		char *p = NULL;
		p = _getcwd(NULL, 0);
		inputFileName = string(p) + "\\" + inputFileName;
	}
	if (dictFileName.find('\\') == 0)
	{
		char *p = NULL;
		p = _getcwd(NULL, 0);
		dictFileName = string(p) + "\\" + dictFileName;
	}

	try
	{
		words = getWords(inputFileName);
	}
	catch(...)
	{
		cout << "Файл " << inputFileName << " открыть не удалось." << endl;
		if (!supress) outPause();
		return 1;
	}

	if (words.size() < 2)
	{
		cout << "Файл " << inputFileName << " содержит менее двух слов." << endl;
		if (!supress) outPause();
		return 2;
	}
	if (words.size() > 2)
	{
		cout << "Файл " << inputFileName << " содержит более двух слов." << endl;
		if (!supress) outPause();
		return 2;
	}
	if (words[0].length() != words[1].length())
	{
		cout << "Файл " << inputFileName << " содержит слова разной длины." << endl;
		if (!supress) outPause();
		return 2;

	}
	wLen = words[0].length();

	// чтение словаря

	ifstream din(dictFileName);
	if (!din.is_open())
	{
		cout << "Файл " << dictFileName << " открыть не удалось." << endl;
		if (!supress) outPause();
		return 1;
	}

	while (!din.eof())
	{
		din >> dWord;
		if (dWord.length() == wLen) // выбираем слова только подходящей длины
		{
			for(unsigned i=0; i<dWord.length(); i++)
			{
				if(dWord[i] == 'ё' || dWord[i] == 'Ё') // замена Ё на Е 
					dWord[i] = 'Ё';
				if(dWord[i] >= 'а' && dWord[i] <= 'я') // все буквы делаем заглавными
					dWord[i]-=32;
			}
			Dictionary.insert(dWord);
		}
	}
	din.close();

	FlyToElefant *fe = new FlyToElefant();			// создаем объект
	vector<string> chain = fe->findChain(words[0], words[1], Dictionary); // получаем цепочку

	if (chain.size() < 2)
		cout << "Задача не имеет решения." << endl;							// цепочка не найдена
	else
	{
		if (!supress) cout << "Решение: " << endl << endl;					
		for (unsigned i=0; i<chain.size(); i++)								// вывод цепочки
		{
				cout << chain[i] << endl;
		}
		if (!supress) cout << endl;
	}
	delete fe;
	if (!supress) outPause();
	return 0;
}

