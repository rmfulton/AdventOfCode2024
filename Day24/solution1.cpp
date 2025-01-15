#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

pair<map<string,bool>, vector<vector<string>>> parseInput(string filename)
{
    ifstream f(filename);
    map<string,int> ordering;
    vector<pair<string,string>> edges;
    string x;
    vector<vector<string>> updates;
    map<string, bool> initialValues;
    while (f >> x)
    {
        if (x.find(':') == x.npos){
            string operation;
            string arg2;
            string arrow;
            string result;
            f >> operation >> arg2 >> arrow >> result;
            updates.push_back({operation,x, arg2, result});
        } else {
            int value;
            f >> value;
            initialValues[x.substr(0,x.size() - 1)] = (bool)value;
        }
    }
    return make_pair(initialValues, updates);

}


bool computeBinaryOp(string operation, bool operand1, bool operand2){
    if (operation == "AND"){
        return operand1 && operand2;
    } else if (operation == "OR"){
        return operand1 || operand2;
    } else {
        return operand1 ^ operand2;
    }
}


long solution(map<string,bool> values, vector<vector<string>> updates){
    map<string, vector<string>> dependencies;
    vector<string> nodeStack;
    for (vector<string> operation : updates){
        string op = operation[0];
        string arg1 = operation[1];
        string arg2 = operation[2];
        string assign = operation[3];
        dependencies[assign] = {arg1, arg2, op};
        nodeStack.push_back(assign);
    }
    while (nodeStack.size() > 0){
        string top = nodeStack.back();
        // cout << nodeStack.size()  << " " << top << endl;
        if (values.find(top) != values.end()){
            nodeStack.pop_back();
            continue;
        }
        string dep1 = dependencies[top][0];
        string dep2 = dependencies[top][1];
        bool areBothReady = true;
        for (string d : {dep1, dep2}){
            if (values.find(d) == values.end()){
                nodeStack.push_back(d);
                areBothReady = false;
            }
        }
        if (areBothReady){
            values[top] = computeBinaryOp(dependencies[top][2], values[dep1], values[dep2]);
            nodeStack.pop_back();
        }

    }

    long long result = 0;
    for (pair<const string,bool> registers : values){
        string name = registers.first;
        // cout << name << " " << registers.second << endl;
        if (name[0] == 'z' && registers.second){
            cout << name << " ";
            int shift = stoi(name.substr(1,name.size() - 1));
            result += ((long)1) << shift;
        }

    }
    cout << endl;
    return result;
}

int main()
{
    time_t t1 = time(NULL);
    string filename = "input.txt";
    // filename = "sample.txt";
    pair<map<string,bool>, vector<vector<string>>> data = parseInput(filename);
    map<string,bool> initialValues = data.first;
    vector<vector<string>> updates = data.second;
    long output = solution(initialValues, updates);
    cout << output << endl;
    cout << "took " << time(NULL) - t1 << " seconds" << endl;
    return 0;
}