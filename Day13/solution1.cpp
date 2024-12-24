#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cmath>

#include <vector>
#include <regex>

using namespace std;

vector<vector<vector<int>>> ParseInput()
{
    string filename = "input.txt";
    ifstream infile(filename);
    vector<vector<vector<int>>> machines;
    string tmp;
    while (infile >> tmp)
    {
        vector<vector<int>> Axequalsb;
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
            Axequalsb.push_back(vector<int>{x,y});
        }
        machines.push_back(Axequalsb);
    }
    return machines;
}
/*
receives an equation of the form ax + by = z
solve for (a,b) where 3a + b is minimal given 
(x,y,z)
*/
int solveDegenerate(vector<int> equation){
    int x = equation[0];
    int y = equation[1];
    int z = equation[2];
    cout << "degenerate " << x << " " << y << " " << z << endl;
    return 0;
}

int solve(vector<vector<int>> system){
    int multipleBottom = system[0][0];
    int multipleTop = system[0][1];
    vector<int> topRow{system[0][0], system[1][0], system[2][0]};
    // cout << multipleBottom << " " << multipleTop << endl;
    // cout << topRow[0] << " " << topRow[1] << " " << topRow[2] << endl;
    vector<int> newBottomRow{0, system[1][1]*multipleBottom - system[1][0]*multipleTop, system[2][1]*multipleBottom - system[2][0]*multipleTop};
    // cout << newBottomRow[0] << " " << newBottomRow[1] << " " << newBottomRow[2] << endl;
    if (newBottomRow[1] == 0 && newBottomRow[2] == 0){
        return solveDegenerate(topRow);
    } else if (newBottomRow[1] == 0) {
        return 0;
    } else {
        if (newBottomRow[2] % newBottomRow[1] != 0){
            return 0;
        }
        int t = newBottomRow[2] / newBottomRow[1];
        int RHS = topRow[2] - topRow[1]*t;
        if (RHS % topRow[0] != 0){
            return 0;
        }
        int s = RHS / topRow[0];
        if (0 <= s && s <= 100 && 0 <= t && t <= 100){
            return 3*s + t;
        }
        return 0;
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
    vector<vector<vector<int>>> linearEquations = ParseInput();
    int cost = 0;
    for(vector<vector<int>> system: linearEquations){
        cost += solve(system);
    }
    cout << endl;
    cout << cost << endl;
    cout << time(NULL) - t1 << endl;
    return 0;
}