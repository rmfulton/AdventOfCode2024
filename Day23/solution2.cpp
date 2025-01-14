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

bool checkIfAllConnected(const vector<vector<bool>> &adj, vector<int> points){
    for (int i = 0; i < points.size(); ++i)
        for (int j = i+1; j < points.size(); ++j)
            if (adj[points[i]][points[j]] == false)
                return false;
    return true;
}

bool checkIfAnyStartWithT(bool *startsWithT, vector<int> points){
    for (int p : points)
        if (startsWithT[p])
            return true;
    return false;
}
/*
the number of 5 cliques is 45045, from all 520 points. Therefore the largest clique is at most 24, since a 25-clique begets 53k 5-cliques.
the number of 6 cliques is 55770, from 520 points. Therefore the largest clique is at most 21, since a 22-clique begets 74k 6-cliques
the number of 7 cliques is 50622, from 520  points.
8 cliques 33462, 520 points
9 cliques 15730, 520 points 
10 cliques 5005, 520 points. therefore the largest clique is at most 15, since a 16 clique would beget 8k 10-cliques.
11 cliques 975. therefore the largest clique is at most 14, since a 15 clique would beget 1.3k 11-cliques
12 cliques 91.
*/
long solution(map<string,int> ordering, vector<vector<bool>> adj){
    const int N = ordering.size();
    bool inACliqueOf13[N];
    for (pair<const string,int> x : ordering){
        inACliqueOf13[x.second] = false;
    }
    long totalNumSets = 0;
    for (int i1 = 0; i1 < N; ++i1){
        cout << i1 << " of " << N << ": " << totalNumSets << endl;
        for (int i2 = i1+1; i2 < N; ++i2){
            if (adj[i1][i2] == false)
                continue;
            for (int i3 = i2+1; i3 < N; ++i3){
                if (!adj[i1][i3] || !adj[i2][i3])
                    continue;
                for (int i4=i3+1; i4 < N; ++i4){
                    if (!adj[i1][i4] || !adj[i2][i4] || !adj[i3][i4])
                        continue;
                    for (int i5 = i4+1; i5 < N; ++i5){
                        if (!adj[i1][i5] || !adj[i2][i5] || !adj[i3][i5] || !adj[i4][i5])
                            continue;
                        for (int i6 = i5+1; i6 < N; ++i6){
                            if (!adj[i1][i6] || !adj[i2][i6] || !adj[i3][i6] || !adj[i4][i6] || !adj[i5][i6])
                                continue;
                            for (int i7 = i6+1; i7 < N; ++i7){
                                if (!adj[i1][i7] || !adj[i2][i7] || !adj[i3][i7] || !adj[i4][i7] || !adj[i5][i7] || !adj[i6][i7])
                                    continue;
                                for (int i8 = i7+1; i8 < N; ++i8){
                                    if (!adj[i1][i8] || !adj[i2][i8] || !adj[i3][i8] || !adj[i4][i8] || !adj[i5][i8] || !adj[i6][i8] || !adj[i7][i8])
                                        continue;
                                    for (int i9 = i8+1; i9 < N; ++i9){
                                        if (!adj[i1][i9] || !adj[i2][i9] || !adj[i3][i9] || !adj[i4][i9] || !adj[i5][i9] || !adj[i6][i9] || !adj[i7][i9] || !adj[i8][i9])
                                            continue;
                                        for (int i10 = i9+1; i10 < N; ++i10){
                                            if (!adj[i1][i10] || !adj[i2][i10] || !adj[i3][i10] || !adj[i4][i10] || !adj[i5][i10] || !adj[i6][i10] || !adj[i7][i10] || !adj[i8][i10] || !adj[i9][i10])
                                                continue;
                                            for (int i11 = i10+1; i11 < N; ++i11){
                                                if (!adj[i1][i11] || !adj[i2][i11] || !adj[i3][i11] || !adj[i4][i11] || !adj[i5][i11] || !adj[i6][i11] || !adj[i7][i11] || !adj[i8][i11] || !adj[i9][i11] || !adj[i10][i11])
                                                    continue;
                                                for (int i12 = i11+1; i12 < N; ++i12){
                                                    if (!adj[i1][i12] || !adj[i2][i12] || !adj[i3][i12] || !adj[i4][i12] || !adj[i5][i12] || !adj[i6][i12] || !adj[i7][i12] || !adj[i8][i12] || !adj[i9][i12] || !adj[i10][i12] || !adj[i11][i12])
                                                        continue;
                                                    for (int i13 = i12+1; i13 < N; ++i13){
                                                        if (!adj[i1][i13] || !adj[i2][i13] || !adj[i3][i13] || !adj[i4][i13] || !adj[i5][i13] || !adj[i6][i13] || !adj[i7][i13] || !adj[i8][i13] || !adj[i9][i13] || !adj[i10][i13] || !adj[i11][i13] || !adj[i12][i13])
                                                            continue;
                                                        ++totalNumSets;
                                                        for (int y : {i1,i2,i3,i4,i5,i6,i7,i8,i9,i10,i11,i12,i13}){
                                                            inACliqueOf13[y] = true;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for (auto p : ordering){
        if (inACliqueOf13[p.second]){
            cout << p.first << ",";
        }
    }
    cout << endl;
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