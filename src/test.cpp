// Copyright 2021 Yash Pandey(YP) - EmperorYP7

#include <dynamic_array>
#include <iostream>

struct TestData {
    int x, y, z;
    TestData(int i, int j, int k)
        : x(i), y(j), z(k)
    {}
};

int main()
{
    custom::dynamic_array<int> array = {1, 2, 3, 4, 5};

    std::cout << "\n[] based for loop:\n";

    for (int i = 0; i < array.size(); i++)
        std::cout << array[i] << " ";

    std::cout << "\n Iterators based for loop:\n";

    for (custom::dynamic_array<int>::iterator it = array.begin(); it != array.end(); it++)
    {
        std::cout << *it << " ";
    }

    std::cout << "\n Range based for loop:\n";

    for(auto it : array)
    {
        std::cout << it << " ";
    }

    custom::dynamic_array<TestData> temp_array;

    temp_array.emplace_back(2, 3, 5);
    temp_array.emplace_back(6, 7, 8);
    temp_array.emplace_back(9, 10, 11);
    temp_array.emplace_back(12, 13, 14);
    temp_array.emplace_back(22, 23, 25);
    temp_array.emplace_back(32, 33, 35);
    temp_array.emplace_back(42, 43, 45);
    temp_array.emplace_back(52, 53, 55);
    temp_array.emplace_back(62, 63, 65);

    std::cout << "\n[] based for loop:\n";

    for (int i = 0; i < temp_array.size(); i++)
        std::cout << temp_array[i].x << ", " << temp_array[i].y << ", " << temp_array[i].z  << "\n";

    std::cout << "--------------------\n";

    std::cout << "\n Iterators based for loop:\n";

    for (auto it = temp_array.begin(); it != temp_array.end(); it++)
        std::cout << it->x << ", " << it->y << ", " << it->z  << "\n";
    
    std::cout << "--------------------\n";

    std::cout << "\n Range based for loop:\n";

    for(auto& it : temp_array)
        std::cout << it.x << ", " << it.y << ", " << it.z  << "\n";

    std::cout << "\n";
}