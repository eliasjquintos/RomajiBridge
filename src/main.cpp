//guarded because this header isn't accessible on linux
#ifdef _WIN32
#include <Windows.h>
#endif

#include <fstream>
#include <map>
#include <string>
#include <iostream>
#include <codecvt>
#include "SimpleTimer.h"
#include "Word.h"
#include "Menu.h"
#include "Trie.h"
#include "BPlusTree.h"
//#include "json.hpp"
//TODO Get words into csv file
//Insert words into trie
//include Menu
//search trie

// UPDATED TODO insert words into B+ tree
// search B+ tree

//using json = nlohmann::json;
using namespace std;

// string getRomaji(const string& kana);

int main()
{
    //Makes OEM Hiragana readable to Windows Powershell (UTF-8).
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif

    /*
    Excluding kanji, and small characters like っ,ゃ,ゅ,ょ
    gives 123095 entries to work with.
    */
    ifstream dict("data/dict.csv");
    if (!dict.is_open())
    {
        cerr << "Couldn't retrieve dictionary file" << endl;
        return -1;
    }

    //make trie from csv
    Trie trie;

    //make B+ tree from csv
    BPlusTree bptree(70); // degree 70 allows max 139 entries per node; shallow tree -> faster prefix search

    //TODO: Make searchable from the menu
    bool loading = true;
    bool searching = true;
    while (searching)
    {
        if (loading)
        {
            while (!dict.eof())
            {
                string kana, def, romaji;
                getline(dict, kana, ',');
                getline(dict, def, ',');
                getline(dict, romaji);
                Word* w = new Word(kana, def, romaji);
                trie.Insert(w);
                bptree.insert(w);
            }
            loading = false;
            dict.close();
            cout << "Dictionary loaded successfully!" << endl;  
        }

        // menu layout
        // prompt for search type -> prompt for method type -> ask for input 
        // first, prompt for search type
        cout << "Choose search mode: \n";
        cout << "1) Exact Search\n";
        cout << "2) Prefix Search\n";
        cout << "q) Quit\n";
        cout << "Enter choice: " << endl;

        string choice;
        getline(cin, choice);
        if (choice == "q") {
            searching = false;
            continue;
        }

        // added in input validation for search type 
        if (choice != "1" && choice != "2")
        {
            cout << "Invalid choice. Please try again." << endl;
            continue;
        }

        // next, prompt for method type (data structure)
        cout << "Search with...\n";
        cout << "A) Trie\n";
        cout << "B) B+ Tree\n";

        string method;
        getline(cin, method);

        // added in input validation for method type
        if (method != "A" && method != "B")
        {
            cout << "Invalid choice. Please try again." << endl;
            continue;
        }

        // finally, ask for input
        cout << "Enter your search or enter q to quit: " << endl;
        string search;
        getline(cin, search);

        if (search.empty())
        {
            cout << "Input cannot be empty. Please enter a valid romaji string." << endl;
            continue;
        }
        else if (search == "q")
        {
            searching = false;
        }
        else if (choice == "1") 
        { 
            if (method == "A")
            {
                SimpleTimer timer; // starts time
                ExactTrieSearch(trie, search);
            }
            else if (method == "B")
            {
                SimpleTimer timer; // starts time
                ExactBPlusTreeSearch(bptree, search);
            }
        }
        else if (choice == "2")
        {
            if (choice == "A")
            {
                SimpleTimer timer; // starts time
                PrefixTrieSearch(trie, search);
            }
            else if (choice == "B")
            {
                SimpleTimer timer; // starts time
                PrefixBPlusTreeSearch(bptree, search);
            }
        }
    }
    return 0;    
}

