#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cmath>
#include <queue>

#include <vector>
#include <regex>

using namespace std;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<vector<int>> vvi;
typedef vector<vector<char>> vvc;
typedef pair<vector<vector<char>>, string> pvvcs;

pair<vi, vi> parseInput()
{
    string filename = "input.txt";
    ifstream infile(filename);
    vi registers;
    string discard;
    int x;
    for (int i = 0; i < 3; ++i){
        infile >> discard;
        infile >> discard;
        infile >> x;
        registers.push_back(x);
    }
    infile >> discard;
    string programString;
    infile >> programString;
    vi program;
    for (int i = 0; i < programString.size();i += 2){
        program.push_back(stoi(programString.substr(i,1)));
    }
    return make_pair(registers, program);
}
int literal(int operand){
    return operand;
}

int combo(int operand, vi registers){
    if (operand < 4) return literal(operand);
    return registers[operand - 4];
}

pair<vi,bool> adv(int operand, vi registers, int pointer){
    int divisorPower = combo(operand, registers);
    int divisor = 1 << divisorPower;
    int writeToA = registers[0] / divisor;
    vi returnArray = {writeToA, registers[1], registers[2], pointer + 2, 0};
    bool writeValue = false;
    pair<vi, bool> returnValue = make_pair(returnArray, writeValue);
    return returnValue;
}

pair<vi,bool> bxl(int operand, vi registers, int pointer){
    int xorResult = registers[1] ^ literal(operand);
    vi returnArray = {registers[0], xorResult, registers[2], pointer + 2, 0};
    bool writeValue = false;
    return make_pair(returnArray, writeValue);
}

pair<vi,bool> bst(int operand, vi registers, int pointer){
    int toB = combo(operand, registers) % 8;
    vi returnArray = {registers[0], toB, registers[2], pointer + 2, 0};
    bool writeValue = false;
    return make_pair(returnArray, writeValue);
}

pair<vi,bool> jnz(int operand, vi registers, int pointer){
    vi fiveInts{registers[0], registers[1], registers[2], pointer + 2, 0};
    if (registers[0] == 0){
        return make_pair(fiveInts, false);
    }
    fiveInts[3] = literal(operand);
    return make_pair(fiveInts, false);
}

pair<vi,bool> bxc(int operand, vi registers, int pointer){
    int toB = registers[1] ^ registers[2];
    vi returnArray = {registers[0], toB, registers[2], pointer + 2, 0};
    bool writeValue = false;
    return make_pair(returnArray, writeValue);
}

pair<vi,bool> out(int operand, vi registers, int pointer){
    int comboOperand = combo(operand, registers);
    int output = comboOperand % 8;
    vi returnArray = {registers[0], registers[1], registers[2], pointer + 2, output};
    bool writeValue = true;
    return make_pair(returnArray, writeValue);
}

pair<vi,bool> bdv(int operand, vi registers, int pointer){
    int divisorPower = combo(operand, registers);
    int divisor = 1 << divisorPower;
    int writeToB = registers[0] / divisor;
    vi returnArray = {registers[0], writeToB, registers[2], pointer + 2, 0};
    bool writeValue = false;
    return make_pair(returnArray, writeValue);
}

pair<vi,bool> cdv(int operand, vi registers, int pointer){
    int divisorPower = combo(operand, registers);
    int divisor = 1 << divisorPower;
    int writeToC = registers[0] / divisor;
    vi returnArray = {registers[0], registers[1], writeToC, pointer + 2, 0};
    bool writeValue = false;
    return make_pair(returnArray, writeValue);
}



/*
returns the new registers a,b, and c, the instruction pointer, a value to output, and a boolean saying whether to actually output it
*/
pair<vi,bool> executeInstruction(int opCode, int operand, vi registers, int instructionPointer){
    switch (opCode){
        case (0):
            return adv(operand, registers, instructionPointer);
        case (1):
            return bxl(operand, registers, instructionPointer);
        case (2):
            return bst(operand, registers, instructionPointer);
        case (3):
            return jnz(operand, registers, instructionPointer);
        case (4):
            return bxc(operand, registers, instructionPointer);
        case (5):
            return out(operand, registers, instructionPointer);
        case (6):
            return bdv(operand, registers, instructionPointer);
        case (7):
            return cdv(operand, registers, instructionPointer);
        }
    throw;
}


string getOutput(vi registers, vi const &program){
    int instructionPointer = 0;
    string output = "";
    int numIters = 0;
    while (instructionPointer < program.size()){
        ++numIters;
        int opCode = program[instructionPointer];
        int operand = program[instructionPointer+1];
        cout << "starting state - " << "instruction pointer: " << instructionPointer << ", "<< "Registers: " << registers[0] << " " << registers[1] << " " << registers[2] << " - OUTPUT: " << output << "\n";
        pair<vi, bool>result = executeInstruction(opCode, operand, registers, instructionPointer);
        registers = {result.first[0], result.first[1], result.first[2]};
        instructionPointer = result.first[3];
        int outputInt = result.first[4];
        if (result.second){
            output += to_string(outputInt) + ",";
            // cout << output << endl;
        }
        cout << "ending state - " << "instruction pointer: " << instructionPointer << ", "<< "Registers: " << registers[0] << " " << registers[1] << " " << registers[2] << " - OUTPUT: " << output << "\n";
        cout << "Program: ";
        for (int x : program){
            cout << x << " ";
        }
        cout << "\n\n";
    }
    cout << "numIters: " << numIters << '\n';
    return output;
}

int main()
{
    time_t t1 = time(NULL);
    pair<vi,vi> registersProgram = parseInput();
    vi registers{registersProgram.first[0], registersProgram.first[1], registersProgram.first[2]};

    vi program = registersProgram.second;
    string result = getOutput(registers, program);

    cout << result << endl;
    cout << endl;
    cout << time(NULL) - t1 << endl;
    return 0;
}