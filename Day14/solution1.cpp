#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cmath>

#include <vector>
#include <regex>

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

int getAnswer(const vector<vector<int>> &finalStates, const int &WIDTH, const int &HEIGHT){
    int x_small_y_small = 0;
    int x_big_y_small = 0;
    int x_small_y_big = 0;
    int x_big_y_big = 0;
    for(vector<int> state: finalStates){
        int x = state[0];
        int y = state[1];
        bool xsmall = 2*x < WIDTH - 1;
        bool ysmall = 2*y < HEIGHT - 1;
        bool xbig = 2*x > WIDTH - 1;
        bool ybig = 2*y > HEIGHT - 1;
        if (xsmall && ysmall){
            ++x_small_y_small;
        }
        if (xbig && ysmall){
            ++x_big_y_small;
        }
        if (xsmall && ybig){
            ++x_small_y_big;
        }
        if (xbig && ybig){
            ++x_big_y_big;
        }
    }
    return x_small_y_small * x_big_y_small * x_small_y_big * x_big_y_big;
}

int main()
{
    time_t t1 = time(NULL);
    const vector<vector<int>> initialStates = ParseInput();
    const int WIDTH = 101;
    const int HEIGHT = 103;
    const int DELTA_T = 100;
    int cost = 0;
    const vector<vector<int>> finalStates = evolve(initialStates, WIDTH, HEIGHT, DELTA_T);
    int answer = getAnswer(finalStates, WIDTH, HEIGHT);
    cout << endl;
    cout << answer << endl;
    cout << time(NULL) - t1 << endl;
    return 0;
}