#pragma once

#include <vector>
#include "FourCC.h"
#include "types.h"

/*
 * the WAV format serializes data as little endian
 */

enum class IndexPolicy
{
    Advance,
    Peek,
};

u32 read_u32(const std::vector<Byte>& bytes, size_t& index, IndexPolicy policy);
u16 read_u16(const std::vector<Byte>& bytes, size_t& index, IndexPolicy policy);
u8 read_u8(const std::vector<Byte>& bytes, size_t& index, IndexPolicy policy);
FourCC read_four_cc(const std::vector<Byte>& bytes,
                    size_t& index,
                    IndexPolicy policy);

Int24 read_i24(const std::vector<Byte>& bytes,
               size_t& index,
               IndexPolicy policy);

i32 read_sample(u8 bit_depth,
                const std::vector<Byte>& bytes,
                size_t& index,
                IndexPolicy policy);
