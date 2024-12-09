#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include <vector>
#include <regex>

using namespace std;
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

void rearrangeFileSystemContents(vector<int> *fileSystem)
{
    int N = (*fileSystem).size();
    int i = 0;
    int j = N - 1;
    while (i < j)
    {
        if ((*fileSystem)[i] != -1)
        {
            ++i;
            continue;
        }
        if ((*fileSystem)[j] == -1)
        {
            --j;
            continue;
        }
        else
        {
            (*fileSystem)[i] = (*fileSystem)[j];
            (*fileSystem)[j] = -1;
            ++i;
            --j;
        }
    }
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
    rearrangeFileSystemContents(&fileSystem);
    return computeCheckSum(fileSystem);
}

int main()
{
    time_t t1 = time(NULL);
    vector<int> fileSystem = parseInput();
    long answer = getAnswer(fileSystem);
    cout << answer << endl;
    cout << "took " << time(NULL) - t1 << " seconds" << endl;
    return 0;
}