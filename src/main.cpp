#include <array>
#include <fstream>
#include <iostream>
#include <vector>

#include "cool_ints.h"

const char* wav_path = "./wav/brk_upfront amen_1 bar_158 bpm.wav";

typedef unsigned char Byte;
typedef std::array<Byte, 4> FourCC;

static bool fourcc_eq(FourCC four_cc, const char* pattern)
{
    for (size_t i = 0; i < 4 && pattern[i]; i++)
    {
        if (four_cc[i] != pattern[i])
            return false;
    }
    return true;
}

static u32 read_u32(const std::vector<Byte>& bytes, size_t& index)
{
    assert(index + 4 < bytes.size());

    u32 out = 0;
    for (size_t i = 0; i < 4; i++, index++)
    {
        out += static_cast<u32>(bytes[index]) << (8 * i);
    }
    return out;
}

static u8 read_u8(const std::vector<Byte>& bytes, size_t& index)
{
    assert(index < bytes.size());

    return bytes[index++];
}

static FourCC read_four_cc(const std::vector<Byte>& bytes, size_t& index)
{
    assert(index + 4 < bytes.size());

    FourCC out;
    for (size_t i = 0; i < 4; i++, index++)
    {
        out[i] = bytes[index];
    }
    return out;
}

[[maybe_unused]] static void log_fourcc(FourCC four_cc)
{
    for (int i = 0; i < 4; i++)
        std::cout << four_cc[i];
}

static void skip_chunk(const std::vector<Byte>& bytes, size_t& index)
{
    index += 4;  // skip tag
    u32 data_size = read_u32(bytes, index);
    index += data_size + (data_size % 2 == 1);
}

int main()
{
    std::ifstream input(wav_path, std::ios::binary);
    std::vector<Byte> bytes(std::istreambuf_iterator<char>(input), {});

    size_t index = 0;

    // riff chunk
    FourCC riff_tag = read_four_cc(bytes, index);
    assert(fourcc_eq(riff_tag, "RIFF"));
    assert(index == 4);
    assert(read_u32(bytes, index) + 8 == bytes.size());
    assert(index == 8);

    FourCC wave_tag = read_four_cc(bytes, index);
    assert(fourcc_eq(wave_tag, "WAVE"));
    assert(index == 12);

    skip_chunk(bytes, index);  // junk chunk
    skip_chunk(bytes, index);  // bext chunk
    skip_chunk(bytes, index);  // fake chunk

    FourCC fmt_tag = read_four_cc(bytes, index);
    assert(fourcc_eq(fmt_tag, "fmt "));
}
