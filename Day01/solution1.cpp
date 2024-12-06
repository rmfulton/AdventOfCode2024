#include <iostream>
#include <fstream>

void insertionSort(int* list, int size){
    for (int i = 0; i < size; ++i){
        for (int j = i+1; j < size; ++j){
            if (list[i] > list[j]){
                int tmp = list[i];
                list[i] = list[j];
                list[j] = tmp;
            }
        }
    }
}

int main()
{
    int left[1000];
    int right[1000];
    int size = 0;
    std::string filename = "input.txt";
    std::ifstream myfile(filename, std::ios_base::in);

    while (myfile >> left[size] >> right[size])
    {
        size += 1;
    }
    insertionSort(left, size);
    insertionSort(right,size);

    int sumDiff = 0;
    for (int i = 0; i < size; ++i){
        int delta = left[i] - right[i];
        sumDiff += delta < 0 ? -delta : delta;
    }

    std::cout << sumDiff << std::endl;

    return 0;
}