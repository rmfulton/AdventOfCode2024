#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cmath>
#include <queue>

#include <vector>
#include <regex>

using namespace std;

////////////////////////////////////////////////////
////////////// minor helper functions //////////////
////////////////////////////////////////////////////

vector<int> getExpectedOutput()
{
    return vector<int>{2, 4, 1, 5, 7, 5, 1, 6, 0, 3, 4, 2, 5, 5, 3, 0};
}
void printPatterns(vector<string> patterns){
    for (string s : patterns){
        cout << s << '\n';
    }
}
string repeat(string base, int times)
{
    string ret = "";
    for (int i = 0; i < times; ++i)
    {
        ret += base;
    }
    return ret;
}

string basePattern(int numberOutputs)
{
    int padding = 11;
    return repeat("X", 3 * numberOutputs + padding);
}

string replaceXWithZero(string p)
{
    string out = "";
    for (char c : p)
    {
        out += c == 'X' ? '0' : c;
    }
    return out;
}

long toBase10(string binary)
{
    long out = 0;
    for (char c : binary)
    {
        out = out << 1;
        if (c == '1')
            out += 1;
    }
    return out;
}

string mergedOrEmpty(string pattern1, string pattern2){
    string mergedPattern = "";
    char wildcard = 'X';
    for (int i = 0; i < pattern1.size(); ++i){
        char c1 = pattern1[i];
        char c2 = pattern2[i];
        if (c1 == wildcard)
        {
            mergedPattern += c2;
            continue;
        }
        if (c2 == wildcard)
        {
            mergedPattern += c1;
            continue; 
        }
        if (c1 != c2){
            return "";
        }
        mergedPattern += c1;
    }
    return mergedPattern;
}

int differences(string s1, string s2){
    int out = 0;
    for (int i = 0; i < s1.size(); ++i){
        out += s1[i] == s2[i] ? 0 : 1;
    }
    return out;
}


string getTheOr(string s1, string s2){
    string out = "";
    for (int i = 0; i < s1.size(); ++i){
        if (s1[i] == s2[i]){
            out += s1[i];
        } else {
            out += 'X';
        }
    }
    return out;
}
vector<string> reducePatterns(vector<string> patterns){
    int numPatterns = patterns.size();
    int previousNumPatterns = numPatterns + 1;
    while (numPatterns < previousNumPatterns){
        map<string,bool> newPatternsMap;
        for (int i = 0; i < patterns.size(); ++i){
            bool hasAMatch = false;
            string matchesBoth;
            for (int j = 0; j < patterns.size(); ++j){
                if (differences(patterns[i], patterns[j]) == 1){
                    hasAMatch = true;
                    matchesBoth = getTheOr(patterns[i], patterns[j]);
                    break;
                }
            }
            if (!hasAMatch){
                newPatternsMap[patterns[i]] = true;
            } else {
                newPatternsMap[matchesBoth] = true;
            }
        }
        vector<string> newPatterns;
        for (pair< const string, bool> item : newPatternsMap){
            newPatterns.push_back(item.first);
        }
        previousNumPatterns = numPatterns;
        patterns = newPatterns;
        numPatterns = patterns.size();
    }
    return patterns;
}

int specialFunctionOneIter(int input){
    int beta = input % 8; // 2
    beta = beta ^ 5; // 7
    int gamma = input >> beta; //6
    beta = beta ^ 6; // 1
    return (beta ^ gamma) % 8;
}

string to_binary(int i){
    string out = "";
    while (i > 0){
        if (i % 2){
            out = '1' + out;
        } else {
            out = '0' + out;
        }
        i = i >> 1;
    }
    return out;
}

////////////////////////////////////////////////////
////////////// major helper functions //////////////
////////////////////////////////////////////////////
// "11111111111"
vector<string> getBinarySequencesMatchingOutput(int output){
    int max_amount = 2048;
    int log_max_amount = 11;
    vector<string> out;
    for (int i = 0; i < max_amount; ++i){
        if (specialFunctionOneIter(i) == output){
            string s = to_binary(i);
            s = repeat("0", log_max_amount - s.size()) + s;
            out.push_back(s);
        }
    }
    return out;
}

vector<string> getNewPatterns(int output, int index){
    vector<string> binarySequencesMatchingOutput = getBinarySequencesMatchingOutput(output);
    int basePatternLength = basePattern(16).size();
    int binarySequenceLength = binarySequencesMatchingOutput[0].size();
    vector<string> out;
    for (string sequence : binarySequencesMatchingOutput){
        string full = repeat("X", basePatternLength - binarySequenceLength - 3*index) + sequence + repeat("X", 3*index);
        out.push_back(full);
    }
    return out;
}


vector<string> updatePatterns(vector<string> currentPatterns, int output, int index)
{
    vector<string> newPatterns = getNewPatterns(output, index);
    vector<string> satisfiesBoth;
    for (string p1 : currentPatterns)
    {
        for (string p2 : newPatterns)
        {
            string mergedPattern = mergedOrEmpty(p1, p2);
            if (mergedPattern.size() != 0)
            {
                satisfiesBoth.push_back(mergedPattern);
            }
        }
    }
    vector<string> compacted = reducePatterns(satisfiesBoth);
    return compacted;
}

long smallestLongMatchingOneOf(vector<string> patterns)
{
    long smallest = -1;
    for (string p : patterns)
    {
        string binary = replaceXWithZero(p);
        long candidate = toBase10(binary);
        if (smallest == -1)
            smallest = candidate;
        if (candidate < smallest)
            smallest = candidate;
    }
    return smallest;
}

long computeSmallestMatch(vector<int> output)
{
    cout << "computing smallestMatch" << endl;
    vector<string> patterns{basePattern(output.size())};
    cout << "first pattern is " << patterns[0] << endl;
    for (int i = 0; i < output.size(); ++i)
    {
        int x = output[i];
        patterns = updatePatterns(patterns, x, i);
        cout << " on iteration " << i << ", there are " << patterns.size() << " new patterns corresponding to " << x << endl;
        (void)printPatterns(patterns);
    }
    return smallestLongMatchingOneOf(patterns);
}

///////////////////////////////////////////
////////////// main function //////////////
///////////////////////////////////////////

int main()
{
    vector<int> expectedOutput = getExpectedOutput();
    cout << computeSmallestMatch(expectedOutput) << endl;
    return 0;
}