#include <string>
#include <vector>
#include <memory>
#include "Word.h"
#include "Trie.h"
using namespace std;

/*======== Constructor ========*/
Trie::TrieNode::TrieNode() : isEnd(false)
{

}

/*======== Helper Functions ========*/
void Trie::GetAllWords(TrieNode* node, vector<Word*>& res)
{
	if (node->isEnd)
	{
		for (Word* w : node->words)
			res.push_back(w);
	}

	for (auto iter = node->children.begin(); iter != node->children.end(); ++iter)
	{
		TrieNode* child = iter->second.get();
		GetAllWords(child, res);
	}
}

/*======== Operations ========*/
void Trie::Insert(Word* word)
{
	const string& romaji = word->getRomaji();
	TrieNode* curr = &root;

	for (char c : romaji)
	{
		// If the child doesn't exist a new one is created
		if (curr->children.find(c) == curr->children.end())
			curr->children[c] = make_unique<TrieNode>();

		// Go down one node in the trie
		curr = curr->children[c].get();
	}

	curr->isEnd = true;
	curr->words.push_back(word);
}
vector<Word*> Trie::Search(const string& romaji)
{
	TrieNode* curr = &root;

	for (char c : romaji)
	{
		// If the child doesn't exist there is no match
		if (curr->children.find(c) == curr->children.end())
			return {};

		// Go down one node in the trie
		curr = curr->children[c].get();
	}

	return curr->words;
}
vector<Word*> Trie::PrefixSearch(const string& romaji)
{
	TrieNode* curr = &root;

	for (char c : romaji)
	{
		if (curr->children.find(c) == curr->children.end())
			return {}; // prefix not found

		// Go down one node in the trie
		curr = curr->children[c].get();
	}

	// Get all words in the subtree
	vector<Word*> res;
	GetAllWords(curr, res);
	return res;
}