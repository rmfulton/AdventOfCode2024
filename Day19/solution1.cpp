#include <iostream>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

pair<vector<string>, vector<string>> parseInput(string filename)
{
    ifstream f(filename);
    vector<string> patterns;
    vector<string> designs;
    string instring;
    int numLackingCommas = 0;
    while (f >> instring)
    {
        if (instring[instring.size() - 1] == ','){
            patterns.push_back(instring.substr(0, instring.size() - 1));
        } else if (numLackingCommas < 1){
            patterns.push_back(instring);
            numLackingCommas += 1;
        } else {
            designs.push_back(instring);
        }
    }
    return make_pair(patterns, designs);
}

int canBeMade(vector<string> patterns, string design)
{
    vector<string> newPatterns;
    for (string p : patterns){
        if (design.find(p) == design.npos){
            continue;
        }
        newPatterns.push_back(p);
    }

    const int NumSubStrings = design.size() + 1;
    // canBeMade[i] denotes whether the substring design.substr(0, i) can be constructed from patterns
    bool canBeMade[NumSubStrings];
    for (int i = 0; i < NumSubStrings; ++i){
        canBeMade[i] = false;
    }
    canBeMade[0] = true;
    for (int i = 1; i < NumSubStrings; ++i){
        bool connectsToWorkingString = false;
        for (string component : patterns){
            int L = component.size();
            if (L <= i && canBeMade[i - L] && design.substr(i-L, L) == component){
                connectsToWorkingString = true;
                break;
            }
        }
        canBeMade[i] = connectsToWorkingString;
    }
    return canBeMade[NumSubStrings - 1];
}

int main()
{

    string filename = "input.txt";
    // filename = "sample.txt";
    pair<vector<string>, vector<string>> patternsAndDesigns = parseInput(filename);
    vector<string> patterns = patternsAndDesigns.first;
    vector<string> designs = patternsAndDesigns.second;
    int count = 0;
    for (string d : designs)
    {
        cout << "Design " << d << endl;
        if (canBeMade(patterns, d))
            ++count;
    }
    cout << count << endl;
    return 0;
}