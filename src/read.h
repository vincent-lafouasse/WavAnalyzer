#pragma once

#include <vector>
#include "FourCC.h"
#include "types.h"

u32 read_u32(const std::vector<Byte>& bytes, size_t& index);
u16 read_u16(const std::vector<Byte>& bytes, size_t& index);
u8 read_u8(const std::vector<Byte>& bytes, size_t& index);
FourCC read_four_cc(const std::vector<Byte>& bytes, size_t& index);
