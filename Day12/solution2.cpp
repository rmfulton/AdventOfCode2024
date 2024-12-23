#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <unordered_map>
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
Now, getPerimeter is calculated differently, as a function of the 
number of sides. That means we need to group the existing boundaries into
equivalence classes of boundaries. 
Def'n: 
    A boundary is an ordered pair of adjacent points (p1, p2), where 
    p1 is inside the region and p2 is outside the region,

Def'n:
    A Side is an equivalence class of boundaries where the 
    equivalence relation ~ is the transitive extension of 
    ((a,b), (w,x)) ~ ((c,d),(y,z))
    <=>
    (a,b) - (w,x) = (c,d) - (y,z) AND 
    (a,b) adjacent to (c,d)
*/
map<vector<vector<int>>, bool> getHashMap(const vector<vector<vector<int>>> &boundaries){
    map<vector<vector<int>>, bool> exists;
    for(vector<vector<int>> boundary: boundaries){
        exists[boundary] = true;
    }
    return exists;
}
/*
provides the Boundary in the direction rotated 90 degrees clockwise wrt the boundary
vector
*/
vector<vector<int>> getHigherBoundary(const vector<vector<int>> &currentBoundary){
    const vector<int> inside = currentBoundary[0];
    const vector<int> outside = currentBoundary[1];
    const vector<int> dir{outside[0] - inside[0], outside[1] - inside[1]};
    const vector<int> delta{dir[1], -dir[0]};
    const vector<int> newInside{inside[0] + delta[0], inside[1] + delta[1]};
    const vector<int> newOutside{outside[0] + delta[0], outside[1] + delta[1]};
    vector<vector<int>> newBoundary;
    newBoundary.push_back(newInside);
    newBoundary.push_back(newOutside);
    return newBoundary;
}
/*
provides the Boundary in the direction rotated 90 degrees clockwise wrt the boundary
vector
*/
vector<vector<int>> getLowerBoundary(const vector<vector<int>> &currentBoundary){
    const vector<int> inside = currentBoundary[0];
    const vector<int> outside = currentBoundary[1];
    const vector<int> dir{outside[0] - inside[0], outside[1] - inside[1]};
    const vector<int> delta{-dir[1], dir[0]};
    const vector<int> newInside{inside[0] + delta[0], inside[1] + delta[1]};
    const vector<int> newOutside{outside[0] + delta[0], outside[1] + delta[1]};
    vector<vector<int>> newBoundary;
    newBoundary.push_back(newInside);
    newBoundary.push_back(newOutside);
    return newBoundary;
}
bool isBoundary(vector<int> v1, vector<int> v2, const map<vector<int>,bool> &exists){
    return exists.find(v1) != exists.end() && exists.find(v2) == exists.end();
}

vector<vector<vector<int>>> getAllBoundaries(const vector<vector<int>> &region){
    map<vector<int>,bool> exists = getHashMap(region);
    vector<vector<vector<int>>> allBoundaries;
    for (vector<int> point: region){
        for (vector<int> adj: getNeighbors(point)){
            if(isBoundary(point,adj, exists)){
                vector<vector<int>> newBoundary;
                newBoundary.push_back(point);
                newBoundary.push_back(adj);
                allBoundaries.push_back(newBoundary);
            }
        }
    }
    return allBoundaries;
}


int getNumSides(const vector<vector<int>> &region){
    vector<vector<vector<int>>> allBoundaries = getAllBoundaries(region);
    map<vector<vector<int>>, bool> notIncludedYet = getHashMap(allBoundaries);
    int numSides = 0;
    for (vector<vector<int>> boundary: allBoundaries){
        if (!notIncludedYet[boundary]){
            continue; //prevents overcounting of sides
        }
        numSides += 1;
        vector<vector<int>> currentBoundary = boundary;
        while (notIncludedYet.find(currentBoundary) != notIncludedYet.end()){
            notIncludedYet[currentBoundary] = false;
            currentBoundary = getHigherBoundary(currentBoundary);
        }
        currentBoundary = boundary;
        while (notIncludedYet.find(currentBoundary) != notIncludedYet.end()){
            notIncludedYet[currentBoundary] = false;
            currentBoundary = getLowerBoundary(currentBoundary);
        }
    }
    return numSides;

}


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
        int p = getNumSides(group) * getArea(group);
        // cout << p << endl;
        score += p;
    }
    cout << score << endl;
    return 0;
}