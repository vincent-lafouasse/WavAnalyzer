#pragma once

#include <iostream>
#include <fstream>

template <typename T>
[[maybe_unused]]
void log(T object, const char* name)
{
    std::cout << name << " : " << object << '\n';
    std::cout.flush();
}

template <typename T>
[[maybe_unused]]
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
