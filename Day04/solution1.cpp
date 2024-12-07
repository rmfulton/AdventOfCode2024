#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <vector>
#include <regex>

using namespace std;

vector<vector<char> > parseInput()
{
    string filename = "input.txt";
    ifstream infile(filename);

    string target;
    vector< vector <char> > result;
    while (infile >> target)
    {
        vector<char> row;
        for (char c : target){
            row.push_back(c);
        }
        result.push_back(row);
    }
    return result;
}
/**
 * A B C
 * D E F
 * G H I
 * ->
 * G D A
 * H E B
 * I F C
 * 
 **/
vector<vector<char> > rotate90(vector<vector<char> > grid){
    vector<vector<char> > rot;
    int rows = grid.size();
    int cols = grid[0].size();
    for (int i = 0; i < cols; ++i){
        vector<char> rot_column;
        for (int j = rows - 1; j > -1; --j){
            rot_column.push_back(grid[j][i]);
        }
        rot.push_back(rot_column);
    }
    return rot;
}

int countHorizontal(vector<vector<char> > grid){
    int cols = grid[0].size();
    int count = 0;
    for( vector<char> r : grid){
        for (int i = 0; i +3 < cols; ++i){
            if (r[i] == 'X' && r[i+1] == 'M' && r[i+2] == 'A' && r[i+3] == 'S'){
                count += 1;
            }
        }
    }
    return count;
}

int countDiagonal(vector<vector<char> > grid){
    int cols = grid[0].size();
    int rows = grid.size();
    int count = 0;
    for( int i = 0; i+3 < rows; ++i){
        for (int j = 0; j +3 < cols; ++j){
            if (grid[i][j] == 'X' && grid[i+1][j+1] == 'M' && grid[i+2][j+2] == 'A' && grid[i+3][j+3] == 'S'){
                count += 1;
            }
        }
    }
    return count;
}

int howMany(vector<vector<char> > grid){
    int total = 0;
    for (int i = 0; i < 4; ++i){
        total += countHorizontal(grid) + countDiagonal(grid);
        grid = rotate90(grid);
    }
    return total;
}


int main()
{
    vector<vector<char> > grid = parseInput();
    int rows = grid.size();
    int cols = grid[0].size();
    int total = howMany(grid);
    cout << total << endl;
    return 0;
}