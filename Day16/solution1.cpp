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
typedef vector<vector<int>> vvi;
typedef vector<vector<char>> vvc;
typedef pair<vector<vector<char>>, string> pvvcs;

vector<vector<char>> parseIntoCharArray()
{
    string filename = "input.txt";
    ifstream infile(filename);
    vector<vector<char>> maze;
    string line;
    while (infile >> line)
    {
        vector<char> row;
        for (char c : line)
            row.push_back(c);
        maze.push_back(row);
    }
    return maze;
}
vi dir(int i)
{
    switch (i)
    {
    case 0:
        return vi{0, 1};
    case 1:
        return vi{-1, 0};
    case 2:
        return vi{0, -1};
    case 3:
        return vi{1, 0};
    }
    throw;
}

int newValue(vi state, int proposedValue, map<vi, int> const &cost)
{
    if (cost.find(state) == cost.end())
    {
        return proposedValue;
    }
    return cost.at(state) < proposedValue ? cost.at(state) : proposedValue;
}

bool improves(vi const &state, int const &proposedValue, map<vi, int> &cost)
{
    if (cost.find(state) == cost.end())
    {
        return true;
    }
    return proposedValue < cost[state];
}

vi getCoordOf(vvc const &maze, char c){
    for (int i = 0; i < maze.size(); ++i){
        for (int j = 0; j < maze[0].size(); ++j){
            if (maze[i][j] == c) return vi{i,j};
        }
    }
    throw;
}

int solve(vvc const &maze)
{
    int MOVE_COST = 1;
    int ROTATE_COST = 1000;
    map<vi, int> cost;
    vi startCoords = getCoordOf(maze, 'S');
    queue<vi> stateQ;
    vi begin{startCoords[0], startCoords[1], 0};
    cost[begin] = 0;
    stateQ.push(begin);
    while (stateQ.size() > 0)
    {
        vi state = stateQ.front();
        stateQ.pop();

        vi rotateState1{state[0], state[1], (state[2] + 1) % 4};
        vi rotateState2{state[0], state[1], (state[2] + 3) % 4};
        vi delta = dir(state[2]);
        vi translateState{state[0] + delta[0], state[1] + delta[1], state[2]};
        if (maze[translateState[0]][translateState[1]] == '.' || maze[translateState[0]][translateState[1]] == 'E')
        {
            int proposedCost = MOVE_COST + cost[state];
            if (improves(translateState, proposedCost, cost))
            {
                stateQ.push(translateState);
                cost[translateState] = proposedCost;
            }
        }
        int proposedCost = ROTATE_COST + cost[state];
        for (vi rotateState : vvi{rotateState1, rotateState2})
        {
            if (improves(rotateState, proposedCost, cost))
            {
                stateQ.push(rotateState);
                cost[rotateState] = proposedCost;
            }
        }
    }
    vi endCoords = getCoordOf(maze, 'E');
    int bestCost = 10000000;
    for (int i = 0; i < 4; ++i){
        vi state{endCoords[0], endCoords[1], i};
        if (cost[state] < bestCost) bestCost = cost[state];
    }
    return bestCost;
}
/*

*/
int main()
{
    time_t t1 = time(NULL);
    vvc maze = parseIntoCharArray();

    cout << solve(maze) << endl;
    cout << endl;
    cout << time(NULL) - t1 << endl;
    return 0;
}