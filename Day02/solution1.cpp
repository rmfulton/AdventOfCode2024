#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

bool isIncreasing(int left, int right, bool increasing){
    return left < right == increasing;
}
bool notTheSame(int left, int right){
    return left != right;
}
bool withinFour(int left, int right){
    if (left < right){
        return left + 4 > right;
    } else {
        return right + 4 > left;
    }
}

bool isValidSequence(std::vector<int> nums){
    int L = nums.size();
    if ( L < 2){
        return true;
    }
    bool increasing = nums[0] < nums[1];
    for (int i = 0; i < L - 1; ++i){
        int left = nums[i];
        int right = nums[i+1];
        bool valid = isIncreasing(left, right, increasing) && notTheSame(left, right) && withinFour(left, right);
        if (!valid){
            return false;
        }
    }
    return true;

}

int main()
{

    std::string filename = "input.txt";
    std::ifstream infile(filename);

    std::string line;
    int goodCount = 0;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        std::vector<int> nums;
        int x;
        while (iss >> x){
            nums.push_back(x);
        }
        if (isValidSequence(nums)){
            goodCount++;
        }
    }

    std::cout << goodCount << std::endl;

    return 0;
}