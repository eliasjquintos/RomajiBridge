#pragma once
#include "Trie.h"
#include <string>
using std::string;

void ExactSearch(Trie& trie, const string& input);
void PrefixSearch(Trie& trie, const string& input);