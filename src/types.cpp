#include "types.h"

i32 Int24::to_i32() const
{
    u32 out = 0;

    out = little_endian_bytes[0] | little_endian_bytes[1] << 8 |
          little_endian_bytes[2] << 16;

    return *((i32*)&out);
}
