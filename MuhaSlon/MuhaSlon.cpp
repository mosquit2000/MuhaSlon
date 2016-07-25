/**************************
������� ��������� ������� 
"��� �� ���� ������� �����"
�������� ������� ���������
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
// ����� �������
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
// ������������ �����
    bool searchChains(unordered_set<string>& startWords, unordered_set<string>& endWords, unordered_set<string>& dict, unordered_map<string, vector<string>>& children, bool forward) 
	{
        forward = !forward;
        if (startWords.empty()) return false;    // �������� ������ ���� - ���������� false
        if (startWords.size() > endWords.size()) // �������� ������ ������� ����������:
            return searchChains(endWords, startWords, dict, children, forward); // ������ ������� ������ � �������� ������� ����������

		// ����������� ��� ����� ��������� ������ �� �������
        for (auto it = startWords.begin(); it != startWords.end(); ++it) dict.erase(*it);
		// ����������� ��� ����� ���������� ������ �� �������
        for (auto it = endWords.begin(); it != endWords.end(); ++it) dict.erase(*it);

        unordered_set<string> intermediate; // ������������� ������
        bool found = false;
        for (auto it = startWords.begin(); it != startWords.end(); ++it) // ���������� �������� ������
		{
			string word = *it;												// � ������� �����
            int n = word.length();		
            string temp = word;
			int p;
            for (p = 0; p < n; p++)											// ������ �� ������� ������ �����
			{
				char letter = word[p];
				for (int i = 0; i < 33; i++)								// �� ��� ������ �� ��������, � �������������� ��������
				{
					if (i == 6)
						word[p] = '�';
					else if (i < 6)
						word[p] = '�' + i;
					else
						word[p] = '�' + i - 1;
					
					if (word[p] != letter)
					{
						if (endWords.find(word) != endWords.end())					// ������� � ��������� ������: 
						{
							found = true; // ������� �������!
							forward ? children[word].push_back(temp) : children[temp].push_back(word); // � ������ �������� ������/���.����� ��������� ���.�����/����� �����
							break;
						}
						else if (!found && dict.find(word) != dict.end())			// ������� � �������:
						{
							intermediate.insert(word);									// ��������� ����� ����� � ������������� ������
							forward ? children[word].push_back(temp) : children[temp].push_back(word); // � ������ �������� ������/���.����� ��������� ���.�����/����� �����
						}
						// �� ������� �����: ������ �� ������
					}
				}
				word[p] = letter;
				if (found) break;
			}
			if (found) break;
        }
		// ���� ���� ���� ����� ��������� ������ ������� � ��������� ������, ���������� true
		// ���� ���, ����������� ������ ��������� ������ ���������, � ������ ���������� - �������������, � �������� ������� ����������
        return found || searchChains(endWords, intermediate, dict, children, forward);
    }

// ���������� ������� �� ������� ��������
    void buildChain(string &start, string &end, unordered_map<string, vector<string>> &children, vector<string> &chain)  
	{
		if (chain[chain.size()-1] == end && chain.size()>1)
			return;
       
		for (auto it=children[start].begin(); it!=children[start].end(); ++it) // ������� ������� ���������� �����
		{
			string child = *it;
            chain.push_back(child);													// ������� ������� � �������
            buildChain(child, end, children, chain);								// ���������� ��������� ����� ������� � �������� ����������
            if (chain[chain.size()-1] == end && chain.size()>1)						// ������� ���������?
				return;																	// ��, �����
			chain.pop_back();															// �� ����������, ������� ������� �� �������
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
				supress = true;			// ������� ����� ���������� ���������
	}

	if (!supress) cout << "������� ���� � ��������� � �������� �������: ";
	cin >> inputFileName;
	if (!supress) cout << "������� ���� �������: ";
	cin >> dictFileName;
	
	if (inputFileName.find('\\') == 0) // ���� ������� ������ ��� �����, ������������, ��� �� � ������� ��������
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
		cout << "���� " << inputFileName << " ������� �� �������." << endl;
		if (!supress) outPause();
		return 1;
	}

	if (words.size() < 2)
	{
		cout << "���� " << inputFileName << " �������� ����� ���� ����." << endl;
		if (!supress) outPause();
		return 2;
	}
	if (words.size() > 2)
	{
		cout << "���� " << inputFileName << " �������� ����� ���� ����." << endl;
		if (!supress) outPause();
		return 2;
	}
	if (words[0].length() != words[1].length())
	{
		cout << "���� " << inputFileName << " �������� ����� ������ �����." << endl;
		if (!supress) outPause();
		return 2;

	}
	wLen = words[0].length();

	// ������ �������

	ifstream din(dictFileName);
	if (!din.is_open())
	{
		cout << "���� " << dictFileName << " ������� �� �������." << endl;
		if (!supress) outPause();
		return 1;
	}

	while (!din.eof())
	{
		din >> dWord;
		if (dWord.length() == wLen) // �������� ����� ������ ���������� �����
		{
			for(unsigned i=0; i<dWord.length(); i++)
			{
				if(dWord[i] == '�' || dWord[i] == '�') // ������ � �� � 
					dWord[i] = '�';
				if(dWord[i] >= '�' && dWord[i] <= '�') // ��� ����� ������ ����������
					dWord[i]-=32;
			}
			Dictionary.insert(dWord);
		}
	}
	din.close();

	FlyToElefant *fe = new FlyToElefant();			// ������� ������
	vector<string> chain = fe->findChain(words[0], words[1], Dictionary); // �������� �������

	if (chain.size() < 2)
		cout << "������ �� ����� �������." << endl;							// ������� �� �������
	else
	{
		if (!supress) cout << "�������: " << endl << endl;					
		for (unsigned i=0; i<chain.size(); i++)								// ����� �������
		{
				cout << chain[i] << endl;
		}
		if (!supress) cout << endl;
	}
	delete fe;
	if (!supress) outPause();
	return 0;
}

