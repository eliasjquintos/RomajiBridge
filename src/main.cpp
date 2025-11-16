//guarded because this header isn't accessible on linux
#ifdef _WIN32
#include <Windows.h>
#endif

#include <fstream>
#include <map>
#include <string>
#include <iostream>
#include <codecvt>
#include "Word.h"
#include "json.hpp"
using json = nlohmann::json;
using namespace std;

string getRomaji(const string& kana);

int main()
{
    ifstream file("data/jmdict-eng-3.6.1.json", ifstream::binary);
    if (!file.is_open())
    {
        cerr << "Couldn't open file for some reason..." << endl;
        return 1;
    }
    json j = json::parse(file);

    /*
    The gibberish nonsense characters were OEM characters. 
    The two lines below tell windows to interpret hiragana as UTF-8.
    */
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif

    /*
    The size of the "words" tag in the "all" file: 213730
    Entries in "eng" file: 213984
    cout << j["words"].size() << " entries in total\n" << endl;
    */


    /*
    Getting words purely in hiragana first. Excluding katakana,
    kanji, and small characters like っ,ゃ,ゅ,ょ.
    */
    int viableWordCount = 0;
    
    /*
    Creates and prints the word object 
    for each entry in the json file
    */
    for (int i = 0; i < 213984; i++)
    {
        string kana = j["words"][i]["kana"][0]["text"];
        string def = j["words"][i]["sense"][0]["gloss"][0]["text"];
        string romaji = getRomaji(kana);
        Word w(kana, def, romaji);
        if (w.getRomaji() != "")
        {
            w.displayWord();
            viableWordCount++;
        }
    }

    /*
    Purely hiragana still gives us 104602 entries ("all" file) to work with.
    "eng" file: 104650
    */
    cout << "Total words accounted for: " << viableWordCount << endl;
    file.close();
    return 0;    
}

//UTF-8 input
string getRomaji(const string& kana)
{
    map<char32_t, string> kanaToRomaji = 
    {
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
        {U'わ', "wa"}, {U'を', "wo"}, {U'ん', "n"}
    };

    string res = "";

    /*
    converts the UTF-8 hiragana string to a wide string
    so the for loop can iterate through it properly
    */
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
