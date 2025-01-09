#include <iostream>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

/*
Parses a sequence of coordinates
*/
vector<vector<int>> getFallingCoords(string filename){
    ifstream infile(filename);
    vector<vector<int>> badBytes;
    string coords;
    while (infile >> coords){
        int commaIndex = coords.find(",");
        int i1 = stoi(coords.substr(0,commaIndex));
        int i2 = stoi(coords.substr(commaIndex+1, coords.size() - commaIndex - 1));
        badBytes.push_back({i1, i2});
    }
    return badBytes;
}

vector<vector<int>> getNeighbors(vector<int> current, const int WIDTH, const int HEIGHT, map<vector<int>, bool> illegalBytes){
    vector<int> up{current[0] - 1, current[1]};
    vector<int> down{current[0] + 1, current[1]};
    vector<int> left{current[0], current[1] - 1};
    vector<int> right{current[0], current[1] + 1};
    vector<vector<int>> candidates{up,down,left,right};
    vector<vector<int>> ret;
    for (vector<int> c : candidates){
        if (illegalBytes.find(c) != illegalBytes.end()){
            continue;
        }
        if (0 <= c[0] && c[0] < WIDTH && 0 <= c[1] && c[1] < HEIGHT){
            ret.push_back(c);
        }
    }
    return ret;
}
/*
We will create a CONSTANT three dimensional array. Zero denotes an empty space.
One denotes a blocked space. 
The time and space here is 4900 * N

Then it's just a graph traversal / dynamic programming problem
    - Create a map of visited coordinates -> graph distance from (0,0)
    - yeah, this will work.
*/
int getShortestPathLength(vector<int> startCoords, vector<int> endCoords, vector<vector<int>> fallingBytes, int numThatFell){
    map<vector<int>, bool> illegalBytes;
    for (int i = 0; i < numThatFell; ++i){
        illegalBytes[fallingBytes[i]] = true;
    }
    const int WIDTH = endCoords[0] + 1;
    const int HEIGHT = endCoords[1] + 1;
    int shortestDistance[WIDTH][HEIGHT];
    for (int i = 0; i < WIDTH; ++i){
        for (int j = 0; j < HEIGHT; ++j){
            shortestDistance[i][j] = WIDTH*HEIGHT;
        }
    }
    shortestDistance[startCoords[0]][startCoords[1]] = 0;
    vector<vector<int>> toVisit{startCoords};
    for (int i = 0; i < toVisit.size(); ++i){
        vector<int> top = toVisit[i];
        int distance = shortestDistance[top[0]][top[1]];
        vector<vector<int>> neighbors = getNeighbors(top, WIDTH, HEIGHT, illegalBytes);
        // cout << neighbors.size() << endl;
        for (vector<int> coord: neighbors){

            if (shortestDistance[coord[0]][coord[1]] > distance + 1){
                shortestDistance[coord[0]][coord[1]] = distance + 1;
                toVisit.push_back(coord);
            }
        }
    }
    // for (int i = 0; i < WIDTH; ++i){
    //     for (int j = 0; j < HEIGHT; ++j){
    //         cout << shortestDistance[j][i] << "   ";
    //     }
    //     cout << '\n';
    // }
    return shortestDistance[endCoords[0]][endCoords[1]];
}

int getFinalTime(vector<int> startCoords, vector<int> endCoords, vector<vector<int>> fallingBytes){
    int L = fallingBytes.size();
    int i = 0;
    int j = L - 1;
    int mid;
    int MAX = (endCoords[0] + 1)*(endCoords[1] + 1);
    while(j - i > 1){
        mid = (i + j)/2;
        if (getShortestPathLength(startCoords, endCoords, fallingBytes, mid) < MAX){
            i = mid;
        } else {
            j = mid;
        }
    }
    return i;
}

int main(){
    vector<int> startCoords{0,0};
    vector<int> endCoords{70,70};
    string filename = "input.txt";
    // endCoords = {6,6};
    // filename = "sample.txt";
    vector<vector<int>> fallingBytes = getFallingCoords(filename);
    int time = getFinalTime(startCoords, endCoords, fallingBytes);
    cout << fallingBytes[time][0] <<","<< fallingBytes[time][1]  << endl;
    return 0;
}