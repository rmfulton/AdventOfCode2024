#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

typedef vector<string> grid;

pair<vector<vector<int>>, vector<vector<int>>> parseInput(string filename)
{
    ifstream f(filename);
    string x;
    grid g;
    vector<grid> gridSet;
    while (f >> x)
    {
        // cout << x << endl;
        g.push_back(x);
        for (int i = 0; i < 6; ++i){
            f >> x;
            // cout << x << endl;
            g.push_back(x);
        }
        // cout << "yay" << endl;
        gridSet.push_back(g);
        g = {};
    }
    vector<vector<int>> keys;
    vector<vector<int>> locks;
    for (vector<string> rows : gridSet){
        vector<int> counts;
        for (int j = 0; j < 5; ++j){
            int count = 0;
            for (int i = 0; i < 7; ++i){
                if (rows[i][j] == '#'){
                    ++count;
                }
            }
            counts.push_back(count);
        }
        if (rows[0] == "....."){
            keys.push_back(counts);
        } else {
            locks.push_back(counts);
        }
    }
    return make_pair(keys, locks);

}


int main()
{
    time_t t1 = time(NULL);
    string filename = "input.txt";
    // filename = "sample.txt";
    auto data = parseInput(filename);
    vector<vector<int>> keys = data.first;
    vector<vector<int>> locks = data.second;
    // for (vector<int> k : keys){
    //     for (int x : k){
    //         cout << x << " ";
    //     }
    //     cout << endl;
    // }
    int count = 0;
    for (vector<int> k : keys){
        for (vector<int> L : locks){
            bool allFit = true;
            for (int i = 0; i < 5; ++i){
                if (k[i] + L[i] > 7){
                    allFit = false;
                    break;
                }
            }
            if (allFit){
                ++count;
            }
        }
    }
    cout << count << endl;
    cout << "took " << time(NULL) - t1 << " seconds" << endl;
    return 0;
}