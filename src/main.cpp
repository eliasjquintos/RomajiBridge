#include <iostream>
#include <fstream>
#include "json.hpp"
#include "Word.h"
using std::ifstream;
using json = nlohmann::json;
using namespace std;

int main()
{
    ifstream file("data/jmdict-all-3.6.1+20251013122507.json/jmdict-all-3.6.1.json", ifstream::binary);
    if (!file.is_open())
    {
        cerr << "Couldn't open file for some reason..." << endl;
        return 1;
    }
    //cout << j["words"].size() << endl;
    json j = json::parse(file);
    
    string randKana = j["words"][420]["kana"][0]["text"];
    cout << randKana << endl;
    
    string randKana2 = j["words"][421]["kana"][0]["text"];
    cout << randKana2 << endl;

    string randDef = j["words"][421]["sense"][0]["gloss"][0]["text"];
    cout << randDef << endl;

    file.close();
    return 0;    
}