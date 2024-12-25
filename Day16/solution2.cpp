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
map<vi,int> getMinimalCostFrom(vvc const &maze, vvi const &baseStates){

    int MOVE_COST = 1;
    int ROTATE_COST = 1000;
    map<vi, int> cost;
    queue<vi> stateQ;
    for(vi baseState: baseStates){
        cost[baseState] = 0;
        stateQ.push(baseState);
    }
    while (stateQ.size() > 0)
    {
        vi state = stateQ.front();
        stateQ.pop();

        vi rotateState1{state[0], state[1], (state[2] + 1) % 4};
        vi rotateState2{state[0], state[1], (state[2] + 3) % 4};
        vi delta = dir(state[2]);
        vi translateState{state[0] + delta[0], state[1] + delta[1], state[2]};
        char c = maze[translateState[0]][translateState[1]];
        if (c == '.' || c == 'E' || c == 'S')
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
    return cost;
}

map<vi,int> getCostFromStart(vvc const &maze){
    const vi startCoords = getCoordOf(maze, 'S');
    const vi begin{startCoords[0], startCoords[1], 0};
    const map<vi, int> costFromStart = getMinimalCostFrom(maze, vvi{begin});
    return costFromStart;
}

map<vi,int> getCostFromEnd(vvc const &maze){
    const vi endCoords = getCoordOf(maze, 'E');
    vvi endStates;
    for (int i = 0; i < 4; ++i){
        vi endState{endCoords[0], endCoords[1], i};
        endStates.push_back(endState);
    }
    const map<vi,int> costFromEnd = getMinimalCostFrom(maze, endStates);
    return costFromEnd;
}

int findNumberOfGoodPoints(map<vi,int> const &costFromStart, map<vi,int> const &costFromEnd, vvc const &maze){
    const vi startCoords = getCoordOf(maze, 'S');
    vi reversedStartCoords{startCoords[0], startCoords[1], 2};
    int bestCost = costFromEnd.at(reversedStartCoords);
    int numPointsWithOptimalPath = 0;
    for (int i = 0; i < maze.size(); ++i){
        for (int j = 0; j < maze.size(); ++j){
            bool shouldIncrementNumPoints = false;
            for (int k = 0; k < 4; ++k){
                vi stateFromBegin{i,j,k};
                vi stateFromEnd{i,j,(k+2)%4};
                if (costFromStart.find(stateFromBegin) != costFromStart.end()){
                    int totalCost = costFromStart.at(stateFromBegin) + costFromEnd.at(stateFromEnd);
                    if (totalCost == bestCost){
                        shouldIncrementNumPoints = true;
                    }
                }
            }
            if (shouldIncrementNumPoints) ++numPointsWithOptimalPath;
        }
    }
    return numPointsWithOptimalPath;
}

int solve(vvc const &maze)
{
    const map<vi,int> costFromStart = getCostFromStart(maze);
    const map<vi,int> costFromEnd = getCostFromEnd(maze);
    return findNumberOfGoodPoints(costFromStart, costFromEnd, maze);
}
void printBoard(const vvc &board){
    for (vector<char> line: board){
        for (char c: line){
            cout << c;
        }
        cout << '\n';
    }
}

/*
Now, we need t
*/
int main()
{
    time_t t1 = time(NULL);
    vvc maze = parseIntoCharArray();
    int result = solve(maze);
    cout << result << endl;
    cout << endl;
    cout << time(NULL) - t1 << endl;
    return 0;
}