//UTF-8 input
/*
string getRomaji(const string& kana)
{
    map<char32_t, string> kanaToRomaji = 
    {
        // Hiragana
        {U'あ', "a"}, {U'い', "i"}, {U'う', "u"}, {U'え', "e"}, {U'お', "o"},
        {U'か', "ka"}, {U'き', "ki"}, {U'く', "ku"}, {U'け', "ke"}, {U'こ', "ko"},
        {U'が', "ga"}, {U'ぎ', "gi"}, {U'ぐ', "gu"}, {U'げ', "ge"}, {U'ご', "go"},
        {U'さ', "sa"}, {U'し', "shi"}, {U'す', "su"}, {U'せ', "se"}, {U'そ', "so"},
        {U'ざ', "za"}, {U'じ', "ji"}, {U'ず', "zu"}, {U'ぜ', "ze"}, {U'ぞ', "zo"},
        {U'た', "ta"}, {U'ち', "chi"}, {U'つ', "tsu"}, {U'て', "te"}, {U'と', "to"},
        {U'だ', "da"}, {U'ぢ', "ji"}, {U'づ', "zu"}, {U'で', "de"}, {U'ど', "do"},
        {U'な', "na"}, {U'に', "ni"}, {U'ぬ', "nu"}, {U'ね', "ne"}, {U'の', "no"},
        {U'は', "ha"}, {U'ひ', "hi"}, {U'ふ', "fu"}, {U'へ', "he"}, {U'ほ', "ho"},
        {U'ば', "ba"}, {U'び', "bi"}, {U'ぶ', "bu"}, {U'べ', "be"}, {U'ぼ', "bo"},
        {U'ぱ', "pa"}, {U'ぴ', "pi"}, {U'ぷ', "pu"}, {U'ぺ', "pe"}, {U'ぽ', "po"},
        {U'ま', "ma"}, {U'み', "mi"}, {U'む', "mu"}, {U'め', "me"}, {U'も', "mo"},
        {U'や', "ya"}, {U'ゆ', "yu"}, {U'よ', "yo"},
        {U'ら', "ra"}, {U'り', "ri"}, {U'る', "ru"}, {U'れ', "re"}, {U'ろ', "ro"},
        {U'わ', "wa"}, {U'を', "wo"}, {U'ん', "n"},

        // Katakana
        {U'ア', "a"}, {U'イ', "i"}, {U'ウ', "u"}, {U'エ', "e"}, {U'オ', "o"},
        {U'カ', "ka"}, {U'キ', "ki"}, {U'ク', "ku"}, {U'ケ', "ke"}, {U'コ', "ko"},
        {U'ガ', "ga"}, {U'ギ', "gi"}, {U'グ', "gu"}, {U'ゲ', "ge"}, {U'ゴ', "go"},
        {U'サ', "sa"}, {U'シ', "shi"}, {U'ス', "su"}, {U'セ', "se"}, {U'ソ', "so"},
        {U'ザ', "za"}, {U'ジ', "ji"}, {U'ズ', "zu"}, {U'ゼ', "ze"}, {U'ゾ', "zo"},
        {U'タ', "ta"}, {U'チ', "chi"}, {U'ツ', "tsu"}, {U'テ', "te"}, {U'ト', "to"},
        {U'ダ', "da"}, {U'ヂ', "ji"}, {U'ヅ', "zu"}, {U'デ', "de"}, {U'ド', "do"},
        {U'ナ', "na"}, {U'ニ', "ni"}, {U'ヌ', "nu"}, {U'ネ', "ne"}, {U'ノ', "no"},
        {U'ハ', "ha"}, {U'ヒ', "hi"}, {U'フ', "fu"}, {U'ヘ', "he"}, {U'ホ', "ho"},
        {U'バ', "ba"}, {U'ビ', "bi"}, {U'ブ', "bu"}, {U'ベ', "be"}, {U'ボ', "bo"},
        {U'パ', "pa"}, {U'ピ', "pi"}, {U'プ', "pu"}, {U'ペ', "pe"}, {U'ポ', "po"},
        {U'マ', "ma"}, {U'ミ', "mi"}, {U'ム', "mu"}, {U'メ', "me"}, {U'モ', "mo"},
        {U'ヤ', "ya"}, {U'ユ', "yu"}, {U'ヨ', "yo"},
        {U'ラ', "ra"}, {U'リ', "ri"}, {U'ル', "ru"}, {U'レ', "re"}, {U'ロ', "ro"},
        {U'ワ', "wa"}, {U'ヲ', "wo"}, {U'ン', "n"}
    };

    string res = "";

    //converts the UTF-8 hiragana string to a wide string
    //so the for loop can iterate through it properly
    
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string wideKana = converter.from_bytes(kana);
    
    for (char32_t ch : wideKana)
    {
        if (kanaToRomaji.find(ch) != kanaToRomaji.end())
        {
            res += kanaToRomaji[ch];
        }
        else
        {
            return "";
        }
    }
    
    return res;
}
*/

/*
Included for reference only
original code to get the csv from json:
ifstream file("data/jmdict-eng-3.6.1.json", ifstream::binary);
if (!file.is_open())
{
    cerr << "Couldn't open file for some reason..." << endl;
    return 1;
}
json j = json::parse(file);
file.close();

original code to parse json
for (int i = 0; i < 213984; i++)
{
    string kana = j["words"][i]["kana"][0]["text"];
    string def = j["words"][i]["sense"][0]["gloss"][0]["text"];
    string romaji = getRomaji(kana);
    Word w(kana, def, romaji);
    if (w.getRomaji() != "")
    {
        dict << w.getKana() << "," << w.getEnDef() << "," << w.getRomaji() << "\n";
        viableWordCount++;
    }
}
*/
