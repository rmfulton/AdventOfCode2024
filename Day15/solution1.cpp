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
typedef vector<vector<int>> vvi;
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
        firstline.push_back(c);
    }
    warehouse.push_back(firstline);
    while (infile >> line)
    {
        vector<char> row;
        for (char c : line)
            row.push_back(c);
        warehouse.push_back(row);
        bool should_break = true;
        for (char c: row){
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

vi robotCoords(const vvc &board){
    for (int i = 0; i < board.size(); ++i){
        for (int j = 0; j < board[0].size(); ++j){
            if (board[i][j] == '@') return vi{i,j};
        }
    }
    return vi{-1,-1};
}

vi newCoords(vi currentCoords, char c){
    switch(c){
        case 'v':
            return vi{currentCoords[0] +1, currentCoords[1]};
        case '^':
            return vi{currentCoords[0] -1, currentCoords[1]};
        case '>':
            return vi{currentCoords[0], currentCoords[1] + 1};
        case '<':
            return vi{currentCoords[0], currentCoords[1] - 1};
    }
    return vi{-1,-1};
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

vvc newBoard(const vvc &currentBoard, char dir){
    vvc newBoard = copyBoard(currentBoard);
    vi coords = robotCoords(currentBoard);
    vi adj = newCoords(coords, dir);
    while (currentBoard[adj[0]][adj[1]] == 'O') adj = newCoords(adj, dir);
    if (currentBoard[adj[0]][adj[1]] == '#'){
        return newBoard;
    }
    vi originalAdj = newCoords(coords, dir);
    newBoard[adj[0]][adj[1]] = 'O';
    newBoard[coords[0]][coords[1]] = '.';
    newBoard[originalAdj[0]][originalAdj[1]] = '@';
    return newBoard;

}

int hashWarehouse( const vvc &finalBoard){
    int result = 0;
    for (int i = 0; i < finalBoard.size(); ++i){
        for(int j = 0; j < finalBoard[0].size(); ++j){
            if (finalBoard[i][j] == 'O') result += 100*i + j;
        }
    }
    return result;
}

int main()
{
    time_t t1 = time(NULL);
    const pvvcs warehouseAndMovements = parseWarehouseAndMovements();
    vvc warehouse = warehouseAndMovements.first;
    string movements = warehouseAndMovements.second;
    vvc newWareHouse = copyBoard(warehouse);
    for (char c: movements){
        // cout << "\nMove " << c << ":\n";
        newWareHouse = newBoard(newWareHouse, c);
        // printBoard(newWareHouse);
    }
    cout << endl;
    cout << hashWarehouse(newWareHouse) << endl;
    cout << time(NULL) - t1 << endl;
    return 0;
}