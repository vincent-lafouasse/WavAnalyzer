#include "read.h"

u32 read_u32(const std::vector<Byte>& bytes, size_t& index)
{
    assert(index + 4 < bytes.size());

    u32 out = 0;
    for (size_t i = 0; i < 4; i++, index++)
    {
        out += static_cast<u32>(bytes[index]) << (8 * i);
    }
    return out;
}

u16 read_u16(const std::vector<Byte>& bytes, size_t& index)
{
    assert(index + 2 < bytes.size());

    u16 out = 0;
    for (size_t i = 0; i < 2; i++, index++)
    {
        out += static_cast<u16>(bytes[index]) << (8 * i);
    }
    return out;
}

u8 read_u8(const std::vector<Byte>& bytes, size_t& index)
{
    assert(index < bytes.size());

    return bytes[index++];
}

FourCC read_four_cc(const std::vector<Byte>& bytes, size_t& index)
{
    assert(index + 4 < bytes.size());

    FourCC out;
    for (size_t i = 0; i < 4; i++, index++)
    {
        out[i] = bytes[index];
    }
    return out;
}

Int24 read_i24(const std::vector<Byte>& bytes, size_t& index)
{
    Int24 out;
    out.bytes[0] = bytes[index++];
    out.bytes[1] = bytes[index++];
    out.bytes[2] = bytes[index++];
    return out;
}
