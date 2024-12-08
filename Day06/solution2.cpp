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

bool inBounds(vector<int> pos, int N, int M)
{
    return 0 <= pos[0] && pos[0] < N && 0 <= pos[1] && pos[1] < M;
}

vector<vector<int> > getNewConditions(vector<vector<char> > grid, int N, int M, vector<int> pos, vector<int> vel)
{
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

bool loops(vector<vector<char> > grid, vector<vector<int> > guard, vector<int> obstacle)
{
    if (guard[0] == obstacle)
    {
        return false;
    }
    grid[obstacle[0]][obstacle[1]] = '#';

    vector<int> current{guard[0][0], guard[0][1], guard[1][0], guard[1][1]};
    vector<int> pos{current[0], current[1]};
    vector<int> vel{current[2], current[3]};
    map<vector<int>, bool> states;
    int N = grid.size();
    int M = grid[0].size();
    while (states.find(current) == states.end() && inBounds(pos, N,M))
    {

        states[current] = true;
        vector<vector<int> > newState = getNewConditions(grid, N,M,pos,vel);
        current = {newState[0][0], newState[0][1], newState[1][0], newState[1][1]};
        pos = {current[0], current[1]};
        vel = {current[2], current[3]};
    }
    grid[obstacle[0]][obstacle[1]] = '.';
    return inBounds(pos,N,M);
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
    time_t t2 = time(NULL);
    while (inBounds(pos, N, M))
    {
        visited[pos[0]][pos[1]] = true;
        vector<vector<int> > newConditions = getNewConditions(grid, N, M, pos, vel);
        pos = newConditions[0];
        vel = newConditions[1];
    }
    time_t t3 = time(NULL);
    cout << "tracing path took " << t3 - t2 << " seconds" << endl;

    int totalLoops = 0;
    for (int i = 0; i < N; ++i)
    {
        int actual_work = 0;
        cout << i << " of " <<  N;
        for (int j = 0; j < M; ++j)
        {
            if (visited[i][j])
            {
                ++actual_work;
                vector<int> obstacle{i, j};
                if (loops(grid, initialConditions, obstacle))
                {
                    totalLoops++;

                }

            }
        }
        cout << ". actual work: " << actual_work << endl;
    }
    cout << totalLoops << endl;
    cout << "time: " << time(NULL) - t1 << endl;
    cout << N * M << endl;
    return 0;
}