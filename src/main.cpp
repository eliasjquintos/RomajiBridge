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
    /*
    Creating a parsable json object from the data path.
    This can be put into a function later
    */

    ifstream file("data/jmdict-all-3.6.1+20251013122507.json/jmdict-all-3.6.1.json", ifstream::binary);
    if (!file.is_open())
    {
        cerr << "Couldn't open file for some reason..." << endl;
        return 1;
    }
    json j = json::parse(file);

    /*
    The gibberish we were getting were OEM characters. 
    These two lines below tell the system to interpret them as UTF-8.
    */
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif

    /*
    The size of the "words" tag ended up being 213730
    //cout << j["words"].size() << " entries in total" << endl;
    */

    /*
    I wanted to count how many words were purely in hiragana
    I think our project would be much too complicated looking up katakana,
    kanji, and small characters like っ,ゃ,ゅ,ょ. Let's not commit
    seppuku just yet.
    */
    int viableWordCount = 0;
    
    /*
    Creates and prints the word object 
    for each entry in the json file
    */
    for (int i = 0; i < 213730; i++)
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
    Purely hiragana still gives us 104602 entries to work with.
    */
    cout << "Total words accounted for: " << viableWordCount << endl;
    file.close();
    return 0;    
}

string getRomaji(const string& kana)
{
    map<string, string> kanaToRomaji = {
        {"あ", "a"}, {"い", "i"}, {"う", "u"}, {"え", "e"}, {"お", "o"},
        {"か", "ka"}, {"き", "ki"}, {"く", "ku"}, {"け", "ke"}, {"こ", "ko"},
        {"が", "ga"}, {"ぎ", "gi"}, {"ぐ", "gu"}, {"げ", "ge"}, {"ご", "go"},
        {"さ", "sa"}, {"し", "shi"}, {"す", "su"}, {"せ", "se"}, {"そ", "so"},
        {"ざ", "za"}, {"じ", "ji"}, {"ず", "zu"}, {"ぜ", "ze"}, {"ぞ", "zo"},
        {"た", "ta"}, {"ち", "chi"}, {"つ", "tsu"}, {"て", "te"}, {"と", "to"},
        {"だ", "da"}, {"ぢ", "ji"}, {"づ", "zu"}, {"で", "de"}, {"ど", "do"},
        {"な", "na"}, {"に", "ni"}, {"ぬ", "nu"}, {"ね", "ne"}, {"の", "no"},
        {"は", "ha"}, {"ひ", "hi"}, {"ふ", "fu"}, {"へ", "he"}, {"ほ", "ho"},
        {"ば", "ba"}, {"び", "bi"}, {"ぶ", "bu"}, {"べ", "be"}, {"ぼ", "bo"},
        {"ぱ", "pa"}, {"ぴ", "pi"}, {"ぷ", "pu"}, {"ぺ", "pe"}, {"ぽ", "po"},
        {"ま", "ma"}, {"み", "mi"}, {"む", "mu"}, {"め", "me"}, {"も", "mo"},
        {"や", "ya"}, {"ゆ", "yu"}, {"よ", "yo"},
        {"ら", "ra"}, {"り", "ri"}, {"る", "ru"}, {"れ", "re"}, {"ろ", "ro"},
        {"わ", "wa"}, {"を", "wo"}, {"ん", "n"}
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
        string k = converter.to_bytes(wc);
        if (kanaToRomaji.find(k) != kanaToRomaji.end())
        {
            res += kanaToRomaji[k];
        }
        else
        {
            return "";
        }
    }
    return res;
}
