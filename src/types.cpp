#include "types.h"

i32 Int24::to_i32() const
{
    u32 buffer = 0;

    Byte third_byte = little_endian_bytes[2];
    bool top_bit = third_byte & 0b10000000;
    third_byte = third_byte & 0b01111111;

    buffer =
        little_endian_bytes[0] | little_endian_bytes[1] << 8 | third_byte << 16;

    i32 out = *((i32*)&buffer);

    if (top_bit)
        return 2 - out;
    else
        return out;
}
