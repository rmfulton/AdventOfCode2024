#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include <vector>
#include <regex>

using namespace std;

map<char, vector<vector<int> > > parseInput()
{
    string filename = "input.txt";
    ifstream infile(filename);

    string line;
    map<char, vector< vector<int> > > antennae;
    int numRows = 0;
    int numCols = 0;
    for (; infile >> line; ++numRows)
    {
        numCols = line.size();
        for (int i = 0; i < numCols; ++i){
            if (line[i] != '.'){
                vector<int> coords{numRows, i};
                if(antennae.find(line[i]) == antennae.end()){
                    vector<vector<int> > newVec;
                    antennae[line[i]] = newVec;
                }
                antennae[line[i]].push_back(coords);
            }
        }
    }
    vector<int>dimensions{numRows, numCols};
    vector<vector<int> > wrapper;
    wrapper.push_back(dimensions);
    antennae['#'] = wrapper;

    return antennae;
}
bool inBounds(vector<int> coords, int N, int M){
    return 0 <= coords[0] && coords[0] < N && 0 <= coords[1] && coords[1] < M;
}

int getAnswer(map<char, vector<vector<int> > > antennae, int N, int M){
    map<vector<int>, bool> antinodes;
    int ans = 0;

    map<char, vector<vector<int> > >::iterator it;

    for (it = antennae.begin(); it != antennae.end(); it++)
    {
        // cout << it->first << ": ";
        vector<vector<int> > coords = it->second;
        for(int i = 0; i < coords.size(); ++i){
            // cout << "(" << coords[i][0] << "," << coords[i][1] << ")";
            for (int j = 0; j < coords.size(); ++j){
                if (i == j){
                    continue;
                }
                vector<int> node{ 2*coords[j][0] - coords[i][0], 2*coords[j][1] - coords[i][1] };
                // cout << node[0] << " " << node[1] << " " << N << " " << M<< "\n";
                if (inBounds(node, N, M)) antinodes[node] = true;
            }
        }
        // cout << "\n";
    }

    return antinodes.size();
}


int main()
{
    time_t t1 = time(NULL);
    map<char, vector<vector< int> > > antennae = parseInput();
    vector<int> dimensions = antennae['#'][0];
    int N = dimensions[0];
    int M = dimensions[1];
    int answer = getAnswer(antennae, N, M);
    cout << answer << endl;
    cout << "took " << time(NULL) - t1 << " seconds" << endl;
    return 0;
}