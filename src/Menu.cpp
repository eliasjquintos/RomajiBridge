#include "Menu.h"
#include <string>
#include <iostream>
using namespace std;

void ExactTrieSearch(Trie& trie, const string& input)
{
	vector<Word*> res = trie.Search(input);

	if (res.empty())
	{
		cout << "No exact match found" << endl;
		return;
	}

	int i = 1;
	for (Word* w : res)
	{
		cout << i << ") ";
		w->displayWord();
		i++;
	}
}

void PrefixTrieSearch(Trie& trie, const string& input)
{
	vector<Word*> res = trie.PrefixSearch(input);

	if (res.empty())
	{
		cout << "No words found with this prefix" << endl;
		return;
	}

	int i = 1;
	for (Word* w : res)
	{
		cout << i << ") ";
		w->displayWord();
		i++;
	}
}