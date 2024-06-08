#include "read.h"

u32 read_u32(const std::vector<Byte>& bytes, size_t& index, IndexPolicy policy)
{
    assert(index + 4 < bytes.size());

    u32 out = 0;
    for (size_t i = 0; i < 4; i++)
    {
        out += static_cast<u32>(bytes[index + i]) << (8 * i);
    }
    if (policy == IndexPolicy::Advance)
        index += sizeof(out);
    return out;
}

u16 read_u16(const std::vector<Byte>& bytes, size_t& index, IndexPolicy policy)
{
    assert(index + 2 < bytes.size());

    u16 out = 0;
    for (size_t i = 0; i < 2; i++)
    {
        out += static_cast<u16>(bytes[index + i]) << (8 * i);
    }
    if (policy == IndexPolicy::Advance)
        index += sizeof(out);
    return out;
}

u8 read_u8(const std::vector<Byte>& bytes, size_t& index, IndexPolicy policy)
{
    assert(index < bytes.size());

    u8 out = bytes[index];

    if (policy == IndexPolicy::Advance)
        index += sizeof(out);
    return out;
}

FourCC read_four_cc(const std::vector<Byte>& bytes,
                    size_t& index,
                    IndexPolicy policy)
{
    assert(index + 4 < bytes.size());

    FourCC out;
    for (size_t i = 0; i < 4; i++)
    {
        out[i] = bytes[index + i];
    }
    if (policy == IndexPolicy::Advance)
        index += 4;
    return out;
}

Int24 read_i24(const std::vector<Byte>& bytes,
               size_t& index,
               IndexPolicy policy)
{
    Int24 out;
    out.bytes[0] = bytes[index];
    out.bytes[1] = bytes[index + 1];
    out.bytes[2] = bytes[index + 2];
    if (policy == IndexPolicy::Advance)
        index += 3;
    return out;
}
