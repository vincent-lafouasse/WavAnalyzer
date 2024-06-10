#pragma once

#include <iostream>
#include <fstream>

template <typename T>
void log(T object, const char* name)
{
    std::cout << name << " : " << object << '\n';
    std::cout.flush();
}

template <typename T>
void write_to_csv(const std::vector<T>& data, const std::string& filename)
{
    std::ofstream csv;
    csv.open(filename);
    for (const T& cell : data)
    {
        csv << cell << ",";
    }
    csv << std::endl;
    csv.close();
}
