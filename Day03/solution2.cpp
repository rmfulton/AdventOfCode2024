#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <vector>
#include <regex>

std::vector<std::string> parseInput()
{
    std::string filename = "input.txt";
    std::ifstream infile(filename);

    std::string target;
    std::regex pattern("mul\\(\\d{1,3},\\d{1,3}\\)|do\\(\\)|don't\\(\\)");
    std::vector<std::string> result;
    while (infile >> target)
    {
        for (std::smatch sm; std::regex_search(target, sm, pattern);)
        {
            result.push_back(sm.str());
            std::cout << sm.str() << std::endl;
            target = sm.suffix();
        }
        std::cout << std::endl;
    }
    return result;
}

std::vector<std::string> cleanedInput(std::vector<std::string> debrided)
{
    std::string prefix = "mul(";
    std::vector<std::string> cleaned;
    bool discard = false;
    for (std::string e : debrided)
    {
        if (e == "do()"){
            discard = false;
            continue;
        }
        if (e == "don't()"){
            discard = true;
            continue;
        }
        if (discard){
            continue;
        }
        int start = prefix.size();
        int end = e.find(",");
        int length = end - start;
        std::string first = e.substr(start, length);
        start = e.find(",") + 1;
        end = e.size() - 1;
        length = end - start;
        std::string second = e.substr(start, length);

        std::string tmp = first + " " + second;
        std::cout << tmp << std::endl;

        cleaned.push_back(tmp);
    }
    std::cout << std::endl;
    return cleaned;
}

int main()
{
    std::vector<std::string> expressions = parseInput();
    std::vector<std::string> cleaned = cleanedInput(expressions);
    int result = 0;
    for (std::string pair : cleaned)
    {
        std::istringstream iss(pair);
        int a, b;
        iss >> a >> b;
        std::cout << a << " " << b << std::endl;
        result += a * b;
    }
    std::cout << result << std::endl;
    return 0;
}