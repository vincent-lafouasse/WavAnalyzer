#include "types.h"

Int24 Int24::read_from_bytes(const std::vector<Byte> &bytes, size_t index)
{
    Int24 out;
    out.bytes[0] = bytes[index];
    out.bytes[1] = bytes[index + 1];
    out.bytes[2] = bytes[index + 2];

    return out;
}
