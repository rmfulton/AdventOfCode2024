#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include <vector>
#include <regex>

using namespace std;

void printState(vector<int> fileSystem)
{
    for (int i = 0; i < fileSystem.size(); ++i)
    {
        int k = fileSystem[i];
        if (k == -1)
        {
            cout << '.';
        }
        else
        {
            cout << k;
        }
    }
    cout << '\n';
}


vector<int> parseInput()
{
    string filename = "input.txt";
    ifstream infile(filename);

    string line;
    infile >> line;
    vector<int> fileSystem;
    bool isFile = false;
    int id = -1;
    string digits = "0123456789";
    for (char c : line)
    {
        isFile = !(isFile);

        int value = digits.find(c);
        int blankSpace = -1;
        int toAdd = blankSpace;
        if (isFile)
        {
            ++id;
            toAdd = id;
        }
        for (int i = 0; i < value; ++i)
        {
            fileSystem.push_back(toAdd);
        }
    }
    return fileSystem;
}

vector<int> rearrangeFileSystemContents(vector<int> fileSystem)
{
    int N = fileSystem.size();
    int j = N - 1;
    const int EMPTY = -1;

    while (j > 0){
        if (fileSystem[j] == EMPTY){
            --j;
            continue;
        }
        int back = j;
        for (;j > -1 && fileSystem[j] == fileSystem[back];--j){}
        int fileSize = back - j;
        int i = 0;
        while (i < j){
            if (fileSystem[i] != EMPTY){
                ++i;
                continue;
            }
            int front = i;
            for(;fileSystem[i] == fileSystem[front]; ++i){}
            int gapSize = i - front;
            if (fileSize > gapSize){
                continue;
            }
            for(int k = 0; k < fileSize; ++k){
                fileSystem[front + k] = fileSystem[back - k];
                fileSystem[back - k] = EMPTY;
            }
            break;
        }
    }
    return fileSystem;
}

long computeCheckSum(vector<int> fileSystem){
    cout << "computing check sum " << '\n';
    long long result = 0;
    int N = fileSystem.size();
    for (long k = 0; k < N; ++k)
    {
        if (fileSystem[k] == -1){
            continue;
        }
        result += k * fileSystem[k];

    }
    return result;
}
long getAnswer(vector<int> fileSystem)
{
    vector<int> permuted = rearrangeFileSystemContents(fileSystem);
    // printState(permuted);
    return computeCheckSum(permuted);
}

int main()
{
    time_t t1 = time(NULL);
    vector<int> fileSystem = parseInput();
    // printState(fileSystem);
    long answer = getAnswer(fileSystem);
    cout << answer << endl;
    cout << "took " << time(NULL) - t1 << " seconds" << endl;
    return 0;
}