#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cmath>

#include <vector>
#include <regex>

using namespace std;

vector<vector<char>> parseIntoCharArray()
{
    string filename = "input.txt";
    ifstream infile(filename);
    vector<vector<char>> garden;
    string line;
    while (infile >> line)
    {
        vector<char> row;
        for (char c : line)
            row.push_back(c);
        garden.push_back(row);
    }
    return garden;
}

void printCharArray(vector<vector<char>> garden)
{
    for (vector<char> row : garden)
    {
        for (char c : row)
        {
            cout << c;
        }
        cout << '\n';
    }
}

int getArea(const vector<vector<int>> &region)
{
    return region.size();
}

vector<vector<int>> getNeighbors(vector<int> coords)
{
    vector<vector<int>> p;
    p.push_back((vector<int>){coords[0] - 1, coords[1]});
    p.push_back((vector<int>){coords[0] + 1, coords[1]});
    p.push_back((vector<int>){coords[0], coords[1] + 1});
    p.push_back((vector<int>){coords[0], coords[1] - 1});
    return p;
}

bool inBounds(vector<int> coords, int n, int m)
{
    // cout << "coord " << coords[0] << " " << coords[1] << " " << n << " " << m << endl;
    return 0 <= coords[0] && coords[0] < n && 0 <= coords[1] && coords[1] < m;
}

map<vector<int>, bool> getHashMap(const vector<vector<int>> &region)
{
    map<vector<int>, bool> exists;
    for (vector<int> coords : region)
    {
        exists[coords] = true;
    }
    return exists;
}

int perimeter(vector<int> point, const map<vector<int>, bool> &exists)
{
    int perimeter = 0;
    for (vector<int> neighbor : getNeighbors(point))
    {
        bool different = exists.find(neighbor) == exists.end();
        if (different)
            ++perimeter;
    }
    return perimeter;
}

int getPerimeter(const vector<vector<int>> &region)
{
    int p = 0;
    const map<vector<int>, bool> exists = getHashMap(region);
    for (vector<int> coords : region)
    {
        p += perimeter(coords, exists);
    }
    return p;
}

vector<vector<int>> getAllCoords(const vector<vector<char>> &garden)
{
    vector<vector<int>> allCoords;
    for (int i = 0; i < garden.size(); ++i)
    {
        for (int j = 0; j < garden[i].size(); ++j)
        {
            allCoords.push_back((vector<int>){i, j});
        }
    }
    return allCoords;
};

vector<vector<vector<int>>> getGroups(const vector<vector<char>> &garden)
{
    int n = garden.size();
    int m = garden[0].size();
    map<vector<int>, bool> visited;
    // cout << "getting all coords" << endl;
    vector<vector<int>> coordStack = getAllCoords(garden);

    vector<vector<vector<int>>> groups;
    while (coordStack.size() != 0)
    {
        // cout << "coordStack size " << coordStack.size() << endl;
        vector<int> front = coordStack.back();
        coordStack.pop_back();
        if (visited.find(front) != visited.end())
        {
            continue;
        }
        // cout << "coord " << front[0] << " " << front[1] << endl;
        visited[front] = true;
        vector<vector<int>> newGroup;
        vector<vector<int>> groupStack;
        groupStack.push_back(front);
        while (groupStack.size())
        {
            vector<int> back = groupStack.back();
            newGroup.push_back(back);
            groupStack.pop_back();
            for (vector<int> adj : getNeighbors(back))
            {
                if (inBounds(adj, n, m))
                {
                    // cout << "coord " << adj[0] << " " << adj[1] << " inbounds"<< endl;
                    if (garden[adj[0]][adj[1]] == garden[back[0]][back[1]])
                    {
                        if (visited.find(adj) == visited.end())
                        {
                            visited[adj] = true;
                            groupStack.push_back(adj);
                        }
                    }
                }
            }
        }
        groups.push_back(newGroup);
        // TODO: DFS or BFS of the group / region, marking each as visited
    }

    return groups;
}

/*
* First, create the groups (arrays of contiguous coordinates)
* - append every coordinate to a queue.
* - create a boolean map to track which are visited
* - 
* Second, calculate the area
* - return # of coordinates
* Third, calculate the perimeter ()
* - iterate over all elements and sum # of non-equal neighbors
*/
int main()
{
    time_t t1 = time(NULL);
    vector<vector<char>> garden = parseIntoCharArray();
    // printCharArray(garden);
    vector<vector<vector<int>>> groups = getGroups(garden);
    cout << "groups success" << endl;
    int score = 0;
    for (vector<vector<int>> group : groups)
    {
        int p = getPerimeter(group) * getArea(group);
        // cout << p << endl;
        score += p;
    }
    cout << score << endl;
    return 0;
}