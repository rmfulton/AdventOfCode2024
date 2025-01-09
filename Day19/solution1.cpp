#include <iostream>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

pair<vector<string>, vector<string>> parseInput(string filename){
    ifstream f(filename);
    string patternString;
    f >> patternString;
    vector<string> patterns;
    int i = 0;
    string p = "";
    for (int i = 0; i < patternString.size(); ++i){
        if (patternString[i] == ","){
            continue;
        }
    }
}

bool canBeMade(vector<string> patterns, string design){

}

int main(){

    string filename = "input.txt";
    // filename = "sample.txt";
    pair<vector<string>, vector<string>> patternsAndDesigns = parseInput(filename);
    vector<string> patterns = patternsAndDesigns.first;
    vector<string> designs = patternsAndDesigns.second;
    int count = 0;
    for (string d : designs){
        if (canBeMade(patterns, d)) ++count;
    }
    cout << count << endl;
    return 0;


}