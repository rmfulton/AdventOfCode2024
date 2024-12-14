#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include <vector>
#include <regex>

using namespace std;
vector< vector<int> > parseInput()
{
    string filename = "sample.txt";
    ifstream infile(filename);
    vector< vector<int> > result;
    string line;
    while (infile >> line){
        vector<int> row;
        string digits = "0123456789";
        for (char c: line){
            int x = digits.find(c);
            if (x != string::npos){
                row.push_back(x);
            }
        }
        result.push_back(row);
    }
    return result;
}

vector< vector<int> > getNeighbors(vector<int> source, int N, int M){
    vector<vector<int> > goodNeighbors;
    for (int i = 0; i < 4; ++i){
        int x = source[0] + (i%2)*(i - 2);
        int y = source[1] + ((i+1)%2)*(i - 1);
        vector<int> candidate{x, y };
        if (0 <= candidate[0] && 0 <= candidate[1] && candidate[0] < N && candidate[1] < M ){
            goodNeighbors.push_back(candidate);
        }
    }
    return goodNeighbors;
}

bool doesPathExist(vector<int> origin, vector<int> dest, vector<vector<int> > top){
    // cout << "trying to find path from (" << origin[0] << ", " << origin[1] << ") to (" << dest[0] << ", " << dest[1] << ")\n";
    const int N = top.size();
    const int M = top[0].size();
    bool visited[N][M];
    for (int i = 0; i < N; ++i){
        for (int j = 0; j < M; ++j){
            visited[i][j] = false;
        }
    }
    vector<vector<int> > stack;
    stack.push_back(origin);
    visited[origin[0]][origin[1]] = true;
    int count = 0;
    while (stack.size()){
        vector<vector<int> > newStack;
        for (vector<int> source: stack){
            vector<vector<int> > neighbors = getNeighbors(source, N, M);
            for (vector<int> neighbor: neighbors){
                if (visited[neighbor[0]][neighbor[1]] == true){
                    continue;
                }
                if ( top[neighbor[0]][neighbor[1]] == 1 + top[source[0]][source[1]]){
                    newStack.push_back(neighbor);
                    visited[neighbor[0]][neighbor[1]] = true;
                }
            }
        }
        ++count;
        stack = newStack;
    }
    bool pathExists = visited[dest[0]][dest[1]];
    // cout << (pathExists ? "SUCCESS" : "FAILURE") << '\n';
    return visited[dest[0]][dest[1]];
    

}
long getScoreSum(vector<vector<int>  >top)
{
    const int N = top.size();
    const int M = top[0].size();
    int total = 0;
    vector<vector<int> > origins;
    vector<vector<int> > dests;
    for(int i = 0; i< N; ++i){
        for (int j = 0; j < M; ++j){
            vector<int> newVec{i,j};
            if (top[i][j] == 0){
                origins.push_back(newVec);
            }
            if (top[i][j] == 9){
                dests.push_back(newVec);
            }
        }
    }
    for (vector<int> origin: origins){
        for(vector<int> dest : dests){
            total += doesPathExist(origin, dest, top) ? 1 : 0;
        }
    }
    return total;
}

int main()
{
    time_t t1 = time(NULL);
    vector<vector<int> > topography = parseInput();
    cout << "\ndimensions " << topography.size() << " " << topography[0].size() << '\n';
    long answer = getScoreSum(topography);
    cout << answer << endl;
    cout << "took " << time(NULL) - t1 << " seconds" << endl;
    return 0;
}