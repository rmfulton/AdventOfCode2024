#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include <vector>
#include <regex>

using namespace std;

vector<vector<char> > parseInput()
{
    string filename = "input.txt";
    ifstream infile(filename);

    string target;
    vector<vector<char> > result;
    while (infile >> target)
    {
        vector<char> row;
        for (char c : target)
        {
            row.push_back(c);
        }
        result.push_back(row);
    }
    return result;
}

vector<vector<int> > getInitialPositionAndVelocity(vector<vector<char> > grid, int N, int M)
{
    vector<int> p;
    vector<int> v;
    vector<vector<int> > both;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            switch (grid[i][j])
            {
            case '.':
                break;
            case '#':
                break;
            case '^':
                p.push_back(i);
                p.push_back(j);
                v.push_back(-1);
                v.push_back(0);
                both.push_back(p);
                both.push_back(v);
                return both;
            case '>':
                p.push_back(i);
                p.push_back(j);
                v.push_back(0);
                v.push_back(1);
                both.push_back(p);
                both.push_back(v);
                return both;
            case 'v':
                p.push_back(i);
                p.push_back(j);
                v.push_back(1);
                v.push_back(0);
                both.push_back(p);
                both.push_back(v);
                return both;
            case '<':
                p.push_back(i);
                p.push_back(j);
                v.push_back(0);
                v.push_back(-1);
                both.push_back(p);
                both.push_back(v);
                return both;
            }
        }
    }
    return both;
}

bool inBounds(vector<int> pos, int N, int M){
    return 0 <= pos[0] && pos[0] < N && 0 <= pos[1] && pos[1] < M;
}

vector<vector<int> > getNewConditions(vector<vector<char> >grid, int N, int M, vector<int> pos,vector<int> vel){
    vector<vector<int> > pair;
    vector<int> candidate;
    candidate.push_back(pos[0] + vel[0]);
    candidate.push_back(pos[1] + vel[1]);
    if (!inBounds(candidate, N, M))
    {
        pair.push_back(candidate);
        pair.push_back(vel);
    } 
    else if (grid[candidate[0]][candidate[1]] == '#')
    {
        vector<int> newVel;

        newVel.push_back(vel[1]);
        newVel.push_back(-vel[0]);
        pair.push_back(pos);
        pair.push_back(newVel);
    }
    else
    {
        pair.push_back(candidate);
        pair.push_back(vel);
    }
    return pair;
}

int main()
{
    time_t t1 = time(NULL);
    vector<vector<char> > grid = parseInput();
    const int N = grid.size();
    const int M = grid[0].size();
    vector<vector<int> > initialConditions = getInitialPositionAndVelocity(grid, N, M);
    vector<int> pos = initialConditions[0];
    vector<int> vel = initialConditions[1];

    bool visited[N][M];
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            visited[i][j] = false;
        }
    }
    grid[pos[0]][pos[1]] = '.';
    while (inBounds(pos,N,M)){
        visited[pos[0]][pos[1]] = true;
        vector<vector<int> > newConditions = getNewConditions(grid, N, M, pos,vel);
        pos = newConditions[0];
        vel = newConditions[1];
    }
    
    int totalVisited = 0;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            totalVisited += visited[i][j] ? 1 : 0;
        }
    }
    cout << totalVisited << endl;
    cout << time(NULL) - t1 << endl;
    return 0;
}