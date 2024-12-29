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
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<long> vl;
typedef vector<vector<int>> vvi;
typedef vector<vector<char>> vvc;
typedef pair<vector<vector<char>>, string> pvvcs;


vi mySpecificProgram(long x){
    long alpha = x;
    long beta = 0;
    long gamma = 0;
    vi output;
    while(true){
        beta = (alpha % 8); // 2 4
        beta = beta ^ 5; // 1 5
        gamma = alpha / (1<<beta); // 7 5
        beta = beta ^ 6; // 1 6
        alpha = alpha / 8; // 0 3
        beta = beta ^ gamma; // 4 2
        output.push_back(beta % 8); // 5 5
        if (alpha == 0) break; // 3 0
    }
    return output;
}

long toBase10(string binary){
    long x = 0;
    for (char c: binary){
        x = x << 1;
        x += c == '1' ? 1 : 0;
    }
    return x;
}

string toBinary(long x){
    if (x == 0) return "0";
    string out = "";
    while (x > 0){
        out = to_string(x % 2) + out;
        x /= 2;
    }
    return out;
}

void testBinaryFunctions(){
    long x = 3;
    if (toBinary(x) != "11") throw;
    for (int i = 0; i < 1000; ++i){
        if (toBase10(toBinary(i)) != i) throw;
    }
    cout << "success";
}

void printVector(vi vector){
    for (int x : vector){
        cout << x << " ";
    }
    cout << '\n';
}
string repeat(string x, int times){
    string out;
    for (int i = 0; i < times; ++i){
        out += x;
    }
    return out;
}
/*
so here we're just computing
modulus = value % 8;
gamma = value / (1 << modulus ^ 5);
output = (modulus ^ gamma ^ 3) % 8;
FACT:
given the first 10 digits of any binary number,
you can determine what the first output will be (the same as if it was just those 10 digits)

FACT:
*/

void printOutputData(){
    for (int i = 0; i < 8*8; ++i){
        string binary = toBinary(i);
        binary =  repeat("0",6 - binary.size()) + binary;
        cout << "Input = " << binary << " -> ";
        printVector(mySpecificProgram(i));
    }
}

vi getFirstOutputMap(){
    int twoToTheEleven = 2048;
    vector<int> firstOutput;
    for (int i = 0; i < twoToTheEleven; ++i){
        vi output = mySpecificProgram(i);
        firstOutput.push_back(output[0]);
    }
    return firstOutput;
}

int main()
{
    time_t t1 = time(NULL);
    // pair<vi,vi> registersProgram = parseInput();
    // vl registers{registersProgram.first[0], registersProgram.first[1], registersProgram.first[2]};
    // vi program = registersProgram.second;
    // testMySpecificProgram(registers, program);
    // testBinaryFunctions();
    // printOutputData();
    testOutputTheory();

    cout << endl;
    cout << time(NULL) - t1 << endl;
    return 0;
}