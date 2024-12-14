#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include <vector>
#include <regex>

using namespace std;
vector<vector<int> > parseInput()
{
    string filename = "input.txt";
    ifstream infile(filename);
    vector<vector<int> > result;
    string line;
    while (infile >> line)
    {
        vector<int> row;
        string digits = "0123456789";
        for (char c : line)
        {
            int x = digits.find(c);
            if (x != string::npos)
            {
                row.push_back(x);
            }
        }
        result.push_back(row);
    }
    return result;
}

void printPaths(vector<vector<int> > paths, int N, int M){
    for (int i = 0; i < N; ++i){
        for (int j = 0; j < M; ++j){
            cout << paths[i][j];
        }
        cout << '\n';
    }
    cout << '\n';
}

void rearrangeFileSystemContents(vector<int> *fileSystem)
{
    int N = (*fileSystem).size();
    int i = 0;
    int j = N - 1;
    while (i < j)
    {
        if ((*fileSystem)[i] != -1)
        {
            ++i;
            continue;
        }
        if ((*fileSystem)[j] == -1)
        {
            --j;
            continue;
        }
        else
        {
            (*fileSystem)[i] = (*fileSystem)[j];
            (*fileSystem)[j] = -1;
            ++i;
            --j;
        }
    }
}

vector<vector<int> > getNeighbors(vector<int> source, int N, int M)
{
    vector<vector<int> > goodNeighbors;
    for (int i = 0; i < 4; ++i)
    {
        int x = source[0] + (i % 2) * (i - 2);
        int y = source[1] + ((i + 1) % 2) * (i - 1);
        vector<int> candidate{x, y};
        if (0 <= candidate[0] && 0 <= candidate[1] && candidate[0] < N && candidate[1] < M)
        {

            goodNeighbors.push_back(candidate);
        }
    }
    return goodNeighbors;
}

int howManyPaths(vector<vector<int> > origins, vector<vector<int> > dests, vector<vector<int> > top)
{
    int N = top.size();
    int M = top[0].size();
    vector<vector<int> > numPathsTo;
    for (int i = 0; i < N;++i){
        vector<int> row;
        for (int j = 0; j < M; ++j){
            row.push_back( 0);
        }
        numPathsTo.push_back(row);
    }
    for (vector<int> start: origins){
        numPathsTo[start[0]][start[1]] = 1;
    }
    vector<vector<int> > stack = origins;
    for (int i = 0; i < 9; ++i){
        vector<vector<int> > newStack;
        // cout << "STACK\n";
        for (vector<int> source: stack){
            // cout << source[0] << " " << source[1] << '\n';
            for (vector<int> adj: getNeighbors(source, N, M)){
                if (top[adj[0]][adj[1]] == i+1){
                    if (numPathsTo[adj[0]][adj[1]] == 0){
                        newStack.push_back(adj);
                    }
                    numPathsTo[adj[0]][adj[1]] += numPathsTo[source[0]][source[1]];
                }
            }
        }
        // cout << "Step " << i << "\n\n";
        stack = newStack;
    }

    int sumPaths = 0;
    for (vector<int> end: dests){
        sumPaths += numPathsTo[end[0]][end[1]];
    }
    return sumPaths;  
}

long getScoreSum(vector<vector<int> > top)
{
    const int N = top.size();
    const int M = top[0].size();
    int total = 0;
    vector<vector<int> > origins;
    vector<vector<int> > dests;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            vector<int> newVec{i, j};
            if (top[i][j] == 0)
            {
                origins.push_back(newVec);
            }
            if (top[i][j] == 9)
            {
                dests.push_back(newVec);
            }
        }
    }
    return howManyPaths(origins, dests, top);
}

int main()
{
    time_t t1 = time(NULL);
    vector<vector<int> > topography = parseInput();
    printPaths(topography, topography.size(), topography[0].size());
    cout << "\ndimensions " << topography.size() << " " << topography[0].size() << '\n';
    long answer = getScoreSum(topography);
    cout << answer << endl;
    cout << "took " << time(NULL) - t1 << " seconds" << endl;
    return 0;
}