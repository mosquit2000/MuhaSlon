/*******************************
Набор тестов для решения задания 
"Как из мухи сделать слона"
Выполнил Алексей Сперантов
19.07.2016
********************************/
#include "stdafx.h"
#include "..\MuhaSlon\getwords.h" // зависит от конфигурации проекта
#include <string>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <direct.h>
#include <vector>

using namespace std;

bool allTestsOK = 1; // глобальная переменная "все тесты пройдены"

// выполнение теста testname
// аргументы: имя теста, командная строка
void goTest(string testName, string com) 
{
	cout << "***" << testName;
	char *command = &com[0];
	system(command);
}

// Вывод результата теста
// аргументы: ожидаемое значение, актуальное значение
void outResult(string expected, string actual)
{
	if (actual == expected)
		cout << " passed***\n";
	else
	{
		allTestsOK = 0;
		cout << " failed!!!***\n";
	}
	system("pause");
}

// Проверка цепочки слов на соответствие условию:
// каждое последующее слово отличается от предыдущего ровно на одну букву.
// аргумент: массив строк
// возвращает true, если условие выполняется
// и false, если нет
bool everyNextExactOneLetter(vector <string> words)
{
	for (unsigned int i = 0; i < words.size() - 2; i++)
	{
		int cnt = 0;
		for (unsigned int k = 0; k < words[0].size(); k++)
		{
			if (words[i][k] != words[i + 1][k])
				cnt++;
		}
		if (cnt != 1)
		{
			return false;
			break;
		}
	}
	return true;
}

int main()
{
	const string outName = "output.txt";
	string exp;
	string act;
	vector <string> outwords;
	vector <string> inwords;

    setlocale(0,"rus");
	try
	{
		_chdir("..\\debug"); // для запуска в IDE
	}
	catch(...) {}
	
 //test1: отсутствует файл с начальным и конечным словом - условие задачи не выполнено
	goTest("TEST#1 NO_WORDS", "muhaslon -s <inputNoWords.txt >output.txt");
	exp = "Файл notexists.txt открыть не удалось.";
	act = getWords(outName)[0];
	outResult(exp, act);

//test1a: отсутствует файл словаря - условие задачи не выполнено
	goTest("TEST#1a NO_DICT", "muhaslon -s <inputNoDict.txt >output.txt");
	exp = "Файл notexists.txt открыть не удалось.";
	act = getWords(outName)[0];
	outResult(exp, act);

//test2: файл с исходными словами содержит менее двух строк - условие задачи не выполнено
	goTest("TEST#2 LESS_2", "muhaslon -s <inputLess2.txt >output.txt");
	exp = "Файл inputwordsless2.txt содержит менее двух слов.";
	act = getWords(outName)[0];
	outResult(exp, act);

//test3: файл с исходными словами содержит более двух строк - условие задачи не выполнено
	goTest("TEST#3 MORE_2", "muhaslon -s <inputMore2.txt >output.txt");
	exp = "Файл inputwordsmore2.txt содержит более двух слов.";
	act = getWords(outName)[0];
	outResult(exp, act);

//test4: файл с исходными словами содержит слова разной длины - условие задачи не выполнено
	goTest("TEST#4 DIFF_LENGTH", "muhaslon -s <inputDiffLength.txt >output.txt");
	exp = "Файл inputwordsdifflength.txt содержит слова разной длины.";
	act = getWords(outName)[0];
	outResult(exp, act);

//test5: условие задачи выполнено, требуемая цепочка не найдена
	goTest("TEST#5 NO_CHAIN", "muhaslon -s <inputNoChain.txt >output.txt");
	exp = "Задача не имеет решения.";
	act = getWords(outName)[0];
	outResult(exp, act);

//test6: условие задачи выполнено, требуемая цепочка найдена
	goTest("TEST#6 FOUND_CHAIN", "muhaslon -s <input.txt >output.txt");
	outwords = getWords(outName);

	act = "true";
	exp = "true";

	inwords = getWords(getWords("input.txt")[0]);

	if (outwords[0] != inwords[0]) act = "false"; // проверка первого слова
	if (outwords[outwords.size() - 1] != inwords[1]) act = "false"; // проверка последнего слова
	if (!everyNextExactOneLetter(outwords)) act = "false"; // проверка условия отличия соседних слов
	outResult(exp, act);

//test7a1: заданы одинаковые слова и словарь, в котором заведомо есть промежуточное слово
	goTest("TEST#7a1 THE_SAME_GOOD", "muhaslon -s <inputTheSameGood.txt >output.txt");
	outwords = getWords(outName);

	act = "true";
	exp = "true";

	if (outwords.size() != 3) act = "false";
	if (!everyNextExactOneLetter(outwords)) act = "false";
	outResult(exp, act);

//test7a2: заданы одинаковые слова и словарь, в котором заведомо нет промежуточного слова
	goTest("TEST#7a2 THE_SAME_BAD", "muhaslon -s <inputTheSameBad.txt >output.txt");
	exp = "Задача не имеет решения.";
	act = getWords(outName)[0];
	outResult(exp, act);

//test7b1: заданы слова, отличающиеся одной буквой, и реальный словарь
	goTest("TEST#7b1 ONE_LETTER_DIFF_GOOD", "muhaslon -s <inputOneLetterDiffGood.txt >output.txt");
	outwords = getWords(outName);
	act = "true";
	exp = "true";

	if (outwords.size() != 2) act = "false";
	inwords = getWords(getWords("inputOneLetterDiffGood.txt")[0]);
	for (int i=0; i<2; i++)
		if (outwords[i] != inwords[i]) act = "false";
	outResult(exp, act);

//test7b2: заданы слова, отличающиеся одной буквой, и заведомо "плохой" словарь
	goTest("TEST#7b2 ONE_LETTER_DIFF_BAD", "muhaslon -s <inputOneLetterDiffBad.txt >output.txt");
	outwords = getWords(outName);
	act = "true";
	exp = "true";

	if (outwords.size() != 2) act = "false";
	inwords = getWords(getWords("inputOneLetterDiffGood.txt")[0]);
	for (int i=0; i<2; i++)
		if (outwords[i] != inwords[i]) act = "false";
	outResult(exp, act);

	if (allTestsOK)
		cout << "\nCONGRATS, ALL TESTS PASSED!\n";
	else
		cout << "\nSORRY, SOME TESTS FAILED.\n";
	system("pause");
	return 0;
}


