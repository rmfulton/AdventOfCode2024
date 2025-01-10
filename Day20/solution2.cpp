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

int numCheatsSavingAtLeastThreshold(const vector<string> &grid, int threshold, int max_cheat)
{
    const int NUM_ROWS = grid.size();
    const int NUM_COLS = grid[0].size();

    int d2s[NUM_ROWS][NUM_COLS];
    int d2e[NUM_ROWS][NUM_COLS];

    for (int i = 0; i < NUM_ROWS; ++i)
    {
        for (int j = 0; j < NUM_COLS; ++j)
        {
            d2s[i][j] = NUM_ROWS * NUM_COLS;
            d2e[i][j] = NUM_ROWS * NUM_COLS;
        }
    }
    vector<int> s_coords = getCoords(grid, 'S');
    vector<int> e_coords = getCoords(grid, 'E');
    d2s[s_coords[0]][s_coords[1]] = 0;
    d2e[e_coords[0]][e_coords[1]] = 0;

    vector<vector<int>> coordQs{s_coords};
    vector<vector<int>> coordQe{e_coords};
    for (int i = 0; i < coordQs.size(); ++i)
    {
        vector<int> coords = coordQs[i];
        int d = d2s[coords[0]][coords[1]];
        for (vector<int> x : getNeighbors(coords, grid))
        {
            if (d2s[x[0]][x[1]] > d + 1)
            {
                coordQs.push_back(x);
                d2s[x[0]][x[1]] = d + 1;
            }
        }
    }
    for (int i = 0; i < coordQe.size(); ++i)
    {
        vector<int> coords = coordQe[i];
        int d = d2e[coords[0]][coords[1]];
        for (vector<int> x : getNeighbors(coords, grid))
        {
            if (d2e[x[0]][x[1]] > d + 1)
            {
                coordQe.push_back(x);
                d2e[x[0]][x[1]] = d + 1;
            }
        }
    }
    cout << "computed distances to both places" << endl;
    int count = 0;
    int standard_cost = d2e[s_coords[0]][s_coords[1]];
    cout << "standard_cost: " << standard_cost << endl;;
    for (int i = 0; i < NUM_ROWS; ++i){
        for (int j = 0; j < NUM_COLS; ++j){
            // for each cheat start
            if (grid[i][j] == '#') continue;
            for (int i2 = i - max_cheat; i2 <= i + max_cheat; ++i2){
                if (i2 < 0 || i2 >= NUM_ROWS) continue;
                int excess = max_cheat - abs(i - i2);
                for (int j2 = j - excess; j2 <= j + excess; ++j2){
                    // and each cheat end
                    if (j2 < 0 || j2 >= NUM_COLS ) continue;
                    if (grid[i2][j2] == '#') continue;
                    int cheat_cost = abs(i - i2) + abs(j - j2) + d2s[i][j] + d2e[i2][j2];
                    // if (cheat_cost <= standard_cost){
                    //     cout << i << " " << j << " " << i2 << " " << j2 << " " << "cheat_cost: " << cheat_cost << endl;
                    // }
                    if (standard_cost - cheat_cost >= threshold){
                        ++count;
                    }
                }
            }

        }
    }
    return count;
}

int main()
{

    string filename = "input.txt";
    // filename = "sample.txt";
    int reductionThreshold = 100;
    int max_cheat = 20;
    vector<string> grid = parseInput(filename);
    int count = numCheatsSavingAtLeastThreshold(grid, reductionThreshold, max_cheat);
    cout << "count that reduce by at least " << reductionThreshold << ": " << count << endl;

    return 0;
}