#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cmath>

#include <vector>
#include <regex>

using namespace std;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<vvi> vvvi;
typedef vector<vector<char>> vvc;
typedef pair<vector<vector<char>>, string> pvvcs;


pvvcs parseWarehouseAndMovements()
{
    string filename = "input.txt";
    ifstream infile(filename);
    vvc warehouse;
    string line;
    infile >> line;
    vector<char> firstline;
    for (char c: line){
        firstline.push_back('#');
        firstline.push_back('#');
    }
    warehouse.push_back(firstline);
    while (infile >> line)
    {
        vector<char> row;
        for (char c : line){
            switch(c){
                case '.':
                    row.push_back('.');
                    row.push_back('.');
                    break;
                case '#':
                    row.push_back('#');
                    row.push_back('#');
                    break;
                case '@':
                    row.push_back('@');
                    row.push_back('.');
                    break;
                case 'O':
                    row.push_back('[');
                    row.push_back(']');
                    break;
            }
        }
        warehouse.push_back(row);
        bool should_break = true;
        for (char c: line){
            if (c != '#') should_break = false;
        }
        if (should_break) break;
    }
    string movements = "";
    while (infile >> line){
        movements += line;
    }
    return make_pair(warehouse, movements);

}

void printBoard(const vvc &board){
    for (vector<char> line: board){
        for (char c: line){
            cout << c;
        }
        cout << '\n';
    }
}

vvc copyBoard(const vvc &b){
    vvc newBoard;
    for (vector<char> v : b){
        vector<char> newv;
        for (char c: v){
            newv.push_back(c);
        }
        newBoard.push_back(newv);
    }
    return newBoard;
}

vi getRobotCoords(const vvc &board){
    for (int i = 0; i < board.size(); ++i){
        for (int j = 0; j < board[0].size(); ++j){
            if (board[i][j] == '@') return vi{i,j};
        }
    }
    return vi{-1,-1};
}

int computeHash(vvc currentState){
    int result = 0;
    for (int i = 0; i < currentState.size(); ++i){
        for (int j = 0; j < currentState[0].size(); ++j){
            if (currentState[i][j] == '['){
                result += 100*i + j;
            }
        }
    }
    return result;
}

bool containsWall(vvi const &candidateCoords, vvc const &currentState){
    for (vi coord: candidateCoords){
        if (currentState[coord[0]][coord[1]] == '#') return true;
    }
    return false;
}

bool allSpace(const vvi &candidateCoords, const vvc &currentState){
    for (vi coord: candidateCoords){
        if (currentState[coord[0]][coord[1]] != '.') return false;
    }
    return true;
}

vi dir(char c){
    switch (c){
        case '<':
            return vi{0,-1};
        case '>':
            return vi{0,1};
        case 'v':
            return vi{1,0};
        case '^':
            return vi{-1,0};
    }
    return {10000000, 10000000};
}

vvi getNextLayer(const vvc &currentState, vvi currentLayer, char c){
    map<vi, bool> nextLayerSet;
    vector<int> delta = dir(c);
    for(vi element: currentLayer){
        vi destination{element[0] + delta[0], element[1] + delta[1]};
        char v = currentState[destination[0]][destination[1]];
        if (v == '.') continue;
        if (v == '#'){
            nextLayerSet[destination] = true;
            break;
        }
        if (v == ']'){
            nextLayerSet[destination] = true;
            if (c == '^' || c == 'v'){
                nextLayerSet[vi{destination[0], destination[1] - 1}] = true;
            }
        }
        if (v == '['){
            nextLayerSet[destination] = true;
            if (c == '^' || c == 'v'){
                nextLayerSet[vi{destination[0], destination[1] + 1}] = true;
            }
        }
    }
    vvi nextLayer;
    for (auto const &x: nextLayerSet){
        nextLayer.push_back(x.first);
    }
    return nextLayer;
}

/*
High Level:
- Phase I (while true) [Create a stack of layers]
- get the next layer of coords
- if the layer contains an obstacle,
    - return the previous state
- if the layer is "....", break
- Phase II (while stack.size() > 0)
-   pop the most recent layer
-   move it to where it belongs
-   replace it with empty space
- return the final state

*/
vvc updateState(const vvc &currentState, char c){
    vi startCoords = getRobotCoords(currentState);
    vvvi layerStack;
    layerStack.push_back(vvi{startCoords});
    while (true){
        vvi nextLayer = getNextLayer(currentState, layerStack.back(), c);
        if (containsWall(nextLayer, currentState)) return currentState;

        if (allSpace(nextLayer, currentState)) break;

        layerStack.push_back(nextLayer);
    }

    vvc newState = copyBoard(currentState);
    vi delta = dir(c);
    while (layerStack.size() > 0){
        vvi layer = layerStack.back();
        layerStack.pop_back();
        for (vi coord: layer){
            newState[coord[0] + delta[0]][coord[1] + delta[1]] = currentState[coord[0]][coord[1]];
            newState[coord[0]][coord[1]] = '.';
        }
    }
    return newState;
}

/*
Now that things got fat, should we model continue to model it as a char grid?
when you push, there is now a set of coordinates that move in the direction
*/
int main()
{
    time_t t1 = time(NULL);
    const pvvcs warehouseAndMovements = parseWarehouseAndMovements();
    vvc warehouse = warehouseAndMovements.first;
    string movements = warehouseAndMovements.second;
    vvc currentState = copyBoard(warehouse);
    for (char c: movements){
        currentState = updateState(currentState, c);
        // printBoard(currentState);
    }
    cout << computeHash(currentState) << endl;
    cout << time(NULL) - t1 << endl;
    return 0;
}