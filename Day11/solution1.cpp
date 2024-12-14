#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cmath>

#include <vector>
#include <regex>

using namespace std;

vector<long> parseInput()
{
    string filename = "input.txt";
    ifstream infile(filename);
    vector<long> result;
    long x;
    while (infile >> x){
        result.push_back(x);
    }
    return result;
}

long numDigits(long stone){
    int k = 0;
    while (stone > 0){
        stone = stone/10;
        ++k;
    } 
    return k;
}

vector<long> splitStone(long stone){
    int d = numDigits(stone);
    long divisor = pow(10,d/2);
    long back = stone % divisor;
    long front = stone / divisor;
    vector<long> v{front,back};
    return v;
}

vector<long> blink(vector<long> stones){
    vector<long> newStones;
    for(long stone: stones){
        // cout << "stone: " << stone << " ";
        if (stone == 0){
            // cout << "newStone: " << 1 << '\n';
            newStones.push_back(1);
        }
        else if (numDigits(stone) % 2 == 0){
            vector<long> split = splitStone(stone);
            // cout << "newStone: " << split[0] << ' ' << split[1] << '\n';
            newStones.push_back(split[0]);
            newStones.push_back(split[1]);
        }
        else {
            // cout << "newStone: " << stone*2024 << '\n';
            newStones.push_back(stone*2024);
        }
    }
    return newStones;
}

long numStonesAfterNBlinks(vector<long> stones, int blinks){
    vector<long> newStones = stones;
    for(int k = 0; k < blinks; ++k){
        // cout << "iter " << k << '\n';
        newStones = blink(newStones);
    }
    // for (int stone: newStones){
    //     cout << stone << " ";
    // }
    // cout << '\n';
    return newStones.size();
}

int main()
{
    time_t t1 = time(NULL);
    vector<long> stones = parseInput();
    int numBlinks = 25;
    long answer = numStonesAfterNBlinks(stones, numBlinks);
    cout << answer << endl;
    cout << "took " << time(NULL) - t1 << " seconds" << endl;
    return 0;
}