#include "types.h"

i32 Int24::to_i32() const
{
    u32 out = 0;

    Byte third_byte = little_endian_bytes[2];
    bool top_bit = third_byte & 0b10000000;
    third_byte = third_byte & 0b01111111;

    out =
        little_endian_bytes[0] | little_endian_bytes[1] << 8 | third_byte << 16;

    if (top_bit)
        out = out | (1 << 31);

    return *((i32*)&out);
}
