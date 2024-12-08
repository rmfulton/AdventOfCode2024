#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include <vector>
#include <regex>

using namespace std;
void print(vector<vector<long> > rows){
    for (vector<long> r : rows){
        cout << "row: ";
        for (long v: r){
            cout << v << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

long concat(long x, long y){
    long z = y;
    while (z != 0){
        z = z/10;
        x = x*10;
    }
    return x + y;
}

vector<vector<long> > parseInput()
{
    string filename = "input.txt";
    ifstream infile(filename);

    string line;
    vector<vector<long> > result;
    while (infile >> line)
    {
        long value;
        long colonIndex = line.find(':');
        istringstream ss(line);
        ss >> value;
        if (colonIndex != string::npos){
            vector<long> newRow;
            result.push_back(newRow);
        }
        result.back().push_back(value);
    }
    return result;
}
void printMatchCall(long target, vector<long> sequence){
    cout << "target " << target << "\n";
    cout << "sequence: ";
    for (long e: sequence) { 
        cout << e << " ";
    }
    cout << "\n\n";    
}

bool matches(long target, vector<long> sequence){
    // printMatchCall(target, sequence);
    if (sequence.size() == 1){
        return target == sequence[0];
    }
    long last = sequence.back(); sequence.pop_back();
    long before = sequence.back(); sequence.pop_back();
    //try addition
    sequence.push_back(last + before);
    if ( matches(target, sequence)){
        return true;
    }
    sequence.pop_back();

    // try multiplication
    sequence.push_back(last * before);
    if ( matches(target, sequence)){
        return true;
    }
    sequence.pop_back();

    // try concat
    sequence.push_back(concat(last,before));
    if ( matches(target, sequence)){
        return true;
    }
    sequence.pop_back();
    return false;
}

long getAnswer(vector<vector<long> > rows){
    long ans = 0;
    for (vector<long> v: rows)
    {
        vector<long> reverse;
        long size = v.size();
        for (long i = size - 1; i > -1; --i){
            reverse.push_back(v[i]);
        }
        long target = reverse.back(); reverse.pop_back();
        // printMatchCall(target,reverse);
        ans += matches(target, reverse) ? target : 0;
    }
    return ans;
}


int main()
{
    time_t t1 = time(NULL);
    vector<vector< long > > rows = parseInput();
    long totalSolvable = getAnswer(rows);
    cout << totalSolvable << endl;
    cout << "took " << time(NULL) - t1 << " seconds" << endl;
    return 0;
}