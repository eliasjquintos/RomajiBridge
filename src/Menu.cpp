#include "Menu.h"
#include <string>
#include <iostream>
using namespace std;

/*====== Trie ======*/
void ExactTrieSearch(Trie& trie, const string& input)
{
	vector<Word*> res = trie.Search(input);

	if (res.empty())
	{
		cout << "No exact match found" << endl;
		return;
	}

	cout << "------------------------------------------------------------" << endl;

	int i = 1;
	for (Word* w : res)
	{
		cout << i << ") ";
		w->displayWord();
		cout << "------------------------------------------------------------" << endl;
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

	cout << "------------------------------------------------------------" << endl;

	int i = 1;
	for (Word* w : res)
	{
		cout << i << ") ";
		w->displayWord();
		cout << "------------------------------------------------------------" << endl;
		i++;
	}
}

/*====== B+ Tree ======*/
void ExactBPlusTreeSearch(BPlusTree& bptree, const string& input)
{
	std::vector<Word*> res = bptree.exactSearch(input); // now, will return a vector of Word*

	if (res.empty())
	{
		cout << "No exact match found" << endl;
		return;
	}

	cout << "------------------------------------------------------------" << endl;
	
	int i = 1;
	for (Word* w : res)
	{
		cout << i << ") ";
		w->displayWord();
		cout << "------------------------------------------------------------" << endl;
		i++;
	}

	cout << "------------------------------------------------------------" << endl;
}

void PrefixBPlusTreeSearch(BPlusTree& bptree, const string& input)
{
	vector<Word*> res = bptree.prefixSearch(input);

	if (res.empty())
	{
		cout << "No words found with this prefix" << endl;
		return;
	}

	cout << "------------------------------------------------------------" << endl;

	int i = 1;
	for (Word* w : res)
	{
		cout << i << ") ";
		w->displayWord();
		cout << "------------------------------------------------------------" << endl;
		i++;
	}
}