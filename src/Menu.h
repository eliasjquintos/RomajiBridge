#pragma once
#include "Trie.h"
#include <string>
using std::string;

void ExactTrieSearch(Trie& trie, const string& input);
void PrefixTrieSearch(Trie& trie, const string& input);