#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

vector<int> parseInput(string filename)
{
    ifstream f(filename);
    vector<int> nums;
    int x;
    while (f >> x)
    {
        nums.push_back(x);
    }
    return nums;
}

long mix(long x, long y){
    return x ^ y;
}

long prune(long x){
    return x % 16777216;
}

map<vector<int>, int> firstPriceForPattern(int seed){
    map<vector<int>, int> patternToPrice;
    vector<int> prices;
    vector<int> delta;
    prices.push_back(seed % 10);
    long secret = seed;
    for (int i = 0; i < 2000; ++i){
        secret = prune(mix(secret, secret << 6));
        secret = prune(mix(secret, secret >> 5));
        secret = prune(mix(secret, secret << 11));
        prices.push_back(secret % 10);
        int n = prices.size();
        delta.push_back(prices[n-1] - prices[n-2]);
        if (n > 4){
            vector<int> pattern{delta[n-5], delta[n-4], delta[n-3], delta[n-2]};
            if (patternToPrice.find(pattern) == patternToPrice.end()){
                patternToPrice[pattern] = prices[n-1];
            }
        }
    }
    return patternToPrice;
}

pair<vector<int>, long> getBestPattern(vector<int> seeds){
    const int N = 19;
    long netPriceOfPattern[N][N][N][N];
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            for (int k = 0; k < N; ++k)
                for (int l = 0; l < N; ++l)
                    netPriceOfPattern[i][j][k][l] = 0;
    
    for (int i = 0; i < seeds.size(); ++i){
        cout << i << " of " << seeds.size() << endl;
        int seed = seeds[i];
        map<vector<int>, int> patternToPrice = firstPriceForPattern(seed);
        // cout << "got patterns " << patternToPrice.size() << endl;
        int j = 0;
        for (pair<const vector<int>,int> p : patternToPrice){
            const vector<int> pattern = p.first;
            // cout << j << endl;
            // ++j;
            int price = p.second;

            netPriceOfPattern[pattern[0] + 9][pattern[1]+ 9][pattern[2] + 9][pattern[3]+9] += price;
        }
    }

    vector<int> bestVector;
    long maxCost = -1;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            for (int k = 0; k < N; ++k)
                for (int l = 0; l < N; ++l)
                    if (maxCost < netPriceOfPattern[i][j][k][l]){
                        maxCost = netPriceOfPattern[i][j][k][l];
                        bestVector = {i,j,k,l};
                    }
    return make_pair(bestVector, maxCost);


}

int main()
{
    time_t t1 = time(NULL);
    string filename = "input.txt";
    // filename = "sample.txt";
    vector<int> nums = parseInput(filename);
    long output = 0;
    pair<vector<int>, long> bestPattern = getBestPattern(nums);
    for (int x : bestPattern.first){
        cout << x << " ";
    }
    cout << ": " << bestPattern.second << endl;
    cout << "took " << time(NULL) - t1 << " seconds" << endl;
    return 0;
}