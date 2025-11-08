//guarded because this header isn't accessible on linux
#ifdef _WIN32
#include <Windows.h>
#endif

#include <fstream>
#include <map>
#include <codecvt>
#include "Word.h"
#include "json.hpp"
using json = nlohmann::json;
using namespace std;

string getRomaji(const string& kana);

int main()
{
    ifstream file("data/jmdict-all-3.6.1+20251013122507.json/jmdict-eng-3.6.1.json", ifstream::binary);
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
    map<wchar_t, string> kanaToRomaji = 
    {
        {L'あ', "a"}, {L'い', "i"}, {L'う', "u"}, {L'え', "e"}, {L'お', "o"},
        {L'か', "ka"}, {L'き', "ki"}, {L'く', "ku"}, {L'け', "ke"}, {L'こ', "ko"},
        {L'が', "ga"}, {L'ぎ', "gi"}, {L'ぐ', "gu"}, {L'げ', "ge"}, {L'ご', "go"},
        {L'さ', "sa"}, {L'し', "shi"}, {L'す', "su"}, {L'せ', "se"}, {L'そ', "so"},
        {L'ざ', "za"}, {L'じ', "ji"}, {L'ず', "zu"}, {L'ぜ', "ze"}, {L'ぞ', "zo"},
        {L'た', "ta"}, {L'ち', "chi"}, {L'つ', "tsu"}, {L'て', "te"}, {L'と', "to"},
        {L'だ', "da"}, {L'ぢ', "ji"}, {L'づ', "zu"}, {L'で', "de"}, {L'ど', "do"},
        {L'な', "na"}, {L'に', "ni"}, {L'ぬ', "nu"}, {L'ね', "ne"}, {L'の', "no"},
        {L'は', "ha"}, {L'ひ', "hi"}, {L'ふ', "fu"}, {L'へ', "he"}, {L'ほ', "ho"},
        {L'ば', "ba"}, {L'び', "bi"}, {L'ぶ', "bu"}, {L'べ', "be"}, {L'ぼ', "bo"},
        {L'ぱ', "pa"}, {L'ぴ', "pi"}, {L'ぷ', "pu"}, {L'ぺ', "pe"}, {L'ぽ', "po"},
        {L'ま', "ma"}, {L'み', "mi"}, {L'む', "mu"}, {L'め', "me"}, {L'も', "mo"},
        {L'や', "ya"}, {L'ゆ', "yu"}, {L'よ', "yo"},
        {L'ら', "ra"}, {L'り', "ri"}, {L'る', "ru"}, {L'れ', "re"}, {L'ろ', "ro"},
        {L'わ', "wa"}, {L'を', "wo"}, {L'ん', "n"}
    };

    string res = "";

    /*
    converts the UTF-8 hiragana string to a wide string
    so the for loop can iterate through it properly
    */
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wideKana = converter.from_bytes(kana);
    
    for (wchar_t wc : wideKana)
    {
        if (kanaToRomaji.find(wc) != kanaToRomaji.end())
        {
            res += kanaToRomaji[wc];
        }
        else
        {
            return "";
        }
    }
    
    return res;
}
