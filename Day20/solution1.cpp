#include <iostream>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

vector<string> parseInput(string filename)
{
    ifstream f(filename);
    vector<string> grid;
    string instring;
    while (f >> instring)
    {
        grid.push_back(instring);
    }
    return grid;
}

vector<int> getCoords(const vector<string> &grid, char c)
{
    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[0].size(); ++j)
        {
            if (grid[i][j] == c)
            {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

vector<vector<int>> getNeighbors(vector<int> coord, const vector<string> &grid)
{
    const int WIDTH = grid.size();
    const int HEIGHT = grid[0].size();
    vector<vector<int>> candidates{{coord[0], coord[1] - 1}, {coord[0], coord[1] + 1}, {coord[0] + 1, coord[1]}, {coord[0] - 1, coord[1]}};
    vector<vector<int>> ret;
    for (vector<int> k : candidates)
    {
        if (0 <= k[0] && k[0] < WIDTH && 0 <= k[1] && k[1] < HEIGHT)
        {
            if (grid[k[0]][k[1]] != '#')
            {
                ret.push_back(k);
            }
        }
    }
    return ret;
}

int distanceFromSToEWithCheats(vector<string> grid, int num_cheats_allowed)
{
    const int NUM_ROWS = grid.size();
    const int NUM_COLS = grid[0].size();
    const int NUM_CHEAT_STATES = num_cheats_allowed + 1;
    int states[NUM_ROWS][NUM_COLS][NUM_CHEAT_STATES];
    throw;
}

int distanceFromSToE(const vector<string> &grid)
{
    const int NUM_ROWS = grid.size();
    const int NUM_COLS = grid[0].size();
    int distance[NUM_ROWS][NUM_COLS];
    for (int i = 0; i < NUM_ROWS; ++i)
    {
        for (int j = 0; j < NUM_COLS; ++j)
        {
            distance[i][j] = NUM_ROWS * NUM_COLS;
        }
    }
    vector<int> s_coords = getCoords(grid, 'S');
    distance[s_coords[0]][s_coords[1]] = 0;

    vector<vector<int>> coordQ{s_coords};
    for (int i = 0; i < coordQ.size(); ++i)
    {
        vector<int> coords = coordQ[i];
        int d = distance[coords[0]][coords[1]];
        for (vector<int> x : getNeighbors(coords, grid))
        {
            if (distance[x[0]][x[1]] > d + 1)
            {
                coordQ.push_back(x);
                distance[x[0]][x[1]] = d + 1;
            }
        }
    }
    vector<int> e_coords = getCoords(grid, 'E');
    return distance[e_coords[0]][e_coords[1]];
}

bool hasTwoEmptyNeighbors(int i, int j, const vector<string> &grid){
    vector<vector<int>> neighbors = getNeighbors({i,j}, grid);
    return neighbors.size() >= 2;
}

int main()
{

    string filename = "input.txt";
    // filename = "sample.txt";
    int reductionThreshold = 100;
    vector<string> grid = parseInput(filename);
    int D = distanceFromSToE(grid);
    cout << "vanilla distance is " << D << endl;
    int count = 0;
    for (int i = 0; i < grid.size(); ++i)
    {
        cout << i << " of " << grid.size() << endl;
        for (int j = 0; j < grid.size(); ++j)
        {
            char tmp = grid[i][j];
            if (tmp == '#' && hasTwoEmptyNeighbors(i,j,grid))
            {
                grid[i][j] = '.';
                int x = distanceFromSToE(grid);
                if (x + reductionThreshold <= D)
                {
                    ++count;
                }
                grid[i][j] = tmp;
            }
        }
    }
    cout << "count that reduce by at least" << reductionThreshold << ": " << count << endl;

    return 0;
}