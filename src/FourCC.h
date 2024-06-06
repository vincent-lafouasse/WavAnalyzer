#pragma once

#include <array>
#include "types.h"

typedef std::array<Byte, 4> FourCC;

bool fourcc_eq(FourCC four_cc, const char* pattern);
[[maybe_unused]] void log_fourcc(FourCC four_cc);
