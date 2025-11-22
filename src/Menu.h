#pragma once
#include "Trie.h"
#include "BPlusTree.h"
#include <string>
using std::string;

/*====== Trie ======*/
void ExactTrieSearch(Trie& trie, const string& input);
void PrefixTrieSearch(Trie& trie, const string& input);

/*====== B+ Tree ======*/
void ExactBPlusTreeSearch(BPlusTree& bptree, const string& input);
void PrefixBPlusTreeSearch(BPlusTree& bptree, const string& input);