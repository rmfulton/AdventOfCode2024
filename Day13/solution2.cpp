#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cmath>

#include <vector>
#include <regex>

using namespace std;

vector<vector<vector<long long>>> ParseInput()
{
    string filename = "input.txt";
    ifstream infile(filename);
    vector<vector<vector<long long>>> machines;
    string tmp;
    while (infile >> tmp)
    {
        vector<vector<long long>> Axequalsb;
        string stringx1;
        string stringy1;
        string stringx2;
        string stringy2;
        string stringb1;
        string stringb2;

        infile >> tmp;
        infile >> stringx1;
        infile >> stringy1;
        infile >> tmp;
        infile >> tmp;
        infile >> stringx2;
        infile >> stringy2;
        infile >> tmp;
        infile >> stringb1;
        infile >> stringb2;
        vector<string> first{stringx1, stringx2, stringb1};
        vector<string> second{stringy1, stringy2, stringb2};
        for (int i = 0; i < 3; ++i){
            string leftInt = first[i].substr(2, first[i].size() - 3);
            string rightInt = second[i].substr(2, second[i].size() - 2);
            int x = stoi(leftInt);
            int y = stoi(rightInt);
            Axequalsb.push_back(vector<long long>{x,y});
        }
        Axequalsb[2][0] += 10000000000000;
        Axequalsb[2][1] += 10000000000000;
        machines.push_back(Axequalsb);
    }
    return machines;
}
/*
receives an equation of the form ax + by = z
solve for (a,b) where 3a + b is minimal given 
(x,y,z)
*/
int solveDegenerate(vector<long long> equation){
    int x = equation[0];
    int y = equation[1];
    int z = equation[2];
    cout << "degenerate " << x << " " << y << " " << z << endl;
    return 0;
}

long long solve(vector<vector<long long>> system){
    // cout << endl;
    long long multipleBottom = system[0][0];
    long long multipleTop = system[0][1];
    vector<long long> topRow{system[0][0], system[1][0], system[2][0]};
    // cout << multipleBottom << " " << multipleTop << endl;
    // cout << topRow[0] << " " << topRow[1] << " " << topRow[2] << endl;
    vector<long long> newBottomRow{0, system[1][1]*multipleBottom - system[1][0]*multipleTop, system[2][1]*multipleBottom - system[2][0]*multipleTop};
    // cout << newBottomRow[0] << " " << newBottomRow[1] << " " << newBottomRow[2] << endl;
    if (newBottomRow[1] == 0 && newBottomRow[2] == 0){
        return solveDegenerate(topRow);
    } else if (newBottomRow[1] == 0) {
        return 0;
    } else {
        if (newBottomRow[2] % newBottomRow[1] != 0){
            return 0;
        }
        long long t = newBottomRow[2] / newBottomRow[1];
        long long RHS = topRow[2] - topRow[1]*t;
        // cout << t << " " << RHS << endl;
        if (RHS % topRow[0] != 0){
            return 0;
        }
        long long s = RHS / topRow[0];
        return 3*s + t;
    }
    return 0;
}

/*
First, define a linear system
Then, determine if it is full rank
If it is full rank 
If it is NOT, then determine the least costly solution, 
i.e., 
*/
int main()
{
    time_t t1 = time(NULL);
    vector<vector<vector<long long>>> linearEquations = ParseInput();
    long long cost = 0;
    for(vector<vector<long long>> system: linearEquations){
        cost += solve(system);
    }
    cout << endl;
    cout << cost << endl;
    cout << time(NULL) - t1 << endl;
    return 0;
}