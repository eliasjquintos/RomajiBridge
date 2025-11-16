#pragma once
#include <map>
#include <vector>
#include <memory>
#include "Word.h"
using std::map;
using std::vector;
using std::unique_ptr;

class Trie
{
	struct TrieNode
	{
		bool isEnd;
		map<char, unique_ptr<TrieNode>> children;
		vector<Word*> words; // Multiple words can share the same romaji

		TrieNode();
	};

	TrieNode root;
	/*====== Helper Functions ======*/
	void GetAllWords(TrieNode* node, vector<Word*>& res); // Gets every word that begins with a romaji prefix 

public:
	/*====== Constructor ======*/
	Trie() = default;

	/*====== Operations ======*/
	void Insert(Word* word); // Inserts the romaji string into the trie
	vector<Word*> Search(const string& romaji); // Returns vector of words that use that exact romaji
	vector<Word*> PrefixSearch(const string& romaji); // Returns vector of words that have that romaji prefix
};