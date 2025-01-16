#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>

using namespace std;
/*
- x_i XOR y_i
- is the ith Basic Addition Bit (BAB_i), i = 00..44

- x_i AND y_i
- is the ith Basic Carry Bit (BCB_i) 00..44

- BCB_i AND BAB_{i+1} is the (i+1)th Intermediate Carry Bit (ICB_{i+1}) i = 00..44

- ICB_i OR BCB_i is the Final Carry Bit (FCB_i) 01..45


- BAB_{i} XOR FCB_{i-1} is the Output Bit (OUT_i) 02..45

expectations: 
- BCB_i OR 
- each FCB is XOR'd with the next BAB to 


- check that the output of x_i AND y_i
- is fed into z_{i+1}

*/

enum KIND{
    BAB,
    BCB,
    ICB,
    FCB,
    OUT
};

class Expression {
    string operand1;
    string operand2;
    string binaryOperator;
    string output;

    public:Expression(string op1, string op2, string binOp, string out){
        operand1 = op1;
        operand2 = op2;
        binaryOperator = binOp;
        output = out;
    }
    public:Expression(){

    }

    public:Expression(const Expression &other){
        operand1 = other.operand1;
        operand2 = other.operand2;
        binaryOperator = other.binaryOperator;
        output = other.output;
    }

    public:KIND getKind(){
        if (isBasicAdditionBit()){
            return BAB;
        }
        if (isBasicCarryBit()){
            return BCB;
        }
        // if it's not either of these, we can deduce type by the operator
        int ANDORXOR = (binaryOperator == "AND") ? 0 : (binaryOperator == "OR") ? 1 : 2;
        switch (ANDORXOR){
            case 0:
                return ICB; // AND => Intermediate Carry Bit
            case 1:
                return FCB; // OR => Final Carry Bit
            case 2:
                return OUT; // XOR => Output Bit
            default:
                throw;
        }
    }

    bool isBasicAdditionBit(){
        if (binaryOperator != "XOR"){
            return false;
        }
        if (operand1[0] == 'x' || operand2[0] == 'x'){
            return true;
        }
        return false;
    }
    bool isBasicCarryBit(){
        if (binaryOperator != "AND"){
            return false;
        }
        if (operand1[0] == 'x' || operand2[0] == 'x'){
            return true;
        }
        return false;
    }
    public:string getOutput(){
        return output;
    }

    public:string toString(){
        return operand1 + " " + binaryOperator + " " + operand2 + " -> " + output;
    }

    public:vector<string> getInputs(){
        return {operand1, operand2};
    }
};

class ExpressionGroup{
    vector<Expression> group;
    map<string,Expression> lookup;
    map<string,Expression> babSet;
    map<string,Expression> bcbSet;
    map<string,vector<string>> contributesTo;

    public:ExpressionGroup(){

    }

    public:ExpressionGroup(const ExpressionGroup &other){
        group = other.group;
        lookup = other.lookup;
        babSet = other.babSet;
        bcbSet = other.bcbSet;
        contributesTo = other.contributesTo;
    }

    public:ExpressionGroup(vector<Expression> g){
        group = g;
        populateLookup();
        createSets();
        createReverseMapping();
    }

    public:ExpressionGroup(map<string,Expression> m){
        lookup = m;
        for (pair<const string,Expression> p: m){
            group.push_back(p.second);
        }
        createSets();
        createReverseMapping();
    }

    private:void createSets(){
        for (pair <const string, Expression> p : lookup){
            KIND k = p.second.getKind();
            if (k == BAB){
                babSet[p.first] = p.second;
            } else if (k == BCB){
                bcbSet[p.first] = p.second;
            }
        }
    }

    private:void createReverseMapping(){
        for (Expression e : group){
            vector<string> inputs = e.getInputs();
            for(string from : inputs){
                if (contributesTo.find(from) == contributesTo.end()){
                    contributesTo[from] = {};
                }
                contributesTo[from].push_back(e.getOutput());
            }
        }
    }

    public:void printBABViolations(){
        for (pair<const string,Expression> p : babSet){
            bool IsNotInvolvedInOutput = true;
            bool IsNotInvolvedInICB = true;
            bool IsNotInvolvedInFCB = true;
            for (string s : contributesTo[p.first]){
                KIND k = lookup[s].getKind();
                if (k == OUT){
                    IsNotInvolvedInOutput = false;
                } else if (k == ICB){
                    IsNotInvolvedInICB = false;
                } 
            }

            if ((IsNotInvolvedInOutput || IsNotInvolvedInICB) && p.first != "z00"){
                cout << p.second.toString() << endl;
            }
        }
    }

    public:void printBCBViolations(){
        for (pair<const string,Expression> p : bcbSet){
            bool IsNotInvolvedInICB = true;
            bool IsNotInvolvedInFCB = true;
            for (string s : contributesTo[p.first]){
                KIND k = lookup[s].getKind();
                if (k == FCB){
                    IsNotInvolvedInFCB = false;
                } else if (k == ICB){
                    IsNotInvolvedInICB = false;
                } 
            }

            if ((IsNotInvolvedInFCB || IsNotInvolvedInICB) && p.first != "z00"){
                cout << p.second.toString() << endl;
            }
        }
    }

    public:void printOUTViolations(){
        for (pair<const string, Expression> p: lookup){
            Expression e = p.second;
            KIND k = e.getKind();
            if (k != OUT){
                continue;
            }
            string wireOut = e.getOutput();
            if (wireOut[0] != 'z'){
                cout << e.toString() << endl;
            }
        }
    }

    private:void populateLookup(){
        for (Expression e : group){
            lookup[e.getOutput()] = e;
        }
    }
    public:void push_back(Expression e){
        group.push_back(e);
        lookup[e.getOutput()] = e;
    }


};


map<string,Expression> parseInput(string filename)
{
    ifstream f(filename);
    string x;
    map<string,Expression> gates;
    map<string, bool> initialValues;
    while (f >> x)
    {
        if (x.find(':') == x.npos){
            string operation;
            string arg2;
            string arrow;
            string result;
            f >> operation >> arg2 >> arrow >> result;
            Expression e = Expression(x, arg2, operation, result);
            gates[result] = e;
        } else {
            int value;
            f >> value;
            initialValues[x.substr(0,x.size() - 1)] = (bool)value;
        }
    }
    return gates;

}

void solution(map<string, Expression> gates){
    ExpressionGroup G = ExpressionGroup(gates);
    G.printBABViolations();
    G.printOUTViolations();
}

/*
deductions: 
qjj is a violation, because it does not input to an OUT expression. suspect qjj <=> cbj
cfk is a violation because it is the output from an OUT string suspect cfk <=> z35
dmn is a violation because it is the output from an OUT string suspect dmn <=> z18
gmt is a violation because it is the output from an OUT string suspect gmt <=> z07

LLMs might actually have a hard time with this one 
*/
int main()
{
    time_t t1 = time(NULL);
    string filename = "input.txt";
    // filename = "sample.txt";
    map<string,Expression> gates = parseInput(filename);
    solution(gates);
    // cout << output << endl;
    cout << "took " << time(NULL) - t1 << " seconds" << endl;
    return 0;
}