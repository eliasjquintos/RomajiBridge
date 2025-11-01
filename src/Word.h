#pragma once

#include <iostream>
using std::string;
using std::cout;
using std::endl;

class Word
{
    string kana;
    string enDef;
    string romaji;
public:
    Word(string k, string e, string r) : kana(k), enDef(e), romaji(r) {}
    string getKana() const { return kana; }
    string getEnDef() const { return enDef; }
    string getRomaji() const { return romaji; }
    void displayWord() {
        cout << "Kana: " << getKana() << ", Definition: " << getEnDef() << ", Romaji: " << getRomaji() << endl;}
};