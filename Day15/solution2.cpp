#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cmath>

#include <vector>
#include <regex>
#include <chrono>
#include <thread>

using namespace std;

vector<vector<int>> ParseInput()
{
    string filename = "input.txt";
    ifstream infile(filename);
    vector<vector<int>> initialStates;
    string p;
    string v;
    while (infile >> p)
    {
        infile >> v;
        vector<int> initialState;
        for (string s: vector<string>{p,v}){
            s = s.substr(2,s.size() - 2);
            size_t commaIndex = s.find(',');
            int x = stoi(s.substr(0,commaIndex));
            int y = stoi(s.substr(commaIndex+1, s.size() - commaIndex - 1));
            initialState.push_back(x);
            initialState.push_back(y);
        }
        initialStates.push_back(initialState);
    }
    return initialStates;
}

vector<vector<int>> evolve(const vector<vector<int>> &initialStates, const int &WIDTH, const int &HEIGHT, const int &DELTA_T){
    vector<vector<int>> finalPositions;
    // cout << "WIDTH " << WIDTH << " HEIGHT " << HEIGHT << endl;
    for (vector<int> state: initialStates){
        vector<int> position;
        // cout << state[0] << " " << state[1] << " " << state[2] << " " << state[3] << endl;
        int final_x = (state[0] + state[2]*DELTA_T + DELTA_T*WIDTH) % WIDTH;
        int final_y = (state[1] + state[3]*DELTA_T + DELTA_T*HEIGHT) % HEIGHT;
        position.push_back(final_x);
        position.push_back(final_y);
        // cout << position[0] << " " << position[1] << endl;
        finalPositions.push_back(position);
    }
    return finalPositions;
}

bool treeish(const vector<vector<int>> &positions, const int &WIDTH, const int &HEIGHT){
    int numInsideTreeRegion = 0;
    for (auto position: positions){
        float x = position[0];
        float y = position[1];
        if (abs(x - WIDTH/2) < y/2) ++numInsideTreeRegion;
    }
    return numInsideTreeRegion > (3*positions.size())/4;

}

map<vector<int>, bool> getHashMap(const vector<vector<int>> &region)
{
    map<vector<int>, bool> exists;
    for (vector<int> coords : region)
    {
        exists[coords] = true;
    }
    return exists;
}

void print(const vector<vector<int>> &positions, const int &WIDTH, const int &HEIGHT){
    map<vector<int>, bool> exists = getHashMap(positions);
    for (int j = HEIGHT - 1; j > -1; --j){
        for (int i = 0; i < WIDTH; ++i){
            if (exists.find(vector<int>{i,j}) != exists.end()){
                cout << "#";
            } else {
                cout << ".";
            }
        }
        cout << '\n';
    }
}

int main()
{
    time_t t1 = time(NULL);
    const vector<vector<int>> initialStates = ParseInput();
    // const int WIDTH = 11;
    // const int HEIGHT = 7;
    const int WIDTH = 101;
    const int HEIGHT = 103;
    const int DELTA_T = 100;
    int numTrees = 0;
    // const vector<vector<int>> finalStates = evolve(initialStates, WIDTH, HEIGHT, DELTA_T);
    for (int i = 0; i < WIDTH*HEIGHT ; ++i){
        vector< vector<int>>states = evolve(initialStates,WIDTH, HEIGHT, i);
        if (treeish(states, WIDTH, HEIGHT)){
            numTrees += 1;
            print(states, WIDTH, HEIGHT);
            cout << i << endl;
        }
    }
    cout << "numTrees: " << numTrees << endl;
    cout << time(NULL) - t1 << endl;
    return 0;
}