#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

pair<map<string,int>, vector<vector<bool>>> parseInput(string filename)
{
    ifstream f(filename);
    map<string,int> ordering;
    vector<pair<string,string>> edges;
    string x;
    while (f >> x)
    {
        int j = x.find('-');
        string s1 = x.substr(0,j);
        string s2 = x.substr(j+1, x.size() - j - 1);
        edges.push_back(make_pair(s1,s2));
    }
    int N = 0;
    for (pair<string,string> e : edges){
        for (string name : {e.first, e.second}){
            if (ordering.find(name) == ordering.end()){
                ordering[name] = N++;
            }
        }
    }
    vector<vector<bool>> adj;
    for (int i = 0; i < N; ++i){
        vector<bool> row;
        for (int j = 0; j < N; ++j)
            row.push_back(false);
        adj.push_back(row);
    }

    for (pair<string,string> e : edges){
        adj[ordering[e.first]][ordering[e.second]] = true;
        adj[ordering[e.second]][ordering[e.first]] = true;
    }
    return make_pair(ordering, adj);

}

long solution(map<string,int> ordering, vector<vector<bool>> adj){
    const int N = ordering.size();
    bool startsWithT[N];
    for (pair<const string,int> x : ordering)
        startsWithT[x.second] = x.first[0] == 't';
    long totalNumSets = 0;
    for (int i = 0; i < N; ++i){
        cout << i << " of " << N << endl;
        for (int j = i+1; j < N; ++j){
            for (int k = j+1; k < N; ++k){
                bool allConnected = adj[i][j] && adj[j][k] && adj[i][k];
                bool atLeastOneStartsWithT = startsWithT[i] || startsWithT[j] || startsWithT[k];
                if (allConnected && atLeastOneStartsWithT)
                    ++totalNumSets;
            }
        }
    }
    return totalNumSets;
}

int main()
{
    time_t t1 = time(NULL);
    string filename = "input.txt";
    // filename = "sample.txt";
    pair<map<string,int>, vector<vector<bool>>> data = parseInput(filename);
    long output = solution(data.first, data.second);
    cout << output << endl;
    cout << "took " << time(NULL) - t1 << " seconds" << endl;
    return 0;
}