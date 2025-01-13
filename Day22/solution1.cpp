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

long get2000thSecret(int seed){
    long secret = seed;
    for (int i = 0; i < 2000; ++i){
        secret = prune(mix(secret, secret << 6));
        secret = prune(mix(secret, secret >> 5));
        secret = prune(mix(secret, secret << 11));
    }
    return secret;
}

int main()
{
    time_t t1 = time(NULL);
    string filename = "input.txt";
    // filename = "sample.txt";
    vector<int> nums = parseInput(filename);
    long output = 0;
    for (int x : nums)
    {
        output += get2000thSecret(x);
    }
    cout << output << endl;
    cout << "took " << time(NULL) - t1 << " seconds" << endl;
    return 0;
}