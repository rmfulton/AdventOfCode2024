#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

vector<string> parseInput(string filename)
{
    ifstream f(filename);
    vector<string> grid;
    string instring;
    while (f >> instring)
    {
        grid.push_back(instring);
    }
    return grid;
}

map<char, vector<int>> getNumericPadCoords()
{
    map<char, vector<int>> numericPad;
    numericPad['7'] = {0, 0};
    numericPad['8'] = {0, 1};
    numericPad['9'] = {0, 2};
    numericPad['4'] = {1, 0};
    numericPad['5'] = {1, 1};
    numericPad['6'] = {1, 2};
    numericPad['1'] = {2, 0};
    numericPad['2'] = {2, 1};
    numericPad['3'] = {2, 2};
    numericPad['X'] = {3, 0};
    numericPad['0'] = {3, 1};
    numericPad['A'] = {3, 2};
    return numericPad;
}

map<char, vector<int>> getDirectionPadCoords()
{
    map<char, vector<int>> directionPad;
    directionPad['X'] = {0, 0};
    directionPad['^'] = {0, 1};
    directionPad['A'] = {0, 2};
    directionPad['<'] = {1, 0};
    directionPad['v'] = {1, 1};
    directionPad['>'] = {1, 2};
    return directionPad;
}

string getArrowInstructions(string s);

int numOnesIn(int x)
{
    int count = 0;
    for (int y = x; y > 0; y >>= 1)
        if (y % 2 == 1)
            ++count;
    return count;
}

/*
Input: start and end, where each char corresponds to one of 
+---+---+---+
| 7 | 8 | 9 |
+---+---+---+
| 4 | 5 | 6 |
+---+---+---+
| 1 | 2 | 3 |
+---+---+---+
  X | 0 | A |
    +---+---+
and X is an illegal square

Output: a set of sequences drawn from '><v^' encoding the shortest paths from start to end

Algorithm: 
    option 1: 
        create a 'DistanceFromEnd' map and a pathStack. 
        Move to neighbors which decrease the distanceFromEnd until you reach end,
        then translate the path to a sequence of instructions and save the instructions.

        complexity: 
            O(paths ~ N^2, where N is the dimension of a square board)

    option 2: 
        compute the taxicab distance D
        map to each binary number 0 <= i < 2^D
            a walk of length D in the same quadrant as end
            if the walk terminates at end,
                and the walk doesn't pass through any illegal zones,
                    save the instructions for the walk
        complexity:
            O(N*2^N)
    
    I prefer option 2. Conceptually elegant > computationally efficient
*/
vector<string> GetAllPathsFromTo(char start, char end, map<char, vector<int>> mapCharToCoords)
{
    vector<int> startCoord = mapCharToCoords[start];
    vector<int> endCoord = mapCharToCoords[end];
    vector<int> illegalCoord = mapCharToCoords['X'];
    const int deltaX = endCoord[0] - startCoord[0];
    const int deltaY = endCoord[1] - startCoord[1];
    const int taxicab = abs(deltaX) + abs(deltaY);
    const int dx = deltaX == 0 ? 0 : deltaX / abs(deltaX);
    const int dy = deltaY == 0 ? 0 : deltaY / abs(deltaY);
    const char dxChar = (vector<char>){'^', 'v'}[(dx + 1) / 2];
    const char dyChar = (vector<char>){'<', '>'}[(dy + 1) / 2];
    vector<int> instructionSet {dxChar, dyChar};
    vector<vector<int>> deltaPos {{dx, 0}, {0, dy}};
    vector<string> allInstructions;
    for (int i = 0; i < (1 << taxicab); ++i)
    {
        if (numOnesIn(i) != abs(deltaY))
            continue;
        bool isIllegalPath = false;
        vector<int> pathTip = startCoord;
        string instructions = "";
        for (int j = 0; j < taxicab; ++j)
        {
            int dir = (i >> j) % 2;
            instructions += instructionSet[dir];
            pathTip[0] += deltaPos[dir][0];
            pathTip[1] += deltaPos[dir][1];
            if (pathTip == illegalCoord)
            {
                isIllegalPath = true;
                break;
            }
        }
        if (isIllegalPath)
            continue;
        allInstructions.push_back(instructions);
    }
    return allInstructions;
}
/*
Input: string s,
Output: all minimum length instructions that output s
Algorithm: 
    Feed k to a kind of reduce function, where the initial value is 'A',
    and the reduce operation *(c1, c2) produces all VALID minimum-length paths from c1 to c2.
    for each prefix in the set of prefix instructions,
        for each element in the result of *(c1, c2),
            you create a new set of instructions prefix + element + 'A'
    then return this final set of instructions. 
*/
vector<string> getAllValidMinimumLengthInstructionsFor(string s, map<char, vector<int>> mapCharToCoords )
{
    vector<string> prefixes = {""};
    char start = 'A';
    for (char end : s)
    {
        vector<string> suffixes = GetAllPathsFromTo(start, end, mapCharToCoords);
        vector<string> instructions;
        for (string prefix : prefixes)
        {
            for (string suffix : suffixes)
            {
                instructions.push_back(prefix + suffix + 'A');
            }
        }

        prefixes = instructions;
        start = end;
    }
    return prefixes;
}

/*
Input: A set of strings s,
Output: The union over elements k of s of (all minimum length instructions that output k)
*/
vector<string> getInstructions(vector<string> strings, map<char, vector<int>> mapCharToCoords)
{
    vector<string> instructionsForAny;
    for (string s : strings)
    {
        for (string instruction : getAllValidMinimumLengthInstructionsFor(s, mapCharToCoords))
        {
            instructionsForAny.push_back(instruction);
        }
    }
    return instructionsForAny;
}
vector<string> getNumericInstructions(vector<string> strings){
    return getInstructions(strings, getNumericPadCoords());
}

vector<string> getDirectionInstructions(vector<string> strings)
{
    return getInstructions(strings, getDirectionPadCoords());
}

int getComplexity(string my_string)
{
    int numeric = stoi(my_string.substr(0, 3));
    vector<string> instructions = getNumericInstructions({my_string});
    instructions = getDirectionInstructions(instructions);
    instructions = getDirectionInstructions(instructions);
    int smallestLength = 10000;
    for (string x : instructions){
        if (x.size() < smallestLength)
            smallestLength = x.size();
    }
    cout << numeric << " " << smallestLength << endl;
    return numeric*smallestLength;
}

int main()
{
    time_t t1 = time(NULL);
    string filename = "input.txt";
    // filename = "sample.txt";
    vector<string> grid = parseInput(filename);
    int complexity = 0;
    for (string s : grid)
    {
        complexity += getComplexity(s);
    }
    cout << complexity << endl;
    cout << "took " << time(NULL) - t1 << " seconds" << endl;
    return 0;
}