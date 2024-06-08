#include "FourCC.h"

#include <iostream>

bool fourcc_eq(FourCC four_cc, const char* pattern)
{
    for (size_t i = 0; i < 4 && pattern[i]; i++)
    {
        if (four_cc[i] != pattern[i])
            return false;
    }
    return true;
}

[[maybe_unused]] void log_fourcc(FourCC four_cc)
{
    for (int i = 0; i < 4; i++)
        std::cout << four_cc[i];
}
