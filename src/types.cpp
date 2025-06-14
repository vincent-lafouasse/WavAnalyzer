#include "types.h"

i64 Int24::to_i64() const {
    u32 buffer = 0;

    Byte third_byte = little_endian_bytes[2];
    bool top_bit = third_byte & 0b10000000;
    third_byte = third_byte & 0b01111111;

    buffer =
        little_endian_bytes[0] | little_endian_bytes[1] << 8 | third_byte << 16;

    i64 out = buffer;

    if (top_bit)
        out -= (1 << 23);

    return out;
}
