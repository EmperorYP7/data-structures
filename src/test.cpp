// Copyright 2021 Yash Pandey(YP) - EmperorYP7

#include <dynamic_array>
#include <iostream>

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

    std::cout << "\n";
}