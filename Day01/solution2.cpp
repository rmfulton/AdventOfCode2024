#include <iostream>
#include <fstream>

int getMultiplicity(int* list, int value, int size){
    int multiplicity = 0;
    for (int i = 0; i < size; ++i){
        if (list[i] == value){
            multiplicity++;
        }
    }
    return multiplicity;
}

int main()
{
    int left[1000];
    int right[1000];
    int size = 0;
    std::string filename = "sample.txt";
    filename = "input.txt";
    std::ifstream myfile(filename, std::ios_base::in);

    while (myfile >> left[size] >> right[size])
    {
        size += 1;
    }
    int similarityScore = 0;
    for (int i = 0; i < size; ++i){
        int m = getMultiplicity(right, left[i], size);
        similarityScore += m*left[i];
    }

    std::cout << similarityScore << std::endl;

    return 0;
}