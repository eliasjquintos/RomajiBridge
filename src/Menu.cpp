#include "Menu.h"
#include <string>
#include <iostream>
using namespace std;

void ExactSearch(Trie& trie, const string& input)
{
	vector<Word*> res = trie.Search(input);

	if (res.empty())
	{
		cout << "No exact match found" << endl;
		return;
	}

	for (Word* w : res)
	{
		cout << "Romaji: " << w->getRomaji() << " ";
		cout << "Kana: " << w->getKana() << endl;
		cout << "Meaning: " << w->getEnDef() << endl << endl;
	}
}

void PrefixSearch(Trie& trie, const string& input)
{
	vector<Word*> res = trie.PrefixSearch(input);

	if (res.empty())
	{
		cout << "No words found with this prefix" << endl;
		return;
	}

	for (Word* w : res)
	{
		cout << "Romaji: " << w->getRomaji() << " ";
		cout << "Kana: " << w->getKana() << endl;
		cout << "Meaning: " << w->getEnDef() << endl << endl;
	}
}